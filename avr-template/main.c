/*
   imakew -f FIRST.mak
*/

#include <iom16v.h>
#include <macros.h>
#include "init_avr.h"
#include "ku.h"
#include "led_send_display.h"

unsigned long time = 0;
#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{
	time++;
}

#define COLUMNS 16
#define CH_COUNT 11

unsigned char light[16][8];

void send_ch(int col, int begin, int offset, const unsigned char *first[])
{
	int i;
	for (i = 4; i >= 0; i++) {
		unsigned char left = 0, right = 0;
		int ch_idx = begin + i;
		if (ch_idx >= 0 && ch_idx < CH_COUNT) {
			right = *(first[i] + col * 2 + 1);
			left = *(first[i] + col * 2);
		}
		send_data(right, i == 0 && offset > 8 ? offset - 8 : 0, 0);
		send_data(left, (i == 0) * offset, 0);
	}
	while (time % 4) {}
	PORTD = COLUMNS - 1 - col;
}

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

int get_point(int row, int col)
{
	return light[row][col / 8] & (1 << (col % 8));
}


int main(void)
{
	int cnt = 4;
	int offset = 0;
	unsigned long num = 1212;
	init();
	while (1) {
		int i, j;
		unsigned char const *arr[11] = {0};

		for (i = 0; i < 4; i++) {
			arr[i] = &ch[11 + i][0];
		}
		for (i = 6; i < 11; i++, num /= 10) {
			arr[6 + i] = &ch[num % 10][0];
		}

		for (i = 0; i < 10; i++)
			arr[i] = &ch[i][0];
		for (i = 0; i < COLUMNS; i++) {
			for (j = 4; j >= 0; j--) {
				send_data(*(arr[cnt + j] + 2 * i + 1), COLUMNS - 1 - i, j == 0 && offset > 8 ? offset - 8 : 0);
				send_data(*(arr[cnt + j] + 2 * i), COLUMNS - 1 - i, (j == 0) * offset);
			}
			while (time % 4) {}
		}

		if (time >= 100) {
			time = 0;
			if (++num % 100 >= 60) {
				num += 40;
				num %= 6000;
			}
			if (++offset >= 16) {
				offset = 0;
				if (++cnt == CH_COUNT)
					cnt = 4;
			}
		}

	}
	return 0;
}
