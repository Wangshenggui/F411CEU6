#include "key_fsm.h"
#include "module_auto_init.h"


/*按键单击回调函数*/
__weak void Key_Click_Callback(KEY_ID id)
{
	UNUSED(id);
}
////////////////////////////////////////////////////////////
/*按键长按回调函数*/
__weak void Key_LongPress_Callback(KEY_ID id)
{
	UNUSED(id);
}
////////////////////////////////////////////////////////////
/*按键长按重复回调函数*/
__weak void Key_LongPressRepeat_Callback(KEY_ID id)
{
	UNUSED(id);
}
////////////////////////////////////////////////////////////
/*按键双击回调函数*/
__weak void Key_DoubleClick_Callback(KEY_ID id)
{
	UNUSED(id);
}


/*KEY FSM结构体*/
struct KEY_FSM_Structure
{
	KEY_ID id;									// id
    KEY_STATE (*get_state)(KEY_ID);				// 获取状态函数
    KEY_FSM_STATE state;                        // 按键状态
    KEY_FSM_STATE last_state;                   // 上个状态
	uint32_t debounce_delay;					// 消抖延时
    uint32_t last_tick;                         // 上次记录时间
    uint32_t long_press_time;                   // 长按触发时间
    uint32_t long_repeat_time;                  // 长按重复触发时间
    uint32_t double_click_time;                 // 双击窗口
};

/*定义key状态机结构体*/
KEY_FSM_Structure *key_fsm;
/*mid层自动初始化*/
void mid_key_fsm_init()
{
    // 配置状态机结构体
	const KEY_FSM_Config cfg = 
    {
        .id = KEY,
        .get_state = Key_GetState,
        .long_press_time = 1000,    // 长按触发时间
        .long_repeat_time = 200,    // 连续触发周期
        .double_click_time = 300,   // 双击窗口，0表示禁用双击
    };
	// 初始化KEY状态机
	key_fsm = KEY_FSM_Init(&cfg);
}
INIT_MID(mid_key_fsm_init);
/***************************************************************************/
/***************************************************************************/
static inline uint32_t key_get_tick_diff(uint32_t current, uint32_t previous)
{
    return current - previous;
}

/*分配静态内存池*/
static KEY_FSM_Structure fsm_pool[KEY_FSM_MAX_NUM];
static uint8_t fsm_used = 0;	// 第一块内存开始分配

/*按键状态机初始化*/
KEY_FSM_Structure *KEY_FSM_Init(const KEY_FSM_Config *cfg)
{
	if (cfg == NULL || cfg->get_state == NULL)
	{
        return NULL;
    }
    if (fsm_used >= KEY_FSM_MAX_NUM)
	{
        return NULL;                 // 池满
    }
    KEY_FSM_Structure *fsm = &fsm_pool[fsm_used++];

	fsm->id = cfg->id;                               // 按键ID
    fsm->get_state = cfg->get_state;                 // 指向获取状态函数
    fsm->state = KEY_FSM_STATE_IDLE;                 // 初始化空闲状态
    fsm->last_state = KEY_FSM_STATE_IDLE;            // 初始化空闲状态
	fsm->debounce_delay = KEY_DEBOUNCE_DELAY;            // 消抖延时(ms)
    fsm->last_tick = 0;
    fsm->long_press_time = cfg->long_press_time;     // 长按触发延时
    fsm->long_repeat_time = cfg->long_repeat_time;   // 长按重复触发时间
    fsm->double_click_time = cfg->double_click_time; // 双击窗口

    return fsm;
}

/*按键状态机轮询*/
void KEY_FSM_Run(KEY_FSM_Structure* fsm, uint32_t tick)
{
	if (fsm == NULL)
	{
        return;
    }
    switch(fsm->state)
    {
        // 空闲
        case(KEY_FSM_STATE_IDLE):
        {
            // 检测到按下
            if(fsm->get_state(fsm->id) == KEY_STATE_Press)
            {
                fsm->last_tick = tick;                      // 记录按下时时间
                fsm->state = KEY_FSM_STATE_PRESS_DEBOUNCE;  // 切换到按下消抖状态
            }
        }
        break;

        // 按下消抖
        case(KEY_FSM_STATE_PRESS_DEBOUNCE):
        {
            // 检测到按下
            if(fsm->get_state(fsm->id) == KEY_STATE_Press)
            {
                // 持续按下超过DEBOUNCE_TIME，确认按下
                if(key_get_tick_diff(tick, fsm->last_tick) > fsm->debounce_delay)
                {
                    fsm->state = KEY_FSM_STATE_PRESS;  // 切换到按下状态
                }
            }
            else    // 误触
            {
                fsm->state = KEY_FSM_STATE_IDLE;    // 回到空闲
            }
        }
        break;

        // 确认按下
        case(KEY_FSM_STATE_PRESS):
        {
            // 等待松开
            if(fsm->get_state(fsm->id) == KEY_STATE_Release)
            {
                fsm->last_tick = tick;                          // 记录松开时间
                fsm->last_state = fsm->state;                   // 记录当前状态
                fsm->state = KEY_FSM_STATE_RELEASE_DEBOUNCE;    // 松开消抖
            }
            // 触发长按
            else if(key_get_tick_diff(tick, fsm->last_tick) >= fsm->long_press_time)
            {
                fsm->state = KEY_FSM_STATE_LONG;        // 触发长按
            }
        }
        break;

        // 松开消抖
        case(KEY_FSM_STATE_RELEASE_DEBOUNCE):
        {
            // 继续检测松开
            if(fsm->get_state(fsm->id) == KEY_STATE_Release)
            {
                // 松开超过DEBOUNCE_TIMEms，确认松开
                if(key_get_tick_diff(tick, fsm->last_tick) > fsm->debounce_delay)
                {
                    if(fsm->last_state == KEY_FSM_STATE_PRESS)
                    {
                        fsm->state = KEY_FSM_STATE_RELEASE;  // 切换到松开状态
                    }
                    // 长按重复跳转的
                    else if(fsm->last_state == KEY_FSM_STATE_LONG_REPEAT)
                    {
                        fsm->state = KEY_FSM_STATE_IDLE;    // 回到空闲
                    }
                }
            }
			// 误触
            else
            {
                fsm->state = fsm->last_state;       // 回到原状态
            }
        }
        break;

        // 松开
        case(KEY_FSM_STATE_RELEASE):
        {
            // 通过双击窗口选择是否触发双击
            if(fsm->double_click_time > 0)
            {
                fsm->last_tick = tick;
                fsm->state = KEY_FSM_STATE_CLICK_WAIT;    // 等待第二次按下
            }
            else
            {
                fsm->state = KEY_FSM_STATE_CLICK;   // 直接触发单击
            }
        }
        break;

        // 等待第二次按下
        case(KEY_FSM_STATE_CLICK_WAIT):
        {
            // 等待期内再次按下
            if(fsm->get_state(fsm->id) == KEY_STATE_Press)
            {
                fsm->last_tick = tick;
                fsm->state = KEY_FSM_STATE_DOUBLE_PRESS_DEBOUNCE;    // 第二次按下消抖
            }
            // 超时确认单击
            else if(key_get_tick_diff(tick, fsm->last_tick) >= fsm->double_click_time)
            {
                fsm->state = KEY_FSM_STATE_CLICK;
            }
        }
        break;

        // 第二次按下消抖
        case (KEY_FSM_STATE_DOUBLE_PRESS_DEBOUNCE):
        {
            if(fsm->get_state(fsm->id) == KEY_STATE_Press)
            {
                // 连续按下
                if(key_get_tick_diff(tick, fsm->last_tick) > fsm->debounce_delay)
                {
                    fsm->last_tick = tick;
                    fsm->state = KEY_FSM_STATE_DOUBLE_RELEASE_DEBOUNCE;  // 等松开
                }
            }
            else
            {
                // 抖动误触：回 CLICK_WAIT，重置基准
                fsm->last_tick = tick;
                fsm->state = KEY_FSM_STATE_CLICK_WAIT;
            }
        }
        break;

        // 第二次松开消抖
        case (KEY_FSM_STATE_DOUBLE_RELEASE_DEBOUNCE):
        {
            // 在双击窗口内
            if(key_get_tick_diff(tick, fsm->last_tick) <= fsm->double_click_time)
            {
                if(fsm->get_state(fsm->id) == KEY_STATE_Release)
                {
                    // 确认松开，触发双击回调
                    if(key_get_tick_diff(tick, fsm->last_tick) > fsm->debounce_delay)
                    {
                        fsm->state = KEY_FSM_STATE_DOUBLE;   // 触发双击
                    }
                }
            }
            // 超过双击窗口视为无效双击
            else
            {
                // 双击按下超时，检测到松开
                if(fsm->get_state(fsm->id) == KEY_STATE_Release)
                {
                    fsm->state = KEY_FSM_STATE_IDLE;    // 回到空闲
                }
            }
        }
        break;

        // 单击
        case(KEY_FSM_STATE_CLICK):
        {
            Key_Click_Callback(fsm->id);        // 执行按下回调函数
            fsm->state = KEY_FSM_STATE_IDLE;    // 回到空闲
        }
        break;

        // 双击
        case(KEY_FSM_STATE_DOUBLE):
        {
            Key_DoubleClick_Callback(fsm->id);  // 执行双击回调函数
            fsm->last_tick = tick;
            fsm->state = KEY_FSM_STATE_LOCK;    // 先上锁，防止三连击导致触发单击
        }
        break;

        // 长按
        case(KEY_FSM_STATE_LONG):
        {
            fsm->last_tick = tick;                          // 记录第一次长按时间
            Key_LongPress_Callback(fsm->id);                // 执行长按回调函数
            fsm->state = KEY_FSM_STATE_LONG_REPEAT;         // 长按重复
        }
        break;

        // 长按重复
        case(KEY_FSM_STATE_LONG_REPEAT):
        {
            // 检测到松开
            if(fsm->get_state(fsm->id) == KEY_STATE_Release)
            {
                fsm->last_state = fsm->state;   // 记录当前状态
                fsm->state = KEY_FSM_STATE_RELEASE_DEBOUNCE;    // 松开消抖
            }
            else if(key_get_tick_diff(tick, fsm->last_tick) >= fsm->long_repeat_time)
            {
                fsm->last_tick = tick;                  // 重新记录连续触发基准
                Key_LongPressRepeat_Callback(fsm->id);  // 执行长按重复回调函数
            }
        }
        break;

        // 状态机上锁
        case(KEY_FSM_STATE_LOCK):
        {
            // 等待解锁
            if(key_get_tick_diff(tick, fsm->last_tick) > KEY_LOCK_DELAY)
            {
                fsm->state = KEY_FSM_STATE_IDLE;    // 回到空闲
            }
        }
        break;
        
		default:
		{
			fsm->state = KEY_FSM_STATE_IDLE;    // 回到空闲
		}
		break;
    }
}

