/**********************************************************
 * Copyright(C) 2007 Jochen Roessner <jochen@lugrot.de>
 * Copyright(C) 2007, 2008 Stefan Siegl <stesie@brokenpipe.de>
 *
 * @author      Benedikt K.
 * @author      Juergen Eckert
 * @author    Ulrich Radig (mail@ulrichradig.de) www.ulrichradig.de
 * @date        04.06.2007

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

 */

#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "core/spi.h"
#include "core/heartbeat.h"
#include "protocols/uip/uip.h"
#include "protocols/uip/uip_router.h"
#include "rfm12.h"
#include "rfm12_raw_net.h"
#include "core/bit-macros.h"

#ifdef DEBUG
#define RFM12_DEBUG(s, args...) printf_P(PSTR("D: " s), ## args)
#else
#define RFM12_DEBUG(a...)
#endif

#define RF12FREQ868(freq)       ((unsigned short)((freq-860.0)/0.005))                  // macro for calculating frequency value out of frequency in MHz for 868MHz Band
#define RF_PORT   PORTC
#define RF_DDR    DDRC
#define RF_PIN    PINC

#define SDI   0
#define SCK	   1
#define CS    2
#define SDO   3
#define FSK   4

#ifndef cbi
#define cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit)) 
#endif
#ifndef sbi
#define sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))  
#endif

uint8_t rfm12_bandwidth = RxBW134;
uint8_t rfm12_gain = LNA_6;
uint8_t rfm12_drssi = RSSI_79;

unsigned short rf12_trans(unsigned short wert) {
  unsigned short werti = 0;
  unsigned char i;

  cbi(RF_PORT, CS);
  for (i = 0; i < 16; i++) {
    if (wert & 32768)
      sbi(RF_PORT, SDI);
    else
      cbi(RF_PORT, SDI);
    werti <<= 1;
    if (RF_PIN & (1 << SDO))
      werti |= 1;
    sbi(RF_PORT, SCK);
    wert <<= 1;
    _delay_us(0.3);
    cbi(RF_PORT, SCK);
  }
  sbi(RF_PORT, CS);
  return werti;
}

void rf12_init(void) {
  RF_DDR = (1 << SDI) | (1 << SCK) | (1 << CS) | (1 << FSK);

  sbi(RF_PORT, CS);
  sbi(RF_PORT, FSK);

  for (unsigned char i = 0; i < 10; i++)
    _delay_ms(10); // wait until POR done

  rf12_trans(0xC0E0); // AVR CLK: 10MHz
  rf12_trans(0x80E7); // Enable FIFO and 868
  rf12_trans(0xC2AB); // Data Filter: internal
  rf12_trans(0xCA81); // Set FIFO mode
  rf12_trans(0xE000); // disable wakeuptimer
  rf12_trans(0xC800); // disable low duty cycle
  rf12_trans(0xC4F7); // AFC settings: autotuning: -10kHz...+7,5kHz

  rfm12_setfreq(RF12FREQ868(868.3));
  rfm12_setbandwidth(4, 1, 4);
  rfm12_setbaud(666);
  rfm12_setpower(0, 6);

}

void rfm12_setbandwidth(unsigned char bandwidth, unsigned char gain, unsigned char drssi) {
  rf12_trans(0x9400 | ((bandwidth & 7) << 5) | ((gain & 3) << 3) | (drssi & 7));
}

void rfm12_setfreq(unsigned short freq) {
  if (freq < 96) // 430,2400MHz
    freq = 96;
  else if (freq > 3903) // 439,7575MHz
    freq = 3903;
  rf12_trans(0xA000 | freq);
}

void rfm12_setbaud(unsigned short baud) {
  if (baud < 663)
    return;
  if (baud < 5400) // Baudrate= 344827,58621/(R+1)/(1+CS*7)
    rf12_trans(0xC680 | ((43104 / baud) - 1));
  else
    rf12_trans(0xC600 | ((344828UL / baud) - 1));
}

void rfm12_setpower(unsigned char power, unsigned char mod) {
  rf12_trans(0x9800 | (power & 7) | ((mod & 15) << 4));
}

unsigned char rf12_ready(unsigned char bTimeout) {
  int timeout = 10000;

  cbi(RF_PORT, SDI);
  cbi(RF_PORT, CS);
  asm( "nop" );
  while (!(RF_PIN & (1 << SDO)) && timeout) {
    if (bTimeout) {
      timeout--;
      _delay_us(1); // wait until FIFO ready
    }
  }
  sbi(PORTB, CS);
  if (timeout == 0)
    return 0;
  else
    return 1;
}

unsigned rf12_data(void) {
  //	int res = rf12_trans(0x0000);
  //	char bOk = ((res) & 0x0200) == 0;
  //	//	printstatus(res);
  //	return bOk;
  cbi(RF_PORT, CS);
  cbi(RF_PORT, SDI);
  asm("nop");
  if (RF_PIN & (1 << SDO))
    return 1;
  else
    return 0;
}

void rf12_txdata(char *data, unsigned char number) {
  unsigned char i;

  sbi(RF_PORT, FSK);

  rf12_trans(0x8238); // TX on
  rf12_ready(0);
  rf12_trans(0xB8AA);
  rf12_ready(0);
  rf12_trans(0xB8AA);
  rf12_ready(0);
  rf12_trans(0xB8AA);
  rf12_ready(0);
  rf12_trans(0xB82D);
  rf12_ready(0);
  rf12_trans(0xB8D4);
  rf12_ready(0);
  rf12_trans(0x0000);
  for (i = 0; i < number; i++) {
    rf12_ready(0);
    rf12_trans(0xB800 | (*data++));
  }
  rf12_ready(0);
  rf12_trans(0x8208); // TX off
}

void rf12_rxdata(char *data, unsigned char number) {
  unsigned char i;
  rf12_trans(0x82C8); // RX on
  rf12_trans(0xCA81); // set FIFO mode
  rf12_trans(0xCA83); // enable FIFO
  for (i = 0; i < number; i++) {
    rf12_ready(0);
    *data++ = rf12_trans(0xB000);
  }
  rf12_trans(0x8208); // RX off
}

unsigned char rf12_rxbyte(char *data) {
  int t = rf12_ready(1);
  if (!t) {
    return 0;
  }
  *data = rf12_trans(0xB000);
  return 1;
}

void enable_rx() {
  rf12_trans(0x82C8); // RX on
  _delay_ms(10);
  rf12_trans(0xCA81); // set FIFO mode
  _delay_ms(10);
  rf12_trans(0xCA83); // enable FIFO
  _delay_ms(10);
}

void disable_rx() {
  rf12_trans(0x8208); // RX off
}

unsigned int rf12_rxdata_if_available(char *data, int *res) {
  //	*res = 0xaaaa;
  //	if (rf12_data())
  //	{
  //		*data = rf12_trans(0xB000);
  //		return 1;
  //	}
  *res = rf12_trans(0x0000);
  char bOk = ((*res) & 0x0200) == 0;
  if (bOk) {
    *data = rf12_trans(0xB000);
  }
  return (bOk);
}

volatile char *g_buf;
volatile int g_len;
volatile uip_udp_conn_t *g_conn;

void udpsend(char *buf, int len)
{
  g_buf = buf;
  g_len = len;
  uip_udp_periodic_conn(g_conn);
  router_output();
}



//    static uip_udp_conn_t *conn, *old_conn;
//    uip_ipaddr_t addr;
//    unsigned char *old_uip_appdata = uip_appdata;
//
//    uip_ipaddr(&addr, 10,1,0,2);
//    conn = uip_udp_new(&addr, HTONS(12345), NULL);
//
//    if (conn) {
////      uip_ipaddr_t ip;
////      set_CONF_ENC_IP(&ip);
////      uip_sethostaddr(&ip);
//
//      old_conn = uip_udp_conn;
//      uip_udp_conn = conn;
//      uip_appdata = &uip_buf[UIP_LLH_LEN + UIP_IPUDPH_LEN];
//      memcpy(uip_appdata, buf, size);
//      uip_udp_send(size);
//      uip_process(UIP_UDP_SEND_CONN);
//
//      memcpy(uip_appdata, buf, size);
//      RFM12_DEBUG ("sending %d bytes\n", size);
//      router_output_to(STACK_ENC);
//
//      uip_udp_remove(conn);
//      uip_appdata = old_uip_appdata;
//      uip_udp_conn = old_conn;
//    }
//    else
//    {
//      RFM12_DEBUG ("no conn\n");
//    }

void rfm12_int_process(void) {
  uint16_t status = rf12_trans(0x0000);

  if (status & 0x4000) {
    RFM12_DEBUG ("rfm12/por -> init.\n");
    rfm12_init();
    return;
  }

  if (status & 0x2000) {
    RFM12_DEBUG ("rfm12/overflow -> init.\n");
    rfm12_init();
    return;
  }

  if ((status & 0x8000) == 0) {
    //    return;
  }

  if (0) {
    int st2 = status;
    char bits[16];
    for (int xc = 0; xc < 16; xc++) {
      bits[xc] = st2 & 1;
      st2 >>= 1;
    }
    RFM12_DEBUG ("status 0b%d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d\n",bits[15],bits[14],bits[13],bits[12],bits[11],bits[10],bits[9],bits[8],bits[7],bits[6],bits[5],bits[4],bits[3],bits[2],bits[1],bits[0]);
  }

  char bOk = (status & 0x0200) == 0;
  //  bOk = rf12_ready(1);
  if (bOk) {
    char buf[64];
    unsigned int x = 0;
    unsigned int size = 5;
    while(1) {
      buf[x] = 0;
      int res = rf12_rxbyte(buf + x);
      if (!res)
      {
      }
      switch (buf[4]) {
      case 'M':
        size = 11;
        break;
      case 'e':
        size = 9;
        break;
      case 'f':
        size = 19;
        break;
      case 'T':
        size = 25;
        break;
      case 'g':
        size = 19;
        break;
      default:
        size = 6;
        break;
      }
      x++;
      if(x>=size)
        break;
    }

    disable_rx();
    _delay_ms(1);
    enable_rx();

    udpsend(buf, size);
//    static uip_udp_conn_t *conn, *old_conn;
//    uip_ipaddr_t addr;
//    unsigned char *old_uip_appdata = uip_appdata;
//
//    uip_ipaddr(&addr, 10,1,0,2);
//    conn = uip_udp_new(&addr, HTONS(12345), NULL);
//
//    if (conn) {
////      uip_ipaddr_t ip;
////      set_CONF_ENC_IP(&ip);
////      uip_sethostaddr(&ip);
//
//      old_conn = uip_udp_conn;
//      uip_udp_conn = conn;
//      uip_appdata = &uip_buf[UIP_LLH_LEN + UIP_IPUDPH_LEN];
//      memcpy(uip_appdata, buf, size);
//      uip_udp_send(size);
//      uip_process(UIP_UDP_SEND_CONN);
//
//      memcpy(uip_appdata, buf, size);
//      RFM12_DEBUG ("sending %d bytes\n", size);
//      router_output_to(STACK_ENC);
//
//      uip_udp_remove(conn);
//      uip_appdata = old_uip_appdata;
//      uip_udp_conn = old_conn;
//    }
//    else
//    {
//      RFM12_DEBUG ("no conn\n");
//    }

//    switch (buf[4]) {
//    case 'T': {
//      unsigned int temp;
//      memcpy(&temp, buf + 5, 2);
//      char *id = buf + 7;
//      RFM12_DEBUG ("%c%c%c%c %2x:%2x:%2x:%2x:%2x:%2x:%2x:%2x  %4d.%01d C\n", buf[0], buf[1], buf[2], buf[3], id[0], id[1], id[2], id[3], id[4], id[5], id[6], id[7], temp >> 4, (temp << 12) / 6553 );
//    }
//      break;
//    case 'g': {
//      signed int x, y, z;
//      memcpy(&x, buf + 5, 2);
//      memcpy(&y, buf + 7, 2);
//      memcpy(&z, buf + 9, 2);
//
//      int x1 = ((x / 2.3));
//      int y1 = ((y / 2.3));
//      int z1 = ((z / 2.3));
//
//      RFM12_DEBUG ("g %4d %4d %4d\n", x1, y1, z1 );
//    }
//      break;
//    default:
//      RFM12_DEBUG ("unknown symbol %c %2x\n", buf[4], buf[4]);
//      RFM12_DEBUG ("content %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x \n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8], buf[9], buf[10], buf[11]);
//    }
  }
  return;

}

void rfm12_txstart(rfm12_index_t size) {
}
uint8_t rfm12_rxstart(void) {
  return 0;
}
rfm12_index_t rfm12_rxfinish(void) {
  return 0;
}

uint16_t rfm12_get_status(void) {
  uint16_t r;

  r = rf12_trans(0x0000); /* read status word */

  return r;
}

void x()
{
  if(g_len)
  {
   uip_send(g_buf,g_len);
  }
  g_len = 0;
}

void rfm12_init(void) {
  rf12_init();

  g_len = 0;
  uip_ipaddr_t ip;
  uip_ipaddr((ip), 10,1,0,255 );
  g_conn = uip_udp_new(&ip, HTONS(12345), x);
}

/*
 -- Ethersex META --
 header(hardware/radio/rfm12/rfm12.h)
 mainloop(rfm12_int_process)
 init(rfm12_init)
 */

