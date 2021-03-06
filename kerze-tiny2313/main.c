#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <stdlib.h>

# define cli()  __asm__ __volatile__ ("cli" ::)
# define sei()  __asm__ __volatile__ ("sei" ::)

void InitPWM()
{
   /*
   TCCR0 - Timer Counter Control Register (TIMER0)
   -----------------------------------------------
   BITS DESCRIPTION


   NO:   NAME   DESCRIPTION
   --------------------------
   BIT 7 : FOC0   Force Output Compare [Not used in this example]
   BIT 6 : WGM00  Wave form generartion mode [SET to 1]
   BIT 5 : COM01  Compare Output Mode        [SET to 1]
   BIT 4 : COM00  Compare Output Mode        [SET to 0]

   BIT 3 : WGM01  Wave form generation mode [SET to 1]
   BIT 2 : CS02   Clock Select               [SET to 0]
   BIT 1 : CS01   Clock Select               [SET to 0]
   BIT 0 : CS00   Clock Select               [SET to 1]

   The above settings are for
   --------------------------

   Timer Clock = CPU Clock (No Prescalling)
   Mode        = Fast PWM
   PWM Output  = Non Inverted

   */


   TCCR1A |= (1<<WGM10)|(1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);
   TCCR1B |= (1<<WGM12) | (1<<CS12) | (1<<CS10);
   DDRB|= (1<<PB3)|(1<<PB4);
}


void SetPWMOutput(uint8_t duty)
{
  OCR1A=duty;
  OCR1B=255-duty;
}

void sleep()
{
  wdt_reset();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();

  sleep_cpu ();
  sleep_disable ();
  wdt_reset();
}

int main()
{
   uint16_t brightness=0;

   InitPWM();

   cli();
   wdt_reset();
   wdt_enable (WDTO_15MS);
   WDTCSR |= _BV(WDIE) | _BV(WDP2) | _BV(WDP1) | _BV(WDP0);
   sei();

   while(1)
   {
     int8_t r = (rand() & 0x20) - 10;
     brightness += r;
     if (brightness < 0x10)
       brightness = 0xff;
     SetPWMOutput (brightness & 0xff);
     sleep();
   }
   return 0;
}
