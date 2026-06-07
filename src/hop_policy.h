// Copyright 2026 Roman Kuzmitskii (@damex)
// SPDX-License-Identifier: MIT

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Length-1 acked keepalive: a retransmit can't replay stale deltas, length 1 marks it as a
 * keepalive rather than a split message, and its one byte tells the central whether the
 * peripheral is actively polling. */
#define ESB_KEEPALIVE_LENGTH 1
#define ESB_KEEPALIVE_IDLE 0x00
#define ESB_KEEPALIVE_ACTIVE 0x01

/* True when a keepalive byte reports the peripheral is actively polling. */
bool hop_policy_keepalive_is_active(uint8_t byte);

/* Returns true and clears the streak after threshold consecutive failures. */
bool hop_policy_should_hop(uint8_t *bad_windows, bool window_failed, uint16_t threshold);

bool hop_policy_is_keepalive(uint8_t length);

uint8_t hop_policy_index_next(uint8_t index, size_t count);

/* Both ends derive the same channel index from the central's epoch. */
uint8_t hop_policy_channel_for_epoch(uint16_t epoch, size_t hop_count);

/* Central hop decision: weighted sum of per-peripheral link loss, true at threshold. */
bool hop_policy_hop_vote(const uint8_t *link_loss, const uint8_t *weights, size_t count,
                         uint16_t threshold);

/* Per window, accrue per-pipe loss from poll traffic, so only an actively-polling pipe
 * whose motion is going missing can drive a hop.
 * Motion this window (bit in motion_mask) clears loss. An active pipe with no motion (in
 * active_mask, not motion_mask) is losing its poll data and accrues loss, saturating at
 * UINT8_MAX. An idle or absent pipe (neither) clears loss, so it never drives a hop. */
void hop_policy_accrue_loss(uint8_t *link_loss, size_t count, uint32_t motion_mask,
                            uint32_t active_mask);

/* Beacon scheduling: announce the epoch only while it is fresh.
 * A changed epoch arms repeat_windows announcements, then goes quiet, so a steady stream
 * never crowds commands out of the reverse channel. */
bool hop_policy_should_beacon(uint8_t epoch, uint8_t *announced_epoch, uint8_t *repeats_left,
                              uint8_t repeat_windows);
