/*
 * timer1.h
 *
 *  Created on: Jul 30, 2016
 *      Author: compi
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <avr/io.h>

extern "C" void TIMER1_COMPA_vect() __attribute__((signal));
extern "C" void TIMER1_OVF_vect() __attribute__((signal));


class Generator
{
public:
	void Init();
	inline static Generator& Instance() {return m_instance;}
	void Transmit(uint16_t data);
	void BusyWait(uint16_t ticks);

private:
	Generator() = default;
	void TimerCompare();
	void TimerOverflow();
	inline void SetOutput();

	static Generator m_instance;

	volatile enum { idle, bits, stop}	m_phase = idle;

	bool 		m_output;
	uint16_t	m_data;
	uint8_t		m_bit;
	
	uint32_t	m_ovfcnt = 0;

	static const uint16_t	PULSES_SHORT	= 900;
	static const uint16_t	PULSES_LONG		= 1900;
	static const uint16_t	PULSES_SPACE	= 32000;

	friend void TIMER1_COMPA_vect();
	friend void TIMER1_OVF_vect();
};

#endif /* GENERATOR_H_ */
