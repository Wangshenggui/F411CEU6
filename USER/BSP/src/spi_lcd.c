#include "spi_lcd.h"
#include "module_auto_init.h"
#include "spi_it.h"

/*注册lcd spi*/
static spi_dma_t lcd_spi = 
{
	.hspi = &hspi1,
	.tx_complete = 1
};
/*bsp层自动初始化*/
void bsp_lcd_init()
{
	// 注册spi设备
	SPI_Register(&lcd_spi);
	// 初始化LCD
	Lcd_Init();
	// 背光亮度
	LCD_BLK_Bright(30);
}
INIT_BSP(bsp_lcd_init);
/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();	//写命令
	SPI_WriteDMA(&lcd_spi, &dat, 1);
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_DC_Set();	//写数据
	SPI_WriteDMA(&lcd_spi, &dat, 1);
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
	static uint8_t buff[2];
	LCD_DC_Set();	//写数据
	
	buff[0]=dat>>8;
	buff[1]=dat;
	SPI_WriteDMA(&lcd_spi, buff, 2);
}

/******************************************************************************
LCD初始化
******************************************************************************/
void Lcd_Init(void)
{
	// CS选中LCD
	LCD_CS_Clr();
	LCD_RST_Set();
	LCD_DC_Set();
	
	LCD_RST_Clr();
	HAL_Delay(20);
	LCD_RST_Set();
	HAL_Delay(20);
	
	//GC9A01 +HSD1.09 init code /////////////
	LCD_WR_REG(0xFE);
	LCD_WR_REG(0xEF);
	LCD_WR_REG(0xEB);
	LCD_WR_DATA8(0x14);
	LCD_WR_REG(0x84);
	LCD_WR_DATA8(0x40);
	LCD_WR_REG(0x88);
	LCD_WR_DATA8(0x0A);
	LCD_WR_REG(0x89);
	LCD_WR_DATA8(0x21);
	LCD_WR_REG(0x8A);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0x8B);
	LCD_WR_DATA8(0x80);
	LCD_WR_REG(0x8C);
	LCD_WR_DATA8(0x01);
	LCD_WR_REG(0x8D);
	LCD_WR_DATA8(0x03);
	LCD_WR_REG(0x8F);
	LCD_WR_DATA8(0xFF);
	LCD_WR_REG(0xB6);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x60);
	LCD_WR_REG(0x36);
	LCD_WR_DATA8(0x88);
	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0x90);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x08);
	LCD_WR_REG(0xBD);
	LCD_WR_DATA8(0x06);
	LCD_WR_REG(0xBC);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xFF);
	LCD_WR_DATA8(0x60);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);
	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x2F);
	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x2F);
	LCD_WR_REG(0xC9);
	LCD_WR_DATA8(0x25);
	LCD_WR_REG(0xBE);
	LCD_WR_DATA8(0x11);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x0E);
	LCD_WR_REG(0xDF);
	LCD_WR_DATA8(0x21);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x02);
	LCD_WR_REG(0xF0);
	LCD_WR_DATA8(0x49);
	LCD_WR_DATA8(0x0e);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2e);
	LCD_WR_REG(0xF1);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x73);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x6F);
	LCD_WR_REG(0xF2);
	LCD_WR_DATA8(0x49);
	LCD_WR_DATA8(0x0e);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2e);
	LCD_WR_REG(0xF3);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x73);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x6F);
	LCD_WR_REG(0xED);
	LCD_WR_DATA8(0x1B);
	LCD_WR_DATA8(0x8B);
	LCD_WR_REG(0xAE);
	LCD_WR_DATA8(0x77);
	LCD_WR_REG(0xCD);
	LCD_WR_DATA8(0x63);
	LCD_WR_REG(0xAC);
	LCD_WR_DATA8(0x27);
	LCD_WR_REG(0x70);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x06);//VGH
	LCD_WR_DATA8(0x0F); //VGL
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x03);
	LCD_WR_REG(0xE8);
	LCD_WR_DATA8(0x24);
	LCD_WR_REG(0x60);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x6D);
	LCD_WR_DATA8(0x6D);
	LCD_WR_DATA8(0x39);
	LCD_WR_DATA8(0xF0);
	LCD_WR_DATA8(0x6D);
	LCD_WR_DATA8(0x6D);
	LCD_WR_REG(0x61);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0xF4);
	LCD_WR_DATA8(0x6D);
	LCD_WR_DATA8(0x6D);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0xF7);
	LCD_WR_DATA8(0x6D);
	LCD_WR_DATA8(0x6D);/////////////////////////////////
	LCD_WR_REG(0x62);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xED);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xEF);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x70);
	LCD_WR_REG(0x63);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xF1);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x38);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x71);
	LCD_WR_DATA8(0xF3);
	LCD_WR_DATA8(0x70);
	LCD_WR_DATA8(0x70);
	LCD_WR_REG(0x64);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x29);
	LCD_WR_DATA8(0xF1);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0xF1);
	LCD_WR_DATA8(0x00);//
	LCD_WR_DATA8(0x1a);//
	LCD_WR_REG(0x66);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x98);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x32);
	LCD_WR_DATA8(0x45);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0x67);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x67);
	LCD_WR_DATA8(0x45);
	LCD_WR_DATA8(0xcd);
	LCD_WR_REG(0x74);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x85); //85
	LCD_WR_DATA8(0x80);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x4E);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0x98);
	LCD_WR_DATA8(0x3e);
	LCD_WR_DATA8(0x07);
	LCD_WR_REG(0x99);
	LCD_WR_DATA8(0x3e);
	LCD_WR_DATA8(0x07);
	LCD_WR_REG(0x35);
	LCD_WR_REG(0x21);
	HAL_Delay(120);
	//--------end gamma setting--------------//
	LCD_WR_REG(0x11);
	HAL_Delay(320);
	LCD_WR_REG(0x29);
	HAL_Delay(120);
	LCD_WR_REG(0x2C);


	LCD_Clear(BLACK);
	
	// 释放LCD
	LCD_CS_Set();
}



/******************************************************************************
	  函数说明：设置起始和结束地址
	  入口数据：x1,x2 设置列的起始和结束地址
				y1,y2 设置行的起始和结束地址
	  返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	// CS选中LCD
	LCD_CS_Clr();
	LCD_WR_REG(0x2a);	//列地址设置
	LCD_WR_DATA(x1);
	LCD_WR_DATA(x2);
	LCD_WR_REG(0x2b);	//行地址设置
	LCD_WR_DATA(y1);
	LCD_WR_DATA(y2);
	LCD_WR_REG(0x2c);	 //储存器写
	// 释放LCD
	LCD_CS_Set();
}

/******************************************************************************
	  函数说明：LCD清屏函数
	  入口数据：无
	  返回值：  无
******************************************************************************/
// 定义分块大小
#define BLOCK_HEIGHT 10	// 240行数的整数倍数，便于整除
static uint16_t block_buffer[LCD_W * BLOCK_HEIGHT];
void LCD_Clear(uint16_t Color)
{
	// CS选中LCD
	LCD_CS_Clr();
	// 总像素
    uint32_t block_pixels = LCD_W * BLOCK_HEIGHT;
    
    // 填充缓冲区
    for(uint32_t i = 0; i < block_pixels; i++)
	{
        block_buffer[i] = __REV16(Color);
	}
    
    // 设置地址（整个屏幕）
    LCD_Address_Set(0, 0, LCD_W - 1, LCD_H - 1);
    LCD_DC_Set();
    
    // 计算块
    uint16_t blocks = LCD_H / BLOCK_HEIGHT;
    
    for(uint16_t block = 0; block < blocks; block++)
    {
        SPI_WriteDMA(&lcd_spi, (uint8_t*)block_buffer, LCD_W * BLOCK_HEIGHT * 2);
    }
	// 释放LCD
	LCD_CS_Set();
}

void LCD_Fill(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint16_t Color)
{
    uint16_t width  = x1 - x0 + 1;
    uint16_t height = y1 - y0 + 1;

    uint32_t block_pixels = width * BLOCK_HEIGHT;

    // 填充 block buffer
    for(uint32_t i = 0; i < block_pixels; i++)
        block_buffer[i] = Color;

    LCD_Address_Set(x0, y0, x1, y1);

    LCD_DC_Set();

    uint16_t full_blocks = height / BLOCK_HEIGHT;
    uint16_t remain_rows = height % BLOCK_HEIGHT;

    // 发送完整块
    for(uint16_t b = 0; b < full_blocks; b++)
    {
        SPI_WriteDMA(&lcd_spi, (uint8_t*)block_buffer, width * BLOCK_HEIGHT * 2);
    }

    // 发送剩余行
    if(remain_rows)
    {
        SPI_WriteDMA(&lcd_spi, (uint8_t*)block_buffer, width * remain_rows * 2);
    }
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_Address_Set(x, y, x, y); //设置光标位置
	LCD_WR_DATA(color);
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量
	delta_y = y2 - y1;
	uRow = x1; //画线起点坐标
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //设置单步方向
	else if (delta_x == 0)
		incx = 0; //垂直线
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //水平线
	else
	{
		incy = -1;
		delta_y = -delta_x;
	}
	if (delta_x > delta_y)
		distance = delta_x; //选取基本增量坐标轴
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		LCD_DrawPoint(uRow, uCol, color); //画点
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

