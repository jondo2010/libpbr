/*
 * dta_can_impl.h
 *
 *  Created on: 2010-04-25
 *      Author: john
 */

#ifndef DTA_CAN_IMPL_H_
#define DTA_CAN_IMPL_H_

void
dta_can_impl_init (const uint8_t mob_index);

/**
 * Callback for when a DTA packet arrives
 */
void
dta_can_impl_rx
(
	uint8_t mob_index,
	uint32_t id,
	packet_type_t type
);

dta_struct_t *
dta_can_impl_get_dta_struct (void);

#endif /* DTA_CAN_IMPL_H_ */
