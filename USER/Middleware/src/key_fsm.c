#include "key_fsm.h"
#include "module_auto_init.h"
#include "utils_time.h"



/*按键单击回调函数*/
__attribute__((weak)) void key_ClickHandler(KEY_ID id)
{
	UNUSED(id);
}

/*KEY FSM结构体*/
struct KEY_FSM_Structure
{
	KEY_ID id;									// id
    KEY_FSM_STATE state;                        // 按键状态
	uint32_t debounce_delay;					// 消抖延时
    uint32_t last_tick;                         // 上次记录时间
    ClickHandle_t click_handle;                 // 按键单击回调函数
    KEY_STATE (*get_state)(KEY_ID);     // 获取状态函数
};

/*定义key状态机结构体*/
KEY_FSM_Structure key_fsm;
/*mid层自动初始化*/
void mid_key_fsm_init()
{
	// 初始化KEY状态机
	key_fsm = KEY_FSM_Init(KEY, key_ClickHandler);
}
INIT_MID(mid_key_fsm_init);

/***************************************************************************/
/***************************************************************************/
/*按键状态机初始化*/
KEY_FSM_Structure KEY_FSM_Init(KEY_ID id, ClickHandle_t click_handle)
{
    KEY_FSM_Structure fsm;

	fsm.id = id;
    fsm.state = KEY_FSM_STATE_IDLE;
	fsm.debounce_delay = 10;			// 消抖延时(ms)
    fsm.last_tick = 0;
    fsm.click_handle = click_handle;
    fsm.get_state = Key_GetState;       // 指向获取状态函数

    return fsm;
}

/*按键状态机轮询*/
void KEY_FSM_Run(KEY_FSM_Structure* fsm, uint32_t tick)
{
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
                if(get_tick_diff(tick, fsm->last_tick) > fsm->debounce_delay)
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

        // 按下
        case(KEY_FSM_STATE_PRESS):
        {
            // 等待松开
            if(fsm->get_state(fsm->id) == KEY_STATE_Release)
            {
                fsm->last_tick = tick;                          // 记录松开时间
                fsm->state = KEY_FSM_STATE_RELEASE_DEBOUNCE;    // 松开消抖
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
                if(get_tick_diff(tick, fsm->last_tick) > fsm->debounce_delay)
                {
                    fsm->state = KEY_FSM_STATE_RELEASE;  // 切换到松开状态
                }
            }
			// 误触
            else
            {
                fsm->state = KEY_FSM_STATE_PRESS;    // 回到按下
            }
        }
        break;

        // 松开
        case(KEY_FSM_STATE_RELEASE):
        {
            fsm->click_handle(fsm->id);		// 执行按下回调函数
            fsm->state = KEY_FSM_STATE_IDLE;    // 回到空闲
        }
        break;
		
		default:
		{
			
		}
		break;
    }
}

