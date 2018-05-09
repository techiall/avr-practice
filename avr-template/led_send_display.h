#ifndef _LED_DISPLAY_H_
#define _LED_DISPLAY_H_

#include "define.h"

#define CLR_SHCLK() PORTC &= ~(1 << 0)
#define SET_SHCLK() PORTC |= (1 << 0)

#define CLR_STCLK() PORTC &= ~(1 << 1)
#define SET_STCLK() PORTC |= (1 << 1)

#define CLR_DS() PORTC &= ~(1 << 3)
#define SET_DS() PORTC |= (1 << 3)

#define OE_L() PORTC &= ~(1 << 2)
#define OE_H() PORTC |= (1 << 2)

#define SEND() CLR_STCLK(); SET_STCLK()

void send_data(unsigned char data, int skip)
{
	int i;
	for (i = 0; i < 8 - skip; i++) {
		if (data & (1 << (7 - i))) {
			SET_DS();
		} else {
			CLR_DS();
		}
		CLR_SHCLK();
		SET_SHCLK();
	}
	SEND();
}


void display_digit(char pos, char digit, unsigned long *time)
{
	static unsigned char led[] = {
		0x3F, 0x06, 0x5B, 0x4F, 0x66,
		0x6D, 0x7D, 0x07, 0x7F, 0x6F,
		0x40, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
	};
	static int size = sizeof(led) / sizeof(*led);
	unsigned long t = *time;

	send_data(~led[digit], 0);
	PORTD &= ~(1 << (4 + pos));
	while (t == *time);
	PORTD |= (1 << (4 + pos));
}

void display_number(long number, int digit,
		int mask, unsigned long *time)
{
	int i;
	if (number < 0) {
		digit--;
		display_digit(digit, 10, time);
	}

	for (i = 0; i < digit; i++, number /= 10) {
		if (mask & (1 << i))
			continue;
		display_digit(i, number % 10, time);
	}
}

#endif /* _LED_DISPLAY_H_ */
