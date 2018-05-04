/*
imakew -f FIRST.mak
*/
#include <iom16v.h>
#include <macros.h>
#include "init_avr.h"
#include "lcd.h"
#include "char_ascii.h"

#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{}


void display_chinese_string(struct CHINESE_ASCII c[], const unsigned long len)
{
	unsigned char i;
	for (i = 0; i < len; i++) 
		lcd_display_chinese_char(12 * i, 0, c[i].data);
}

int main(void)
{
	char str[][23] = {
		"",
		"3310LCD test!",
		"",
		"1  3310LCD Init",
		"“2  3310 write",
		"3  3310 display"
	};
	int size = sizeof(str) / sizeof(*str);
	int i;
	init();
	lcd_init();
	for (i = 0; i < size; i++)
		lcd_display_english_string(0, i, str[i]);
	return 0;
}