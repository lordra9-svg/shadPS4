// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/types.h"

namespace Libraries::Vdecsw {

/// Validates a guest buffer. Returns 0 when usable, otherwise the supplied API error.
inline s32 ValidateGuestBuffer(const void* pointer, u64 size, bool is_mapped, s32 pointer_error,
                               s32 size_error) {
    if (pointer == nullptr) {
        return pointer_error;
    }
    if (size == 0) {
        return size_error;
    }
    if (!is_mapped) {
        return pointer_error;
    }
    return 0;
}

} // namespace Libraries::Vdecsw
