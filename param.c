/*
 * param.c
 *
 *  Created on: 2010-02-20
 *      Author: john
 */

#include <avr/io.h>

#include "param.h"

const uint8_t *vdm_strings[] = { "Pit", "Accel", "Dynamic"};

const uint8_t *module_param_strings[] =
{
		"",

		"TEL CHANNEL",
		"TEL PAN ID",
		"TEL CAR ADDR",
		"TEL DTA ADDR",
		"TEL DAC ADDR",
		"TEL DTA ON",
		"TEL DAC ON",

		"BRK BIAS",

		"ENG OTSTRT ON",
		"ENG OTSTRT TIM",
		"ENG IDLE RPM",

		"TEST1",
		"TEST2",
		"TEST3",
		"TEST4",
		"TEST5"
};
