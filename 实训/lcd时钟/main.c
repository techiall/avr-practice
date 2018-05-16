/*
   imakew -f FIRST.mak
*/

#include <iom16v.h>
#include <macros.h>
#include "ku.h"

void port_init(void)
{
	PORTA = 0x00;
	DDRA  = 0x00;

	PORTB = 0x0f;
	DDRB  = 0x00;

	PORTC = 0x00;
	DDRC  = 0x0F;

	PORTD = 0x00;
	DDRD  = 0x0F;
}

void time_init(void)
{
	TCCR0 = 0x00;
	TCNT0 = 0xC2;
	OCR0  = 0x3E;
	TCCR0 = 0x0B;
}

void init(void)
{
	CLI();

	port_init();
	time_init();

	MCUCR = 0x00;
	GICR  = 0x00;
	TIMSK = 0x02;
	SEI();
}

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

unsigned long time = 0;
#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{
	time++;
}

#define COLUMNS 16
#define CH_COUNT 17

void send_num(int num, int len, int offset)
{
	int j;
	for (j = 0; j < 4; j++, num /= 10) {
		send_data(ch[num % 10 - 1 == -1 ? 9 : num % 10 - 1][2 * offset + 1], 0);
		send_data(ch[num % 10 - 1 == -1 ? 9 : num % 10 - 1][2 * offset], 0);
	}
	while (time % 6) {}
	PORTD = COLUMNS - 1 - offset;
}

int main(void)
{
	unsigned long num = 0;
	int i, j;
	unsigned char flag = 1;
	init();
	while (1) {
		int i;
		for (i = 0; i < COLUMNS; i++)
			send_num(num, 4, i);
		while (~PINB & (1 << 1)) {
			for (i = 0; i < COLUMNS; i++)
				send_num(num, 4, i);
			if (~PINB & (1 << 1))
				continue;
			flag ^= 1;
		}

		if (time >= 1000) {
			time = 0;
			num += flag;
			if (num % 100 >= 60) {
				num += 40;
				num %= 6000;
			}
		}
	}
	return 0;
}
