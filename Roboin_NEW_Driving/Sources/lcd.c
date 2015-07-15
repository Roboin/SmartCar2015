/*
 * lcd.c
 * Library For MAI-SLCD-216-v2.0 
 * Converted from Arduino Library, From http://cafe.naver.com/at128mai
 */

#include "jdp.h"
#include "gpio.h"
#include "lcd.h"

char str1[16] = "MAI-SLCD-216-V20";
char str2[16] = "test test test";


uint32_t LCD_timer_ms = 0;
void LCD_PIT1_ISR (void)
{
	LCD_timer_ms += 200;
	PIT.CH[1].TFLG.R = 0x00000001;
}

void LCD_initialize(void)
{
	LCD_PtS(LCD_FUNCTION_2);
	LCD_com();
	
	LCD_PtS(LCD_CMD_ENTRY_INC);
	LCD_com();
	
	LCD_OFF();
	LCD_Clear();
	LCD_ON();
}

uint32_t delay_t = 1;
void LCD_PtS(unsigned char data)
{
	int i = 0;
	GPIO_Set(LCD_CLK_PIN,0);
	
	while (i <= 7)
	{
		if (data & 0x80)	      // LOW BIT
		{
			GPIO_Set(LCD_DATA_PIN,1);
		}
		else			// HIGH BIT
		{
			GPIO_Set(LCD_DATA_PIN,0);
		}
		data = data << 1;
		
		GPIO_Set(LCD_CLK_PIN,1);
		LCD_delay_us(delay_t);
		GPIO_Set(LCD_CLK_PIN,0);
		i++;
	}
}

void LCD_ENA_HL(void)
{
	GPIO_Set(LCD_ENA_PIN,1);
	GPIO_Set(LCD_ENA_PIN,0);
}

void LCD_com(void)
{
	GPIO_Set(LCD_DATA_PIN,0);
	LCD_ENA_HL();
}

void LCD_data(const char c)
{
	LCD_PtS(c);
	GPIO_Set(LCD_DATA_PIN,1);
	LCD_ENA_HL();
}

void LCD_XY(unsigned char x, unsigned char y)
{
  LCD_PtS(0x80 | (x + y * 0x40));
  LCD_com();
}

void LCD_string(unsigned char x, unsigned char y, char *string)
{
  LCD_XY(x, y);
  while (*string != '\0')
  {
    LCD_data(*string);
    string++;
  }
}

void LCD_Clear(void)
{
  LCD_PtS(LCD_CMD_CLEAR );
  LCD_com();
  LCD_delay_us(delay_t<<1);
}

void LCD_ON(void)
{
  LCD_PtS(LCD_CMD_ON);
  LCD_com();
}

void LCD_OFF(void)
{
  LCD_PtS(LCD_CMD_OFF);
  LCD_com();
}

void LCD_delay_us(uint32_t delay){//delay half us
	uint32_t temp = 0;
	while( (delay<<(6-1)) > temp){
		temp++;
	}
}
