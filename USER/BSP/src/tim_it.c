#include "tim_it.h"


/*bsp最先层自动初始化*/
void bsp_tim_init()
{
	// 开启LCD PWM调光
	TIM2->CCR2 = 0;
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
}
INIT_BSP_EARLY(bsp_tim_init);




