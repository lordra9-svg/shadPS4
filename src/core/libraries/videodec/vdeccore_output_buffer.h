// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/types.h"
#include "videodec_error.h"

namespace Libraries::VdecCore {

/// A guest output buffer must be mapped, non-empty and large enough for the frame and metadata.
inline s32 ValidateOutputFrameBuffer(const void* pointer, u64 size, bool is_mapped, u64 required) {
    if (pointer == nullptr || size == 0 || !is_mapped || size < required) {
        return ORBIS_VDEC_CORE_ERROR_INVALID_ARGUMENT;
    }
    return 0;
}

} // namespace Libraries::VdecCore
