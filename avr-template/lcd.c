#include "lcd.h"
#include <iom16v.h>
#include <macros.h>
#include "ascii_table.h"

void lcd_init(void)
{
	PORTA &= ~LCD_RST;
	PORTA |= LCD_RST;
	PORTA &= ~LCD_CE ;

	lcd_display_byte(0x21, 0);
	lcd_display_byte(0xC8, 0);
	lcd_display_byte(0x20, 0);
	lcd_clear(0x00);
	lcd_display_byte(0x0C, 0);

	PORTA |= LCD_CE;
}

void lcd_set_position(const unsigned char begin_row, const unsigned char begin_clo)
{
	lcd_display_byte(0x40 | begin_clo, 0);
	lcd_display_byte(0x80 | begin_row, 0);
}

void lcd_display_byte(const unsigned char data, const unsigned char command)
{
	PORTA &= ~LCD_CE;
	PORTA = command ? PORTA | LCD_DC : PORTA & ~LCD_DC;
	SPDR = data;
	while (!(SPSR & (1 << 7)));
	PORTA |= LCD_CE;
}

void lcd_clear(const unsigned char data)
{
	unsigned int i;
	lcd_display_byte(0x0C, 0);
	lcd_display_byte(0x80, 0);
	// 6 * 84
	for (i = 0; i < 504; i++)
		lcd_display_byte(data, 1);
}

void lcd_display_ascii_char(const unsigned char c)
{
	unsigned char i;
	for (i = 0; i < 6; i++)
		lcd_display_byte(ascii_table[c - 32][i], 1);
}

void lcd_display_english_string(const unsigned char begin_row, const unsigned char begin_clo,
                                char *str)
{
	lcd_set_position(begin_row, begin_clo);
	while (*str)
		lcd_display_ascii_char(*str++);
}

void lcd_display_chinese_char(const unsigned char row, const unsigned char col,
                              const unsigned char *str)
{
	unsigned char i, j;
	for (i = 0; i < 2; i++) {
		lcd_set_position(row, col + i);
		for (j = 0; j < 12; j++) {
			lcd_display_byte(*(12 * i + str + j), 1);
		}
	}
}

void lcd_draw_bmp_pixel(const unsigned char begin_row,
                        const unsigned char begin_col,
                        const unsigned char *map,
                        const unsigned char pix_long, const unsigned char pix_hight)
{
	unsigned int i, j;
	unsigned char tmp = pix_long / 8 + (!!(pix_long % 8));
	for (i = 0; i < tmp; i++) {
		lcd_set_position(begin_row, begin_col++);
		for (j = 0; j < pix_hight; j++) {
			lcd_display_byte(map[i * pix_long + j], 1);
		}
	}
}