// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <algorithm>
#include <cstring>
#include <string_view>

#include "common/types.h"

namespace Libraries::Kernel {

inline constexpr size_t PthreadNameBufferSize = 32;

/// Copies a thread name into the 32-byte guest buffer, always NUL-terminating it.
inline void CopyPthreadName(std::string_view name, char* buffer) {
    const size_t length = std::min(name.size(), PthreadNameBufferSize - 1);
    std::memcpy(buffer, name.data(), length);
    buffer[length] = '\0';
}

} // namespace Libraries::Kernel
