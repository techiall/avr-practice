/*
   imakew -f FIRST.mak
*/

#include <iom16v.h>
#include <macros.h>
#include "init_avr.h"
#include "led_send_display.h"

unsigned long time = 0;
#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{
	time++;
}

#define COLUMNS 16
#define CH_COUNT 17

unsigned char light[16][8];

void display_light(void)
{
	int i, j;
	for (i = 0; i < 16; i++) {
		for (j = 7; j >= 0; j--) {
			send_data(light[i][j], COLUMNS - 1 - i, 0);
		}
		while (time % 5) {}
	}
}

void set_point(int row, int col, int open)
{
	if (open == 0)
		light[row][col / 8] &= ~(1 << (col % 8));
	else if (open == 1)
		light[row][col / 8] |= 1 << (col % 8);
	else if (open == 2)
		light[row][col / 8] ^= 1 << (col % 8);
}

int main(void)
{
	init();
	while (1) {
		int row = 16 * 4;
		int col = 16;
		int num = 1;
		int h = 0;
		int v = row - 1;
		int tmpx = col, tmpy = row;
		int i = 0;
		while (num < row * col) {
			set_point(h, v, 1);
			display_light();
			for (i = 0; i < tmpx - 1; i++) {
				set_point(++h, v, 1);
				num++;
				display_light();
			}
			for (i = 0; i < tmpy - 1; i++) {
				set_point(h, --v, 1);
				num++;
				display_light();
			}
			for (i = 0; i < tmpx - 1; i++) {
				set_point(--h, v, 1);
				num++;
				display_light();
			}
			for (i = 0; i < tmpy - 2; i++) {
				set_point(h, ++v, 1);
				num++;
				display_light();
			}
			h++;
			tmpx -= 2;
			tmpy -= 2;
		}
		for (i = 0; i < 16; i++) {
			for (v = 0; v < 8; v++)
				light[i][v] = 0;
		}
		PORTD = 0;
	}
	return 0;
}
