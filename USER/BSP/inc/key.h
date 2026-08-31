#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"
#include <stdbool.h>


/*按键 ID枚举*/
typedef enum
{
    KEY,
    KEY_COUNT           // 总数
} KEY_ID;

/*KEY极性枚举*/
typedef enum
{
    KEY_POLARITY_LOW,   // 低电平表示按下
    KEY_POLARITY_HIGH,  // 高电平表示按下
} KEY_POLARITY;

/*KEY状态枚举*/
typedef enum
{
    KEY_STATE_Release,		// 松开
    KEY_STATE_Press,		// 按下
	KEY_STATE_NONE			// 无事件
} KEY_STATE;

// 前向声明，防止外部修改
typedef struct Key_Structure Key_Structure;
// 定义按键结构体
extern Key_Structure key;

/*key初始化*/
void Key_Init(void);
/*获取按键电平状态*/
KEY_STATE Key_GetState(KEY_ID id);

#endif
