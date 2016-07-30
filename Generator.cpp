/*
 * timer1.cpp
 *
 *  Created on: Jul 30, 2016
 *      Author: compi
 */

#include "Generator.h"

#include "config.h"
#include <avr/interrupt.h>

Generator Generator::m_instance;

Generator::Generator()
{
	// TODO Auto-generated constructor stub
}

void Generator::Init()
{
	REMOTE_DDR_PORT |= REMOTE_DDR_BIT;

	TCCR1A = 0;
    TCCR1B = _BV(CS11); // normal mode; F_CPU / 8 => 16MHz / (8 * 65536) = 30.517578125 Hz => 32.768 msec (tick: 500ns)
    TIMSK1 |= _BV(TOIE1); // enable overflow interrupt
    OCR1A = TCNT1 + 1000;	//50ms
    TIMSK1 |= _BV(OCIE1A);
}



ISR(TIMER1_OVF_vect)
{
	OCR1A += 1000;
	REMOTE_PORT ^= _BV(REMOTE_BIT);
}

ISR(TIMER1_COMPA_vect)
{
}
