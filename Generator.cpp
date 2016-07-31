/*
 * timer1.cpp
 *
 *  Created on: Jul 30, 2016
 *      Author: compi
 */

#include "Generator.h"

#include "config.h"
#include <avr/interrupt.h>
#include <util/atomic.h>

Generator Generator::m_instance;
// Timer1: normal mode; F_CPU / 8 => 16MHz / (8 * 65536) = 30.517578125 Hz => 32.768 msec (tick: 500ns)

void Generator::Init()
{
	REMOTE_DDR_PORT |= _BV(REMOTE_DDR_BIT);
	m_output = true;

	SetOutput();

	TCCR1A = 0;
    TIMSK1 |= _BV(TOIE1); // enable overflow interrupt
	TCNT1 = 0;
    OCR1A = PULSES_LONG;	//50ms
	m_phase = stop;
	TCCR1B |= _BV(CS11);	//start timer (pre = F/8)
    TIMSK1 |= _BV(OCIE1A);
}

void Generator::Transmit(uint16_t data)
{
	while(m_phase != idle)
		;
		
	m_data = data;
	m_bit = 0;
	m_phase = bits;
	m_output = true;
	SetOutput();
	OCR1A = TCNT1 + PULSES_SHORT;
    TIMSK1 |= _BV(OCIE1A);
}

void Generator::BusyWait(uint16_t ticks)
{
	uint16_t start, now;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
		start = TCNT1;
	}
	do 
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
			now = TCNT1;
		}
	} while (now - start < ticks);
	
}

void Generator::TimerCompare()
{
	bool bitst;

	switch(m_phase)
	{
	case bits:
		bitst = (m_data & ((uint16_t)1 << m_bit)) != 0;
		if(m_output)	//bit start
			OCR1A += bitst ? PULSES_SHORT : PULSES_LONG;
		else {
			OCR1A += bitst ? PULSES_LONG : PULSES_SHORT;
			if(++m_bit == 12)
				m_phase = stop;
		}
		m_output = !m_output;
		break;

	case stop:
		if(m_output) {
			m_output = false;
			OCR1A += PULSES_SPACE;
		} else {
		    TIMSK1 &= ~_BV(OCIE1A);
			m_phase = idle;
		}
		break;

	case idle:
		break;
	}

	SetOutput();
}

void Generator::TimerOverflow()
{
	++m_ovfcnt;
}

void Generator::SetOutput()
{
	REMOTE_PORT = (REMOTE_PORT & ~_BV(REMOTE_BIT)) | ((uint8_t)m_output << REMOTE_BIT);
}

ISR(TIMER1_OVF_vect)
{
	Generator::Instance().TimerOverflow();
}

ISR(TIMER1_COMPA_vect)
{
	Generator::Instance().TimerCompare();
}
