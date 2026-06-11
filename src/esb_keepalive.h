// Copyright 2026 Roman Kuzmitskii (@damex)
// SPDX-License-Identifier: MIT

/*
 * Uplink keepalive wire format: tag, hop-state byte, pressed-position bitmap.
 * Tag 0xFF cannot collide with event packets, whose first byte is an event type.
 * The bitmap is the peripheral's authoritative pressed set: the central diffs it
 * against its own view and replays transitions the radio lost.
 * Positions above ESB_KEEPALIVE_POSITION_COUNT are not covered.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

#define ESB_KEEPALIVE_TAG 0xFF
#define ESB_KEEPALIVE_TAG_OFFSET 0
#define ESB_KEEPALIVE_STATE_OFFSET 1
#define ESB_KEEPALIVE_BITMAP_OFFSET 2
#define ESB_KEEPALIVE_BITMAP_BYTES 8
#define ESB_KEEPALIVE_POSITION_COUNT (ESB_KEEPALIVE_BITMAP_BYTES * 8)
#define ESB_KEEPALIVE_LENGTH (ESB_KEEPALIVE_BITMAP_OFFSET + ESB_KEEPALIVE_BITMAP_BYTES)

/* out must hold ESB_KEEPALIVE_LENGTH bytes. */
void esb_keepalive_encode(uint8_t *out, uint8_t state, const uint8_t *position_bitmap);

bool esb_keepalive_matches(const uint8_t *data, uint8_t length);

uint8_t esb_keepalive_state(const uint8_t *data);

const uint8_t *esb_keepalive_bitmap(const uint8_t *data);

/* Out-of-range positions: set is ignored, get reads false. */
void esb_keepalive_bitmap_set(uint8_t *bitmap, uint32_t position, bool pressed);
bool esb_keepalive_bitmap_get(const uint8_t *bitmap, uint32_t position);
