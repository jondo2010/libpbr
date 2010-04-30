/*
 * param.h
 *
 *  Created on: 2010-02-20
 *      Author: John Hughes
 */

#ifndef PARAM_H_
#define PARAM_H_

#include <avr/io.h>

//const uint8_t *vdm_strings[];
//const uint8_t *module_param_strings[];

/**
 * Vehicle Dynamics Mode
 */
typedef enum vdm_mode_t
{
	pit,
	accel,
	dynamic
}
vmd_mode_t;

typedef struct module_param_t
{
	const char		*string;
	const uint8_t	size;
}
module_param_t;

/**
 * Module Parameters
 */
/*
typedef enum module_param_t2
{
	// General
	param_general_vdm_mode		= 0x00,			// VDM Mode

	// Telemetry
	param_telemetry_channel		= 0x01,			// Zigbee channel
	param_telemetry_pan			= 0x02,			// 16-bit PAN ID
	param_telemetry_car_address	= 0x03,			// 16-bit address of the car's modem
	param_telemetry_dta_address	= 0x04,
	param_telemetry_dac_address	= 0x05,
	param_telemetry_dta_enabled	= 0x06,
	param_telemetry_dac_enabled	= 0x07,

	// Brake
	param_brake_bias				= 0x08,		// Signed 8-bit brake bias front-to-back

	// Engine
	param_engine_otstart_enabled	= 0x09,		// One-touch start
	param_engine_otstart_timeout	= 0x0a,		// Timeout in ms
	param_engine_idle_rpm			= 0x0b,		// Idle RPM used to determine if running

	param_test1,
	param_test2,
	param_test3,
	param_test4,
	param_test5
}
module_param_t2;
*/
#endif /* PARAM_H_ */
