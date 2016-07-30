/*
 * transmitter.cpp
 *
 *  Created on: Jul 30, 2016
 *      Author: compi
 */
#include "config.h"
#include <avr/io.h>
#include "Generator.h"

void setup()
{
	Generator::Instance().Init();
}


int main(void)
{
	return 0;
}

