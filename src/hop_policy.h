// Copyright 2026 Roman Kuzmitskii (@damex)
// SPDX-License-Identifier: MIT

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Length-1 acked keepalive: a retransmit can't replay stale deltas, and its one byte
 * carries the peripheral's current rate. */
#define ESB_KEEPALIVE_LENGTH 1
#define KEEPALIVE_RATE_OFFSET 0
#define KEEPALIVE_RATE_IDLE 0x00
#define KEEPALIVE_RATE_ACTIVE 0x01

/* Returns true and clears the streak after threshold consecutive failures. */
bool hop_policy_should_hop(uint8_t *bad_windows, bool window_failed, uint16_t threshold);

bool hop_policy_marks_active(uint8_t length, uint8_t first_byte);

bool hop_policy_is_keepalive(uint8_t length);

uint8_t hop_policy_index_next(uint8_t index, size_t count);
