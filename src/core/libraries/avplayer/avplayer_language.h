// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <cstring>

#include "common/types.h"

namespace Libraries::AvPlayer {

/// Default language codes are three-letter ISO 639-1/2 codes, matching the 4-byte
/// language_code fields (3 characters plus a terminator).
constexpr u32 LanguageCodeLength = 3;

/// Copy a guest-provided language code into the fixed 4-byte buffer. The source is read for at
/// most LanguageCodeLength bytes and the destination is always NUL-terminated, so callers can
/// safely build a string_view over it. Returns false (leaving the buffer empty) when there is no
/// mapped source.
inline bool CopyLanguageCode(char (&dest)[4], const char* source, bool is_mapped) {
    std::memset(dest, 0, sizeof(dest));
    if (source == nullptr || !is_mapped) {
        return false;
    }
    std::memcpy(dest, source, LanguageCodeLength);
    dest[LanguageCodeLength] = '\0';
    return true;
}

} // namespace Libraries::AvPlayer
