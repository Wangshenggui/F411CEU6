#ifndef _KEY_FSM_H_
#define _KEY_FSM_H_

#include "main.h"
#include "key.h"

/*KEY FSM状态枚举*/
typedef enum
{
    KEY_FSM_STATE_IDLE,                     // 空闲
    KEY_FSM_STATE_RELEASE,                  // 松开
    KEY_FSM_STATE_RELEASE_DEBOUNCE,         // 松开消抖
    KEY_FSM_STATE_PRESS,                    // 按下
    KEY_FSM_STATE_PRESS_DEBOUNCE,           // 按下消抖
    KEY_FSM_STATE_COUNT
} KEY_FSM_STATE;

/*定义点击回调函数类型*/
typedef void (*ClickHandle_t)(KEY_ID id);

/*前向声明，防止外部修改*/
typedef struct KEY_FSM_Structure KEY_FSM_Structure;
/*定义按键状态机结构体*/
extern KEY_FSM_Structure key_fsm;

/*按键状态机初始化*/
KEY_FSM_Structure KEY_FSM_Init(KEY_ID id, ClickHandle_t click_handle);
/*按键状态机轮询*/
void KEY_FSM_Run(KEY_FSM_Structure* fsm, uint32_t tick);

#endif
