#ifndef _LED_DISPLAY_H_
#define _LED_DISPLAY_H_

#include "define.h"

void send_data(unsigned char data)
{
	int j = 0;
	for (j = 7; j >= 0; j--) {
		PORTA = !!(data & (1 << j));
		PORTB |= (1 << 1);
		PORTB &= ~(1 << 1);
	}
	PORTB |= (1 << 0);
	PORTB &= ~(1 << 0);
}

void display_digit(char pos, char digit, unsigned long *time)
{
	static const unsigned char led[] = {
		0x3F, 0x06, 0x5B, 0x4F, 0x66,
		0x6D, 0x7D, 0x07, 0x7F, 0x6F,
		0x40, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
	};
	static unsigned char size = sizeof(led) / sizeof(*led);
	unsigned long t = *time;

	send_data(~led[digit]);
	PORTD &= ~(1 << (4 + pos));
	while (t == *time);
	PORTD |= (1 << (4 + pos));
}

void display_number(long number, unsigned char digit,
                    unsigned char mask, unsigned long *time)
{
	int i;

	if (number < 0) {
		digit--;
		display_digit(digit, 10, time);
	}

	for (i = 0; i < digit; i++, number /= 10) {
		if (mask & (1 << i))
			continue;
		display_digit(i, abs(number % 10), time);
	}
}

#endif /* _LED_DISPLAY_H_ */
