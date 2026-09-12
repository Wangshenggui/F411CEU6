#ifndef _SPI_LCD_H
#define _SPI_LCD_H

#include "main.h"
#include "spi.h"


/*分辨率*/
#define LCD_W 240
#define LCD_H 240

/*CS*/
#define LCD_CS_Clr()  HAL_GPIO_WritePin(SPI_LCD_CS_GPIO_Port,SPI_LCD_CS_Pin,GPIO_PIN_RESET)
#define LCD_CS_Set()  HAL_GPIO_WritePin(SPI_LCD_CS_GPIO_Port,SPI_LCD_CS_Pin,GPIO_PIN_RESET)
/*RES*/
#define LCD_RST_Clr() HAL_GPIO_WritePin(SPI_LCD_RESET_GPIO_Port,SPI_LCD_RESET_Pin,GPIO_PIN_RESET)//RES
#define LCD_RST_Set() HAL_GPIO_WritePin(SPI_LCD_RESET_GPIO_Port,SPI_LCD_RESET_Pin,GPIO_PIN_SET)
/*DC*/
#define LCD_DC_Clr() HAL_GPIO_WritePin(SPI_LCD_DC_GPIO_Port,SPI_LCD_DC_Pin,GPIO_PIN_RESET)//DC
#define LCD_DC_Set() HAL_GPIO_WritePin(SPI_LCD_DC_GPIO_Port,SPI_LCD_DC_Pin,GPIO_PIN_SET)
/*背光*/
#define LCD_BLK_Clr()  HAL_GPIO_WritePin(SPI_LCD_BLC_GPIO_Port,SPI_LCD_BLC_Pin,GPIO_PIN_RESET)
#define LCD_BLK_Set()  HAL_GPIO_WritePin(SPI_LCD_BLC_GPIO_Port,SPI_LCD_BLC_Pin,GPIO_PIN_SET)
#define LCD_BLK_Bright(percent)	TIM2->CCR2 = (percent);

void Lcd_Init(void);
void LCD_Clear(uint16_t Color);
void LCD_Fill(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint16_t Color);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/*常用颜色定义*/
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40	//棕色
#define BRRED 0XFC07	//棕红色
#define GRAY 0X8430	//灰色
#define DARKBLUE 0X01CF	//深蓝色
#define LIGHTBLUE 0X7D7C	//浅蓝色
#define GRAYBLUE 0X5458	//灰蓝色
#define LIGHTGREEN 0X841F	//浅绿色
#define LGRAY 0XC618	//浅灰色
#define LGRAYBLUE 0XA651	//浅灰蓝色
#define LBBLUE 0X2B12	//浅棕蓝色


#endif
