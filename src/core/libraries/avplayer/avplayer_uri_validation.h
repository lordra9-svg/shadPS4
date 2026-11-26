// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/types.h"

namespace Libraries::AvPlayer {

/// Upper bound on a guest-supplied URI/path. Well beyond any real path, while stopping a bogus
/// length from making the host scan an arbitrarily large guest range.
constexpr u32 MaxUriLength = 4096;

/// A URI descriptor is valid when it names a non-empty, bounded range that is mapped in guest
/// memory. `is_mapped` is the caller's mapping check for [name, name + length).
inline bool IsValidUri(const char* name, u32 length, bool is_mapped) {
    return name != nullptr && length > 0 && length <= MaxUriLength && is_mapped;
}

} // namespace Libraries::AvPlayer
