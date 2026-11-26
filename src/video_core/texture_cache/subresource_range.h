// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <algorithm>
#include <utility>

#include "common/types.h"

namespace VideoCore {

/// Clamps a subresource range to `limit` and returns the half-open [start, end) interval.
inline std::pair<u32, u32> ClampSubresourceRange(u32 base, u32 extent, u32 limit) {
    const u32 start = std::min(base, limit);
    const auto end = static_cast<u32>(
        std::min<u64>(static_cast<u64>(base) + static_cast<u64>(extent), static_cast<u64>(limit)));
    return {start, end};
}

} // namespace VideoCore
