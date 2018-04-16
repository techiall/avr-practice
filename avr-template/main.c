/*
imakew -f FIRST.mak
*/
#include <iom16v.h>
#include <macros.h>

void port_init(void)
{
	PORTA = 0x00;
	DDRA  = 0xFF;

	PORTC = 0xC0;
	DDRC  = 0x3F;
}

void timer0_init(void)
{
	TCCR0 = 0x00;
	TCNT0 = 0x8;
	OCR0  = 0x7D;
	TCCR0 = 0x0B;
}

#define TIME_1S  100
unsigned long time = 0;
#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{
	time++;
}

enum KEY_STATE {
	NONE, DOWN, PRESS, LONG_PRESS, KEY_STATE_NUM,
};

struct key {
	int state;
	unsigned long press_time;
	int (*is_down)(void);
	int (*operate[KEY_STATE_NUM])(void *first, void *last);
};

struct key key_state_move(struct key cur_key, int key_down)
{
	static int move[KEY_STATE_NUM][2] = {
		NONE, DOWN,
		NONE, PRESS,
		NONE, LONG_PRESS,
		NONE, PRESS,
	};

	if (cur_key.state == PRESS) {
		if (time - cur_key.press_time >= 1 * TIME_1S)
			cur_key.state = move[cur_key.state][key_down];
	} else {
		if (cur_key.state == DOWN)
			cur_key.press_time = time;
		else if (cur_key.state == LONG_PRESS)
			cur_key.press_time = time - TIME_1S / 2;
		cur_key.state = move[cur_key.state][key_down];
	}	

	return cur_key;	
}

void init_devices(void)
{
	CLI();
	port_init();
	timer0_init();

	MCUCR = 0x00;
	GICR  = 0x00;
	TIMSK = 0x02;
	SEI();
}

void display_digit(int pos, unsigned char digit)
{
	static const unsigned char led[] = {
		0x3F, 0x06, 0x5B, 0x4F, 0x66,
	       	0x6D, 0x7D, 0x07, 0x7F, 0x6F,
	       	0x40, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71,
	};
	PORTC = ~(1 << pos);
	PORTA = led[digit];
}

void display_number(long number, unsigned char digit, unsigned char mask)
{
	char i;

	if (number < 0) {
		digit--;
		display_digit(digit, 10);
	}

	for (i = 0; i < digit; i++, number /= 10) {
		if (mask & (1 << i))
			continue;
		display_digit(i, abs(number % 10));
	}
}

int key_is_down(char *keys)
{
	int and = 0;
	for (; *keys; keys++)
		and |= 1 << (*keys - '0');
	return (~PINC & 0xc0) == (and << 5);
}

int _1(void)
{ return key_is_down("2"); }

int __1(void *first, void *last)
{ return 1; }

int _2(void)
{ return key_is_down("1"); }

int __2(void *first, void *last)
{ return -1; }

int _3(void)
{ return key_is_down("12"); }

int __3(void *first, void *last)
{ return 1000; }

int main(void)
{
	unsigned char len = 6;
	long num = 0;
	struct key key[3] = {
		{0, 0, _1, {0, __1, 0, __1}},
		{0, 0, _2, {0, __2, 0, __2}},
		{0, 0, _3, {0, __3, 0, __3}},
	};
	int key_num = sizeof(key) / sizeof(*key);
	int i;

	init_devices();
	while(1) {
		for (i = 0; i < key_num; i++) {
			key[i] = key_state_move(key[i], key[i].is_down());
			if (key[i].operate[key[i].state])
				num += key[i].operate[key[i].state](0, 0);
		}
		display_number(num, len, 0x00);
	}
	return 0;
}
