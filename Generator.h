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
	static Generator& Instance() {return m_instance;}

private:
	Generator();
	void TimerCompare();
	void TimerOverflow();

	static Generator m_instance;

	friend void TIMER1_COMPA_vect();
	friend void TIMER1_OVF_vect();


};

#endif /* GENERATOR_H_ */
