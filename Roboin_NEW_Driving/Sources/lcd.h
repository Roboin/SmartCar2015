/*
 * lcd.h
 *
 */

#ifndef LCD_H_
#define LCD_H_

#include "jdp.h"
#include "gpio.h"

#define F_CPU 64000000

#define LCD_ENA_PIN     GPIO_PF13
#define LCD_CLK_PIN     GPIO_PF14
#define LCD_DATA_PIN	GPIO_PF15

#define LCD_PIT1_GAP            10//update LCD each 10ms

#define LCD_CMD_HOME            0x02    /* cursor home */
#define LCD_CMD_ENTRY_DEC       0x04    /* decrement, no shift */
#define LCD_CMD_ENTRY_DEC_SHIFT 0x05    /* decrement with shift */
#define LCD_CMD_ENTRY_INC       0x06    /* increment, no shift */
#define LCD_CMD_ENTRY_INC_SHIFT 0x07    /* increment with shift */
#define LCD_CMD_CLEAR           0x01    /* clear, cursor home */
#define LCD_CMD_OFF             0x08    /* display off */
#define LCD_CMD_ON              0x0C    /* display on, cursor off */
#define LCD_CMD_ON_BLINK        0x0D    /* display on, cursor off, blink char */
#define LCD_CMD_ON_CURSOR       0x0E    /* display on, cursor on */
#define LCD_CMD_ON_CURSOR_BLINK 0x0F    /* display on, cursor on, blink char */
#define LCD_CMD_CURSOR_LEFT     0x10    /* decrement cursor (left) */
#define LCD_CMD_CURSOR_RIGHT    0x14    /* increment cursor (right) */
#define LCD_CMD_SHIFT_LEFT      0x18    /* shift (left) */
#define LCD_CMD_SHIFT_RIGHT     0x1C    /* shift (right)*/
#define LCD_FUNCTION_1          0x30   /* 5x7 characters, single line display */
#define LCD_FUNCTION_2          0x38   /* 5x7 characters, 2 line display */

void LCD_PtS(unsigned char data);
void LCD_ENA_HL(void);
void LCD_com(void);
void LCD_data(const char c);// putchar()
void LCD_XY(unsigned char x, unsigned char y);
void LCD_string(unsigned char x, unsigned char y, char *string);
void LCD_Clear(void);
void LCD_ON(void);
void LCD_OFF(void);
void LCD_initialize(void);

void LCD_PIT1_ISR (void);
void LCD_delay_us(uint32_t delay);
void LCD_UPDATE_STRING(unsigned char x, unsigned char y, char *string);


#endif /* CHARLCD_H_ */
