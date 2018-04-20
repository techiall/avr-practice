/*
imakew -f FIRST.mak
*/
#include <iom16v.h>
#include <macros.h>
#include <math.h>
#include "define.h"
#include "led_send_display.h"
#include "key.h"
#include "init_avr.h"

unsigned long time = 0;
long num = 0;
unsigned char MASK = 0;
unsigned char mask = (3 << 2 * 2);

#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{
	time++;

	if (time >= TIME_1S / 2) {
			MASK = mask == MASK ? 0 : mask;
		}
	if (time >= TIME_1S) {
		if (mask == (3 << 2 * 2))
			num++;
		time = 0;
	}

	if (num % 100 >= 60)
		num += 40;
	num %= 6000;

}

int _1(void *first, void *last)
{
	long *num = first;
	unsigned char *x = last;
	if (*x == 3) {
		++*num;
		*num = *num / 100 * 100 + *num % 100 % 60;
	} else if (*x == (3 << 2)) {
		*num += 100;
		*num = *num / 100 % 60 * 100 + *num % 100;
	}
	return 0;
}

int mask_move(void *first, void *last)
{
	unsigned char *x = last;
	*x <<= 2;
	if (*x == (3 << (2 + 1) * 2))
		*x = 3;
	return 0;
}

int main(void)
{
	int i;
	struct key key[2];
	unsigned char key_num = sizeof(key) / sizeof(*key);
	key[0] = key_init(&PINA, 0x80, ~(1 << 7), 0, 0, mask_move, 0, mask_move);
	key[1] = key_init(&PIND, 0x08, ~(1 << 3), 0, 0, _1, 0, _1);

	init();
	while(1) {
		display_number(num, 4, mask ^ MASK, &time);
		for (i = 0; i < key_num; i++) {
			if (key_is_down(key[i])) {
				key[i] = key_state_move(key[i], &time);
				key_operate(key[i], &num, &mask);
			}
			 display_number(num, 4, mask ^ MASK, &time);
		}
	}

	return 0;
}
