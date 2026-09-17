#ifndef _KEY_FSM_H_
#define _KEY_FSM_H_

#include "main.h"
#include "key.h"

/*默认消抖时间(ms)*/
#define DEBOUNCE_DELAY  10

/*KEY FSM状态枚举*/
typedef enum
{
    KEY_FSM_STATE_IDLE,                     // 空闲
    KEY_FSM_STATE_PRESS,                    // 按下
    KEY_FSM_STATE_PRESS_DEBOUNCE,           // 按下消抖
    KEY_FSM_STATE_RELEASE,                  // 松开
    KEY_FSM_STATE_RELEASE_DEBOUNCE,         // 松开消抖
    KEY_FSM_STATE_CLICK,                    // 单击
    KEY_FSM_STATE_CLICK_WAIT,               // 等待第二次按下
    KEY_FSM_STATE_DOUBLE_PRESS,             // 第二次按下消抖
    KEY_FSM_STATE_DOUBLE_RELEASE,           // 第二次松开消抖
    KEY_FSM_STATE_LONG,                     // 长按
    KEY_FSM_STATE_LONG_REPEAT,              // 长按重复
    KEY_FSM_STATE_COUNT
} KEY_FSM_STATE;

/*定义点击回调函数类型*/
typedef void (*ClickHandle_t)(KEY_ID id);
typedef void (*LongHandle_t)(KEY_ID id);

/*状态机配置结构体*/
typedef struct
{
	KEY_ID id;									// id
    KEY_STATE (*get_state)(KEY_ID);				// 获取状态函数
    uint32_t long_press_time;                   // 长按触发时间
    uint32_t long_repeat_time;                  // 长按重复触发时间
    uint32_t double_click_time;                 // 双击窗口
} KEY_FSM_Config;

/*前向声明，防止外部修改*/
typedef struct KEY_FSM_Structure KEY_FSM_Structure;
/*定义按键状态机结构体*/
extern KEY_FSM_Structure key_fsm;

/*按键状态机初始化*/
KEY_FSM_Structure KEY_FSM_Init(const KEY_FSM_Config *cfg);
/*按键状态机轮询*/
void KEY_FSM_Run(KEY_FSM_Structure* fsm, uint32_t tick);

#endif
