#ifndef _LED_FSM_H_
#define _LED_FSM_H_

#include "main.h"
#include "led.h"

/*LED FSM状态枚举*/
typedef enum
{
    LED_FSM_STATE_OFF,		// 关闭状态
    LED_FSM_STATE_ON,		// 开启状态
    LED_FSM_STATE_COUNT
} LED_FSM_STATE;

/*LED FSM事件枚举*/
typedef enum
{
    LED_FSM_EVENT_IDLE,		// 空闲
    LED_FSM_EVENT_OFF,		// 关闭
    LED_FSM_EVENT_ON,		// 开启
    LED_FSM_EVENT_BLINK,	// 闪烁
	LED_FSM_EVENT_TOGGLE,	// 切换状态
    LED_FSM_EVENT_COUNT
} LED_FSM_EVENT;

/*前向声明，防止外部修改*/
typedef struct LED_FSM_Structure LED_FSM_Structure;
/*蓝色LED状态机结构体*/
extern LED_FSM_Structure led_blue_fsm;

/*LED状态机初始化*/
LED_FSM_Structure LED_FSM_Init(LED_ID id);
/*LED状态机运行处理函数*/
void LED_FSM_Run(LED_FSM_Structure* fsm, uint32_t tick);
/*设置led闪烁*/
void LED_FSM_SetBlinkEvent(LED_FSM_Structure* fsm, uint32_t on_time, uint32_t off_time);
/*led状态切换*/
void LED_FSM_SetToggleEvent(LED_FSM_Structure* fsm);
/*关闭led*/
void LED_FSM_SetOFFEvent(LED_FSM_Structure* fsm);
/*开启led*/
void LED_FSM_SetONEvent(LED_FSM_Structure* fsm);

#endif
