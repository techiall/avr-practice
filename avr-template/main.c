/*
imakew -f FIRST.mak
*/
#include <iom16v.h>
#include <macros.h>

const unsigned char led[] = {
        0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
        0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};

void init(void)
{
	CLI();
	PORTA = 0x01;
	DDRA  = 0x01;
	
	PORTB = 0x03;
	DDRB  = 0x03;
	
	PORTD = 0xF0;
	DDRD  = 0xF0;

	TCCR0 = 0x00;
	TCNT0 = 0x83;
	OCR0  = 0x7D;
	TCCR0 = 0x0B;

	MCUCR = 0x00;
	GICR  = 0x00;
	TIMSK = 0x02; 
	SEI();
}

unsigned int i = 0;
unsigned char flag = 0, light = 0;

#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{
	i = (i + 1) % 101;
	flag = i == 100 ? 1 : flag;
	light = i % 2 ? light : 1;
}

void send_data(unsigned char data)
{
	int j = 0;
	for (j = 7; j >= 0; j--) {
		PORTA = data & (1 << j) ? PORTA | (1 << 0) : PORTA & ~(1 << 0);
		PORTB |= (1 << 1);
		PORTB &= ~(1 << 1);
	}
	PORTB |= (1 << 0);
	PORTB &= ~(1 << 0);
}

void main(void)
{
	unsigned int num = 0;
	unsigned int time[4] = {0};
	char k = 0;
	int tmp = 0;
	
	init();
	while(1) {
		if(flag) {
			flag = 0;
			if (++num % 100 >= 60) {
				num = (num + 100) % 10000;
				num -= num % 100;
			}
			for (tmp = num, k = 0; k < 4; k++, tmp /= 10)
				time[4 - 1 - k] = tmp % 10;
		}
		for(k = 0; k < 4; k++) {
			send_data(~led[time[k]]);
			PORTD &= ~(1 << (7 - k));
			while(!light);
			light = 0;
			PORTD |= (1 << (7 - k));
		}

	}
}