/*
 * dtafast.h
 * Defines structures for the data received from the
 * DTAFast ECU over CANBus
 *
 * Created on: 2010-01-31
 * Author: John Hughes <jondo2010@gmail.com>
 */

#ifndef DTAFAST_H_
#define DTAFAST_H_

#include <avr/io.h>

typedef struct dta_struct1_t
{
	uint16_t rpm;			/* RPM						*/
	uint16_t tps;			/* Throttle position %		*/
	uint16_t water_t;		/* Water temperature C		*/
	uint16_t air_t;			/* Air temperature C		*/
}
dta_struct1_t;

typedef struct dta_struct2_t
{
	uint16_t man_p;			/* Manifold Pressure in Kpa	*/
	uint16_t lambda;		/* Lambda x 1000			*/
	uint16_t kph;			/* KPH x 10					*/
	uint16_t oil_p;			/* Oil Pressure in Kpa		*/
}
dta_struct2_t;

typedef struct dta_struct3_t
{
	uint16_t fuel_p;		/* Fuel Pressure in Kpa		*/
	uint16_t oil_t;			/* Oil temperature C		*/
	uint16_t volts;			/* System voltage x 10		*/
	uint16_t l_hr;			/* Fuel usage L/Hr x 10		*/
}
dta_struct3_t;

typedef struct dta_struct4_t
{
	uint16_t gear;			/* Current gear				*/
	uint16_t advance;		/* Spark advance deg. x 10	*/
	uint16_t injection;		/* Injection period x 100	*/
	uint16_t l_km;			/* Fuel usage L/100Km x 10	*/
}
dta_struct4_t;

typedef struct dta_struct_t
{
	dta_struct1_t data1;
	dta_struct2_t data2;
	dta_struct3_t data3;
	dta_struct4_t data4;
}
dta_struct_t;

#endif /* DTAFAST_H_ */
