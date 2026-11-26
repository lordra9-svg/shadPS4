// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <filesystem>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "common/types.h"

namespace Vulkan {

/// Reads a patched SPIR-V binary from patch_dir, or returns nothing if there is no patch.
std::optional<std::vector<u32>> LoadShaderPatch(const std::filesystem::path& patch_dir,
                                                const std::string& filename);

/// Chooses the patched binary when patching is enabled and a non-empty patch exists.
inline std::span<const u32> SelectShaderBinary(const std::vector<u32>& cached,
                                               const std::optional<std::vector<u32>>& patch,
                                               bool patch_enabled) {
    if (patch_enabled && patch && !patch->empty()) {
        return *patch;
    }
    return cached;
}

} // namespace Vulkan
