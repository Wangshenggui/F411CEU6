#include "key_control.h"
#include "system.h"


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
			debug_info("Long\r\n");
		}
		break;
		
		default:
		{
			
		}
		break;
	}
}

