/*
 * module_params_can_impl.c
 * Manages automatically transferring module parameter data over the bus
 * in response to a parameter broadcast request.
 *
 *  Created on: 2010-04-26
 *      Author: john
 */

#include <libcan/can.h>

#include "module_params_can_impl.h"

typedef enum fsm_state_t
{
	ready,
	sending,
	param_change
}
fsm_state_t;

static			fsm_state_t fsm_state = ready;
static			uint8_t current_parameter = 0;

static const	uint8_t	*(*get_param_ptr)(uint8_t p, uint8_t data[8]);
static const	uint8_t	*(*set_param_ptr)(uint8_t p, uint8_t data[8]);
static			uint8_t num_params = 0;
static			uint8_t	module_id = 0;

static			mob_config_t mob;

static void prv_reset_fsm (uint8_t mob_index);
static void prv_load_next_param (uint8_t mob_index);
static void prv_load_next_param (uint8_t mob_index);
static void prv_can_tx (uint8_t mob_index);
static void prv_can_rx (uint8_t mob_index, uint32_t id, packet_type_t type);

void
module_params_can_impl_init
(
	const uint8_t	mob_index,
	const uint32_t	mod_id,
	const uint8_t	*(*get_param)(uint8_t p, uint8_t data[8]),
	const uint8_t	*(*set_param)(uint8_t p, uint8_t data[8]),
	const uint8_t	num_parameters
)
{
	get_param_ptr = get_param;
	set_param_ptr = set_param;
	num_params = num_parameters;
	module_id = mod_id;

	prv_reset_fsm (mob_index);
}

static void
prv_reset_fsm (uint8_t mob_index)
{
	fsm_state = ready;
	current_parameter = 0;
	mob.id = 0x4000;
	mob.mask = 0x01ff3000;
	mob.id_type = extended;
	mob.rx_callback_ptr = prv_can_rx;
	mob.tx_callback_ptr = prv_can_tx;
	can_config_mob (mob_index, &mob);
	can_ready_to_receive (mob_index);
}

static void
prv_load_next_param (uint8_t mob_index)
{
	mob.id = 0x4000 | (module_id << 8) | current_parameter;
	can_config_mob (mob_index, &mob);

	uint8_t buf[8];
	uint8_t n = get_param_ptr (current_parameter++, buf);
	can_load_data (mob_index, buf, n);
	can_ready_to_send (mob_index);
}

static void
prv_can_rx
(
	uint8_t mob_index,
	uint32_t id,
	packet_type_t type
)
{
	switch (id >> 12)
	{
		case 0x4:		/// Request to broadcast parameters
			if (fsm_state == ready)
			{
				fsm_state = sending;
				prv_load_next_param (mob_index);
			}
			else
			{
				/// Out of sequence parameter request, ignore.
			}
			break;
		case 0x8:		/// Request to change parameters
			if (fsm_state == ready)
			{
				current_parameter = id & 0x0f;
				uint8_t buf[8];
				can_read_data (mob_index, buf, 8);
				if (set_param_ptr (current_parameter, buf))
				{
					fsm_state = param_change;
					prv_load_next_param (mob_index);
				}
			}
		default:
			can_ready_to_receive (mob_index);
			break;
	}
}

static void
prv_can_tx (uint8_t mob_index)
{
	if (fsm_state == sending)
	{
		if (current_parameter > num_params)			/// Reset
		{
			prv_reset_fsm (mob_index);
		}
		else
		{
			prv_load_next_param (mob_index);
		}
	}
	else if (fsm_state == param_change)
	{
		prv_reset_fsm (mob_index);
	}
}
