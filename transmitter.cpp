/*
 * transmitter.cpp
 *
 *  Created on: Jul 30, 2016
 *      Author: compi
 */
#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Generator.h"

void setup()
{
	Generator::Instance().Init();
	sei();
}


int main(void)
{
	setup();
	while(true);
	return 0;
}

