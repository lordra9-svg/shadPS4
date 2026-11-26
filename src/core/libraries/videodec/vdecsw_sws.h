// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

namespace Libraries::Vdecsw {

struct SwsCacheKey {
    int width{};
    int height{};
    int format{-1};
};

/// True when a cached sws context was built for the given source geometry/format.
inline bool SwsContextMatches(const SwsCacheKey& cached, int width, int height, int format) {
    return cached.width == width && cached.height == height && cached.format == format;
}

} // namespace Libraries::Vdecsw
