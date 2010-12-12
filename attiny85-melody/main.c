#include <inttypes.h>		// uint8_t usw.
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>	// Interrupts
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/delay.h>		// definiert _delay_ms und _delay_loop_2 
#include "lied.h"           //Melodien, Offsets und Tonh�hen

#define adc_ein   (1<<ADATE)|(1<<ADEN)|(1<<ADIE)|7
#define adq       (1<<ADLAR)|(1<<MUX0)

#define avt0      10
#define leiser    3
#define busy      7
#define pwm_ein   (1<<PWM1A)|(1<<COM1A1)|(1<<CS10)
#define spk       PB1

#define idle      1<<SE
#define down      idle|(1<<SM1)

char tfl = 0;
char aus = 0;
char leis = 0;
char laut = 0;
char tonein = 0;
char avt = 0;
char dauer = 0;

volatile char x = 0;

ISR( PCINT0_vect)
{
};

ISR( INT0_vect)
{
};

ISR(TIMER0_OVF_vect)
{
  x = !x;
  OCR1A = 0x80;
  laut = 0xff;
  dauer = 20;
}


ISR(ADC_vect)
{
  ADCSRA |= adc_ein;
  avt--;

  if(!avt)
  {
    avt = avt0;
    if(laut)
    {
  //   avt = leis;
     laut--;
    }
    if(!dauer)
    {
      OCR1A = 0;
     return;
    }
    dauer--;
    OCR1A = (0x80 * laut) >> 8;
  }
}

void tiefschlaf ()
{
  MCUCR = down;
  GIFR = 1<<INT0;
  GIMSK = 1<<INT0;
  TCCR1 = 0;
}

int
main()
{
  wdt_reset();

  avt = avt0;
  ADMUX = adq;
  leis = leiser;

  TIMSK = (1 << TOIE0); //Interrupts auf Comp Wert A zulassen
  OCR1B = 0xff;

  PORTB = (1<<PB2);
  DDRB |= (1<<PB1) | (1<<PB0);

  GIMSK = (1 << INT0);
  x = !x;
  aus = 0;

  tfl |= (1<<busy);
  MCUCR = idle;
  GIMSK = 0;
  OCR1A = 0x80;
  TCCR1 = pwm_ein;
  TCCR0B = 1;


  dauer = 30;
  leis = 4;
  laut = 0xff;

//  MCUCR = down;

  ACSR |= 1<<ACD;
  ADCSRA |= adc_ein;

  sei();

  ADCSRA |= (1<<ADSC);

  for (;;)
  {
    if(x)
    {
      PORTB |= 1<<PB0;
    }
    else
    {
      PORTB &= ~(1<<PB0);
    }
    continue;
    if( (tfl & (1<<busy)) == 0)
    {
      if( (PINB & (1<<PB2)) == 0)
      {

        tfl |= (1<<busy);
//        MCUCR = idle;
//        GIMSK = 0;
        OCR1A = 0x30;
        TCCR1 = pwm_ein;
        TCCR0A = tonein;


        dauer = 10;
        leis = 4;
        laut = 0xff;
      }

      aus--;
//      tiefschlaf();
//      sleep_cpu();
      continue;
    }
    aus = 0;
//    sleep_cpu();
  } //for


} //main










//uint8_t getcap(uint8_t pin, uint8_t sampletime, uint8_t waittime) {
// DDRB &= ~pin;
// while (waittime--) {
// __asm__ ("nop");
// }
// while (!(PINB & pin) && --sampletime)
// ;
// DDRB |= pin;
// return sampletime;
//}
//
//ISR(TIMER1_OVF_vect)
//{
// OCR1A = 0x80;
//}
//
//int main() {
//
// // while (1) {
// // int i = 0;
// // int n = 11;
// // while (--n) {
// // i += getcap(1 << PB3, 50, 40);
// // }
// // i /= 10;
// // if (i < 42) {
// // PORTB |= (1 << PB0);
// // } else if (i > 45) {
// // PORTB &= ~(1 << PB0);
// // }
// // _delay_ms(100);
// // }
//
// wdt_reset();

