#ifndef _INIT_AVR_H_
#define _INIT_AVR_H_

void port_init(void)
{
	PORTA = 0x00;
	DDRA  = 0x00;

	PORTB = 0x00;
	DDRB  = 0x00;

	PORTC = 0x00;
	DDRC  = 0x0F;

	PORTD = 0x00;
	DDRD  = 0x0F;
}
void uart0_init(void)
{
	UCSRB = 0x00;
	UCSRA = 0x00;
	UCSRC = BIT(URSEL) | 0x06;
	UBRRL = 0x2F;
	UBRRH = 0x00;
	UCSRB = 0x18;
}

void spi_init(void)
{
	SPCR = 0x50;
	SPSR = 0x00;
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
	//spi_init();
	//uart0_init();
	time_init();

	MCUCR = 0x00;
	GICR  = 0x00;
	TIMSK = 0x02;
	SEI();
}

#endif /* _INIT_AVR_H_ */
