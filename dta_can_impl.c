/*
 * dta_can_impl.c
 *
 *  Created on: 2010-04-25
 *      Author: john
 */

#include <libcan/can.h>
#include "dtafast.h"
#include "dta_can_impl.h"

static dta_struct_t dta_data;

/**
 * Initialize the DTA packet receive at the given mob index
 */
void
dta_can_impl_init (const uint8_t mob_index)
{
	mob_config_t mob;

	mob.id = 0x2000;
	mob.mask = 0x01fffffc;
	mob.id_type = extended;
	mob.rx_callback_ptr = dta_can_impl_rx;
	mob.tx_callback_ptr = 0;
	can_config_mob (mob_index, &mob);
	can_ready_to_receive (mob_index);
}

/**
 * Callback for when a DTA packet arrives
 */
void
dta_can_impl_rx
(
	uint8_t mob_index,
	uint32_t id,
	packet_type_t type
)
{
	switch (id)
	{
	case 0x2000:
		can_read_data (mob_index, (uint8_t *) &dta_data.data1, 8);
		break;
	case 0x2001:
		can_read_data (mob_index, (uint8_t *) &dta_data.data2, 8);
		break;
	case 0x2002:
		can_read_data (mob_index, (uint8_t *) &dta_data.data3, 8);
		break;
	case 0x2003:
		can_read_data (mob_index, (uint8_t *) &dta_data.data4, 8);
		break;
	}

	can_ready_to_receive (mob_index);
}

dta_struct_t *
dta_can_impl_get_dta_struct (void)
{
	return &dta_data;
}
