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

void send_ch(const unsigned char *first, const unsigned char *last, int len,
             int begin, int end, int col, int offset)
{
	const unsigned char *it = first + begin * len,
	                     *it_end = first + end * len;

	if (len < 0) {
		len = -len;
		it = first + end * len - len;
		it_end = first + begin * len - len;
		len = -len;
	}

	for (; it != it_end; it += len) {
		unsigned char left = 0, right = 0;
		int left_skip = 0, right_skip = 0;

		if (it >= first && it < last) {
			right = *(it + col * 2 + 1);
			left = *(it + col * 2);
		}

		if (it + len == it_end) {
			int half = COLUMNS >> 1;
			if (len < 0) {
				left_skip = offset;
				if (offset > half)
					right_skip = offset - half;
			} else {
				left_skip = COLUMNS - offset;
				if (offset < half)
					right_skip = half - offset;
			}
		}

		send_data(right, COLUMNS - 1 - col, right_skip);
		send_data(left, COLUMNS - 1 - col, left_skip);
	}
	while (time % 4) {}
}

int main(void)
{
	int cnt = -4;
	int tmp = 0;
	int offset = 0;
	init();
	while (1) {
		int i;
		const unsigned char *first = (const unsigned char *)ch,
		                     *last = (const unsigned char *)(ch + CH_COUNT);
		for (i = 0; i < COLUMNS; i++) {
			send_ch(first, last, -32, cnt, cnt + 5, i, offset);
			//send_ch(first, last, 32, cnt, cnt + 5, i, offset);
		}
		if (time > 10) {
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
