/*
imakew -f FIRST.mak
*/
#include <iom16v.h>
#include <macros.h>

const unsigned char led[] = {
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
	0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};

void delay(unsigned int time)
{
	while (time--) {
		int i = 0;
		for (i = 0; i < 100; i++);
	}
}

void set_number(int number, int digits)
{
	int i;
	for (i = digits - 1; i >= 0; i--, number /= 10) {
		PORTC = (1 << i);
		PORTB = ~led[number % 10];
		delay(4);
	}
}

void init(void)
{
	DDRB = 0xFF;
	PORTB = 0xFF;

	DDRC = 0x00;
	PORTC = 0x00;

	DDRD &= ~(1 << 3);
	PORTD |= (1 << 3);
}

int main(void)
{
	int number = 0;
	init();
	while (1) {
		set_number(number, 4);
		if (!(PIND & (1 << 3)))  {
			while (!(PIND & (1 << 3))) {
				set_number(number, 4);	
			}
			number++;
		}
	}
	return 0;
}