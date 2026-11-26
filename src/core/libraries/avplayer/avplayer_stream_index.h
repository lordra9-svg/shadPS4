// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <optional>

#include "common/types.h"

extern "C" {
#include <libavformat/avformat.h>
}

namespace Libraries::AvPlayer {

/// AvPlayerSource::m_video_stream_index and m_audio_stream_index already hold FFmpeg stream
/// indices (EnableStream converts the logical index once). Resolve them against the demuxer
/// context directly instead of treating them as indices into the filtered logical stream list,
/// which can be shorter and therefore read out of bounds.
inline const AVStream* GetFfmpegStream(const AVFormatContext* context, std::optional<s32> index) {
    if (context == nullptr || !index.has_value() || index.value() < 0) {
        return nullptr;
    }
    const auto ffmpeg_index = static_cast<unsigned>(index.value());
    if (ffmpeg_index >= context->nb_streams) {
        return nullptr;
    }
    return context->streams[ffmpeg_index];
}

} // namespace Libraries::AvPlayer
