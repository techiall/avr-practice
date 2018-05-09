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
#define CH_COUNT 17

void send_ch(int col, int begin, int offset)
{
	int i;
	for (i = 4; i >= 0; i--) {
		unsigned char left = 0, right = 0;
		int ch_idx = (begin + i);

		if (ch_idx >= 0 && ch_idx < CH_COUNT) {
			right = ch[ch_idx][col * 2 + 1];
			left = ch[ch_idx][col * 2];
		}

		send_data(right, i == 0 && offset > 8 ? offset - 8 : 0);
		send_data(left, (i == 0) * offset);
	}

	while (time % 4) {}
	PORTD = COLUMNS - 1 - col;
}

int main(void)
{
	int cnt = -4;
	int tmp = 0;
	int offset = 0;
	init();
	while (1) {
		int i, j;
		for (i = 0; i < COLUMNS; i++) {
			send_ch(i, cnt, offset);
		}

		if (time > 100) {
			if (++offset == 16) {
				offset = 0;
				if (++cnt == CH_COUNT)
					cnt = -4;
			}
			time = 0;
		}

	}
	return 0;
}
