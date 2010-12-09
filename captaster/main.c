#include <stdlib.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(a) (1 << a)

#ifndef cbi
#define cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))
#endif


enum { UP, DOWN };

ISR (TIMER1_OVF_vect)           /* Note [2] */
{
    static uint16_t pwm;        /* Note [3] */
    static uint8_t direction;

    switch (direction)          /* Note [4] */
    {
        case UP:
            if (++pwm == 0xff)
                direction = DOWN;
            break;

        case DOWN:
            if (--pwm == 0)
                direction = UP;
            break;
    }

    OCR1A = pwm;                  /* Note [5] */
}

void
ioinit (void)                   /* Note [6] */
{
    /* Timer 1 is 10-bit PWM (8-bit PWM on some ATtinys). */
    TCCR1 = _BV(PWM1A) | _BV(COM1A1) | _BV(CS12);

    /* Set PWM value to 0. */
    OCR1A = 0;

    /* Enable OC1 as output. */
    DDRB = _BV (PB1) | _BV (PB0);

    /* Enable timer 1 overflow interrupt. */
    TIMSK = _BV (TOIE1);
    sei ();
}

int
main (void)
{

    ioinit ();

    /* loop forever, the interrupts are doing the rest */

    for (;;)                    /* Note [7] */
        sleep_mode();

    return (0);
}


//uint8_t getcap(uint8_t pin, uint8_t sampletime, uint8_t waittime) {
//	DDRB &= ~pin;
//	while (waittime--) {
//		__asm__ ("nop");
//	}
//	while (!(PINB & pin) && --sampletime)
//		;
//	DDRB |= pin;
//	return sampletime;
//}
//
//ISR(TIMER1_COMPA_vect)
//{
//	OCR0A = rand() & 0xff;
//}
//
//int main() {
//	//	while (1) {
//	//		int i = 0;
//	//		int n = 11;
//	//		while (--n) {
//	//			i += getcap(1 << PB3, 50, 40);
//	//		}
//	//		i /= 10;
//	//		if (i < 42) {
//	//			PORTB |= (1 << PB0);
//	//		} else if (i > 45) {
//	//			PORTB &= ~(1 << PB0);
//	//		}
//	//		_delay_ms(100);
//	//	}
//
//	PLLCSR = 0b00000110;
//	GTCCR  = 0b01100000;
//	TCCR1  = 0b01100001;
//	TCCR0A = 0b00000010;
//	TCCR0B = 0b00000010;
//	TIMSK  = _BV(OCIE1A);
//
//	OCR0A = rand() & 0xff;
//	OCR1A = 0x80;
//
//	while (1) {
////		PORTB |= (1 << PB0);
////		_delay_ms(rand() / 60000);
////		PORTB &= ~(1 << PB0);
////		_delay_ms(rand() / 60000);
//	}
//
//}
