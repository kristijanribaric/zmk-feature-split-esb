// Copyright 2026 Roman Kuzmitskii (@damex)
// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "hop_policy.h"

bool hop_policy_should_hop(uint8_t *bad_windows, bool window_failed, uint16_t threshold) {
    assert(bad_windows != NULL);
    if (window_failed) {
        (*bad_windows)++;
    } else {
        *bad_windows = 0;
    }
    if (*bad_windows >= threshold) {
        *bad_windows = 0;
        return true;
    }
    return false;
}

bool hop_policy_marks_active(uint8_t length, uint8_t first_byte) {
    if (length == ESB_KEEPALIVE_LENGTH) {
        return first_byte == KEEPALIVE_RATE_ACTIVE;
    }
    return true;
}

bool hop_policy_is_keepalive(uint8_t length) {
    return length == ESB_KEEPALIVE_LENGTH;
}

uint8_t hop_policy_index_next(uint8_t index, size_t count) {
    assert(count > 0);
    return (uint8_t)(((size_t)index + 1U) % count);
}
