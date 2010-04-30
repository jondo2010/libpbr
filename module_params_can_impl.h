/*
 * module_params_can_impl.h
 *
 *  Created on: 2010-04-26
 *      Author: john
 */

#ifndef MODULE_PARAMS_CAN_IMPL_H_
#define MODULE_PARAMS_CAN_IMPL_H_

#include <libcan/can.h>

void
module_params_can_impl_init
(
	const uint8_t	mob_index,
	const uint32_t	mod_id,
	const uint8_t	*(*get_param)(uint8_t p, uint8_t data[8]),
	const uint8_t	*(*set_param)(uint8_t p, uint8_t data[8]),
	const uint8_t	num_parameters
);

#endif /* MODULE_PARAMS_CAN_IMPL_H_ */
