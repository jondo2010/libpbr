/*
 * timeout.c
 *
 *  Created on: 2010-04-26
 *      Author: John Hughes
 */

#include <inttypes.h>
#include <stdlib.h>
#include <util/atomic.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "timeout.h"

static timeout_t	*head = 0;
static uint16_t		time_ms = 0;

#define timeout_timer_enable() TIMSK1 |= _BV (OCIE1A);
#define timeout_timer_disable() TIMSK1 &= ~_BV (OCIE1A);

void
prv_timeout_ordered_insert (timeout_t *to);

void
prv_timeout_unlink (timeout_t *to);

/**
 * 1ms interrupt handler
 */
ISR (TIMER1_COMPA_vect)
{
	if (head==0)
	{
		timeout_timer_disable ();
		return;
	}

	time_ms++;

	timeout_t *cur = head;

	/// Iterate from head to skip over any items that have rolled-over the 16-bit limit
	while (cur->next && cur->expires_at < time_ms)
	{
		cur = cur->next;
	}

	/// Evaluate the expiring callbacks
	while (cur && cur->expires_at == time_ms)
	{
		cur->callback ();

		if (cur->recurring)
		{
			timeout_reset (cur, 0);
		}
		else
		{
			timeout_clear (cur);
		}

		cur = cur->next;
	}
}

void
timeout_init (void)
{
	TCCR1B = _BV (WGM12) | _BV (CS11) | _BV (CS10);			/// CTC mode, Main clock/64 = 250 kHz
	OCR1AH = 0x00;											/// Timer counts up to 250
	OCR1AL = 0xfa;
}

timeout_t *
timeout_set
(
	uint16_t	delay_ms,
	void		(*callback)(void),
	uint8_t		recurring
)
{
	timeout_t *to = malloc (sizeof (timeout_t));

	to->delay_ms = delay_ms;
	to->expires_at = time_ms + delay_ms;
	to->callback = callback;
	to->recurring = recurring;

	prv_timeout_ordered_insert (to);

	timeout_timer_enable ();
	return to;
}

void
timeout_clear
(
	timeout_t	*to
)
{
	prv_timeout_unlink (to);
	free (to);
}

void
timeout_reset
(
	timeout_t	*to,
	uint16_t	delay_ms
)
{
	prv_timeout_unlink (to);
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		to->expires_at = time_ms + ((delay_ms > 0) ? delay_ms : to->delay_ms);			/// New expiration time
	}
	prv_timeout_ordered_insert (to);
}

/** End of public functions **/

void
prv_timeout_ordered_insert (timeout_t *to)
{
	timeout_t *cur;

	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		if (head==0)	/// Empty list
		{
			head = to;
			to->prev = 0;
			to->next = 0;
		}
		else
		{
			if (to->expires_at < head->expires_at)	/// Insert before head
			{
				to->prev = 0;
				to->next = head;
				head->prev = to;
				head = to;
			}
			else									/// Iterate through the list to find the correct insertion point.
			{
				for (cur = head; (cur->next && to->expires_at > cur->expires_at); cur = cur->next);

				to->next = cur->next;
				to->prev = cur;
				cur->next = to;

				if (to->next)
				{
					to->next->prev = to;
				}
			}
		}
	}
}

void
prv_timeout_unlink (timeout_t *to)
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		if (to == head)
		{
			head = to->next;
			to->next->prev = 0;
		}
		else if (to->next == 0)
		{
			to->prev->next = 0;
		}
		else
		{
			to->prev->next = to->next;
			to->next->prev = to->prev;
		}
	}
}
