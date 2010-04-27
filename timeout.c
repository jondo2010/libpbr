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
			timeout_reset (cur, 0, 0);
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
	//TIMSK1 = _BV (OCIE1A);									/// Interrupt when the timer matches the count
}

timeout_t *
timeout_set
(
	uint16_t	delay_ms,
	void		(*callback)(void),
	uint8_t		recurring
)
{
	timeout_t *cur;
	timeout_t *to = malloc (sizeof (timeout_t));

	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		to->expires_at = time_ms + delay_ms;
		to->callback = callback;

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

	timeout_timer_enable ();
	return to;
}

void
timeout_clear
(
	timeout_t	*timeout
)
{
	ATOMIC_BLOCK (ATOMIC_RESTORESTATE)
	{
		if (timeout == head)
		{
			head = timeout->next;
			timeout->next->prev = 0;
			//timeout_timer_disable ();
		}
		else if (timeout->next == 0)
		{
			timeout->prev->next = 0;
		}
		else
		{
			timeout->prev->next = timeout->next;
			timeout->next->prev = timeout->prev;
		}
	}

	free (timeout);
}

void
timeout_reset
(
	timeout_t	*timeout,
	uint16_t	delay_ms,
	void		(*callback)(void)
)
{
}
