/*
 * timeout.h
 * Allows setting arbitrary timeouts with millisecond resolution.
 * Uses TIMER1.
 *
 *  Created on: 2010-04-26
 *      Author: john
 */

#ifndef TIMEOUT_H_
#define TIMEOUT_H_

#include <inttypes.h>

typedef struct timeout_t
{
	uint16_t			delay_ms;
	uint16_t			expires_at;
	void				(*callback)(void);
	uint8_t				recurring;
	struct timeout_t	*prev;
	struct timeout_t	*next;
}
timeout_t;

void
timeout_init (void);

/**
 * Set a timeout to occur delay_ms milliseconds from now.
 * If recurring is > 1, the timeout will automatically be rescheduled
 * after it expires.
 */
timeout_t *
timeout_set
(
	uint16_t	delay_ms,
	void		(*callback)(void),
	uint8_t		recurring
);

/**
 * Clear a timeout
 */
void
timeout_clear
(
	timeout_t	*timeout
);

/**
 * Reset a timeout. If delay_ms = 0, resets using the previous delay.
 */
void
timeout_reset
(
	timeout_t	*timeout,
	uint16_t	delay_ms
);

#endif /* TIMEOUT_H_ */
