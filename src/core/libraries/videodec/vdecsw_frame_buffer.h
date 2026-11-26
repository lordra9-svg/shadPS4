// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "common/alignment.h"
#include "common/types.h"

namespace Libraries::Vdecsw {

/// Size of an NV12 frame after the 64-byte pitch / 16-line height alignment used by CopyNV12Data.
inline u64 Nv12FrameDataSize(u32 width, u32 height) {
    const u64 pitch = Common::AlignUp<u64>(static_cast<u64>(width), 64);
    const u64 aligned_height = Common::AlignUp<u64>(static_cast<u64>(height), 16);
    return pitch * aligned_height * 3 / 2;
}

/// True when the guest buffer fits both the frame data and the trailing picture-info block.
inline bool IsFrameBufferLargeEnough(u64 buffer_size, u32 width, u32 height, u64 info_size) {
    const u64 frame_size = Nv12FrameDataSize(width, height);
    return buffer_size >= frame_size && buffer_size - frame_size >= info_size;
}

} // namespace Libraries::Vdecsw
