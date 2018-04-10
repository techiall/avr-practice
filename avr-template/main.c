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
        PORTA = 0x00;
        DDRA  = 0x01;
        
        PORTB = 0x00;
        DDRB  = 0x03;
        
        PORTD = 0x00;
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

#pragma interrupt_handler timer0_comp_isr:iv_TIM0_COMP
void timer0_comp_isr(void)
{
        i++;
}

void send_data(unsigned char data)
{
        int j = 0;
        for (j = 7; j >= 0; j--) {
                PORTA = data & (1 << j) 
                        ? PORTA | (1 << 0) 
                        : PORTA & ~(1 << 0);
                PORTB |= (1 << 1);
                PORTB &= ~(1 << 1);
        }
        PORTB |= (1 << 0);
        PORTB &= ~(1 << 0);
}

void display(unsigned int num)
{
        int j;
        int tmp = num % 2 == 1;
        for (j = 0; j < 4; j++, num /= 10) {
                send_data((tmp && j == 2) 
                        ? ~led[num % 10] & ~(1 << 7) 
                        : ~led[num % 10]);
                PORTD &= ~(1 << (4 + j));
                while(i % 1);
                PORTD |= (1 << (4 + j));
        }
}

int main(void)
{
        unsigned int num = 00;
        init();
        while(1) {
                if (i == 100) {
                        i = 0;
                        if (++num % 100 == 60)
                                num += 40;
                        num %= 6000;
                }
                display(num);
        }
        return 0;
}