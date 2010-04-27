/*
 * module_params_can_impl.h
 *
 *  Created on: 2010-04-26
 *      Author: john
 */

#ifndef MODULE_PARAMS_CAN_IMPL_H_
#define MODULE_PARAMS_CAN_IMPL_H_

#include <libcan/can.h>

typedef enum module_params_fsm_state_t
{
	waiting_for_remote,
	sending
}
module_params_fsm_state_t;

void
module_params_can_impl_init
(
	uint8_t			mob_index,
	uint32_t		id,
	const void		*(*get_param_struct)(void),
	const uint8_t	struct_size
);

/**
 * Callback after the config mob replies to a remote request
 * for the modules' config data. If the config data spans
 * more than 8 bytes, after the initial 8 byte reply we start
 * sending regular messages with the rest.
 */
void
module_params_can_impl_tx
(
	uint8_t mob_index
);

#endif /* MODULE_PARAMS_CAN_IMPL_H_ */
