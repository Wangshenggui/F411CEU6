#include "key_control.h"
#include "system.h"

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
        .double_click_time = 200,   // 双击窗口，0表示禁用双击
    };
	// 初始化KEY状态机
	key_fsm = KEY_FSM_Init(&cfg);
}
INIT_MID(mid_key_fsm_init);

/*按键单击回调函数*/
void Key_Click_Callback(KEY_ID id)
{
	switch(id)
	{
		case(KEY):
		{
			debug_info("Click\r\n");
		}
		break;
		
		default:
		{
			
		}
		break;
	}
}

/*按键长按回调函数*/
void Key_LongPress_Callback(KEY_ID id)
{
	switch(id)
	{
		case(KEY):
		{
			debug_info("LongPress\r\n");
		}
		break;
		
		default:
		{
			
		}
		break;
	}
}

/*按键长按重复回调函数*/
void Key_LongPressRepeat_Callback(KEY_ID id)
{
	switch(id)
	{
		case(KEY):
		{
			debug_info("LongPressRepeat\r\n");
		}
		break;
		
		default:
		{
			
		}
		break;
	}
}

/*按键双击回调函数*/
void Key_DoubleClick_Callback(KEY_ID id)
{
	switch(id)
	{
		case(KEY):
		{
			debug_info("double\r\n");
		}
		break;
		
		default:
		{
			
		}
		break;
	}
}

