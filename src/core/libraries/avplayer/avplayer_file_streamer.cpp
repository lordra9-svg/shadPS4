// SPDX-FileCopyrightText: Copyright 2024 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <algorithm> // std::max, std::min
#include "core/libraries/avplayer/avplayer_file_streamer.h"

extern "C" {
#include <libavformat/avio.h>
#include <libavutil/error.h>
#include <libavutil/mem.h>
}

constexpr u32 AVPLAYER_AVIO_BUFFER_SIZE = 4096;

namespace Libraries::AvPlayer {

AvPlayerFileStreamer::AvPlayerFileStreamer(const AvPlayerFileReplacement& file_replacement)
    : m_file_replacement(file_replacement) {}

AvPlayerFileStreamer::~AvPlayerFileStreamer() {
    if (m_avio_context != nullptr) {
        avio_context_free(&m_avio_context);
    }
    if (m_file_replacement.close != nullptr && m_fd >= 0) {
        const auto close = m_file_replacement.close;
        const auto ptr = m_file_replacement.object_ptr;
        close(ptr);
    }
}

bool AvPlayerFileStreamer::Init(std::string_view path) {
    const auto ptr = m_file_replacement.object_ptr;
    m_fd = m_file_replacement.open(ptr, path.data());
    if (m_fd < 0) {
        return false;
    }
    m_file_size = m_file_replacement.size(ptr);
    // avio_buffer is deallocated in `avio_context_free`
    const auto avio_buffer = reinterpret_cast<u8*>(av_malloc(AVPLAYER_AVIO_BUFFER_SIZE));
    m_avio_context =
        avio_alloc_context(avio_buffer, AVPLAYER_AVIO_BUFFER_SIZE, 0, this,
                           &AvPlayerFileStreamer::ReadPacket, nullptr, &AvPlayerFileStreamer::Seek);
    return true;
}

void AvPlayerFileStreamer::Reset() {
    m_position = 0;
}

s32 AvPlayerFileStreamer::ReadPacket(void* opaque, u8* buffer, s32 size) {
    const auto self = reinterpret_cast<AvPlayerFileStreamer*>(opaque);
    if (size < 0) {
        return AVERROR(EINVAL);
    }
    if (size == 0) {
        return 0;
    }
    if (self->m_position >= self->m_file_size) {
        return AVERROR_EOF;
    }
    const u64 remaining = self->m_file_size - self->m_position;
    if (static_cast<u64>(size) > remaining) {
        size = static_cast<s32>(remaining);
    }
    const auto read_offset = self->m_file_replacement.read_offset;
    const auto ptr = self->m_file_replacement.object_ptr;
    const s32 bytes_read = read_offset(ptr, buffer, self->m_position, static_cast<u32>(size));
    if (bytes_read < 0) {
        // Guest-reported error: leave the position untouched instead of underflowing it.
        return bytes_read;
    }
    if (bytes_read == 0) {
        return AVERROR_EOF;
    }
    if (bytes_read > size) {
        // The callback claims more bytes than it was asked for. Advancing would desync the
        // stream and let FFmpeg read past the end of the supplied buffer.
        return AVERROR(EIO);
    }
    self->m_position += static_cast<u64>(bytes_read);
    return bytes_read;
}

s64 AvPlayerFileStreamer::Seek(void* opaque, s64 offset, int whence) {
    const auto self = reinterpret_cast<AvPlayerFileStreamer*>(opaque);
    if (whence & AVSEEK_SIZE) {
        return self->m_file_size;
    }

    if (whence == SEEK_CUR) {
        self->m_position =
            std::min(u64(std::max(s64(0), s64(self->m_position) + offset)), self->m_file_size);
        return self->m_position;
    } else if (whence == SEEK_SET) {
        self->m_position = std::min(u64(std::max(s64(0), offset)), self->m_file_size);
        return self->m_position;
    } else if (whence == SEEK_END) {
        self->m_position =
            std::min(u64(std::max(s64(0), s64(self->m_file_size) + offset)), self->m_file_size);
        return self->m_position;
    }

    return -1;
}

} // namespace Libraries::AvPlayer
