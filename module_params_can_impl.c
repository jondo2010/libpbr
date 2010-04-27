/*
 * module_params_can_impl.c
 * Manages automatically transferring module parameter data over the bus
 * in response to a remote request.
 *
 *  Created on: 2010-04-26
 *      Author: john
 */

#include <libcan/can.h>

#include "module_params_can_impl.h"

static			module_params_fsm_state_t fsm_state = waiting_for_remote;
static			uint8_t bytes_sent = 0;
static void		*(*get_param_struct_ptr)(void);
static			uint8_t param_struct_size = 0;

void
module_params_can_impl_init
(
	uint8_t			mob_index,
	uint32_t		id,
	const void		*(*get_param_struct)(void),
	const uint8_t	struct_size
)
{
	mob_config_t mob;

	get_param_struct_ptr = get_param_struct;
	param_struct_size = struct_size;

	/// MOB2 is used to send module data
	mob.id = id;
	mob.mask = 0xffffffff;
	mob.rx_callback_ptr = 0;
	mob.tx_callback_ptr = module_params_can_impl_tx;
	can_config_mob (2, &mob);

	/// Load up to the first 8 bytes of parameter data as remote request payload
	const uint8_t *params = get_param_struct ();
	can_load_data (2, (uint8_t *)params, param_struct_size);
	can_reply_valid (2);

	bytes_sent = param_struct_size > 8 ? 8 : (param_struct_size);
}

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
)
{
	uint8_t *params = get_param_struct_ptr ();
	uint8_t num_bytes_to_send;

	num_bytes_to_send = (param_struct_size - bytes_sent) > 8 ? 8 : (param_struct_size - bytes_sent);

	if (fsm_state == waiting_for_remote && num_bytes_to_send > 0)
	{
		/// Set up second message to go out
		can_load_data (mob_index, (uint8_t *)(params+bytes_sent), num_bytes_to_send);
		can_ready_to_send (mob_index);

		bytes_sent += param_struct_size - bytes_sent;
		fsm_state = sending;
	}
	else if (param_struct_size <= 8 || bytes_sent == param_struct_size)
	{
		/// Done sending data, reload the remote request payload
		can_load_data (mob_index, (uint8_t *)(params), param_struct_size);
		can_reply_valid (mob_index);
		fsm_state = waiting_for_remote;
	}
	else
	{
		/// Still more data to send
		can_load_data (mob_index, (uint8_t *)(params+bytes_sent), num_bytes_to_send);
		can_ready_to_send (mob_index);

		bytes_sent += param_struct_size - bytes_sent;
	}
}
