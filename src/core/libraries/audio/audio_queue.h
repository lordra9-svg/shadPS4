// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/types.h"

namespace Libraries::AudioOut {

/// Returns the number of bytes to discard from a backed-up queue so it retains half a threshold.
inline s32 AudioQueueBytesToDiscard(s32 queued, s32 threshold) {
    if (threshold <= 0 || queued < threshold) {
        return 0;
    }
    return queued - threshold / 2;
}

} // namespace Libraries::AudioOut
