/*
 * transmitter.cpp
 *
 *  Created on: Jul 30, 2016
 *      Author: compi
 */
#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "Generator.h"

void setup()
{
	Generator::Instance().Init();
	sei();
}

int main(void)
{
	setup();
	while(true)
		for(uint16_t code = 0; code < 4096; ++code)
			for(uint8_t send = 0; send < 3; ++send)
				Generator::Instance().Transmit(code);
	return 0;
}

