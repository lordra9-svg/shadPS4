// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <condition_variable>
#include <mutex>

namespace Libraries::AvPlayer {

/// Tracks how many event callbacks are currently on the stack for a player state. A player must
/// not be destroyed while one of its callbacks is executing, so close defers deletion until this
/// reports idle.
class EventDispatchTracker {
public:
    void Begin() {
        std::lock_guard lock{m_mutex};
        ++m_depth;
    }

    void End() {
        std::lock_guard lock{m_mutex};
        --m_depth;
        if (m_depth == 0) {
            m_cv.notify_all();
        }
    }

    bool IsActive() const {
        std::lock_guard lock{m_mutex};
        return m_depth != 0;
    }

    void WaitForIdle() {
        std::unique_lock lock{m_mutex};
        m_cv.wait(lock, [this] { return m_depth == 0; });
    }

private:
    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
    int m_depth = 0;
};

} // namespace Libraries::AvPlayer
