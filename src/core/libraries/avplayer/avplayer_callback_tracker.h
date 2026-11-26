// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <atomic>

namespace Libraries::AvPlayer {

/// Counts the game-provided replacement callbacks (file and memory) currently executing for a
/// player. A close requested while any of them is active cannot safely destroy the player.
class GuestCallbackTracker {
public:
    class Scope {
    public:
        explicit Scope(const GuestCallbackTracker& tracker) : m_tracker(tracker) {
            m_tracker.m_depth.fetch_add(1, std::memory_order_relaxed);
        }

        ~Scope() {
            m_tracker.m_depth.fetch_sub(1, std::memory_order_relaxed);
        }

        Scope(const Scope&) = delete;
        Scope& operator=(const Scope&) = delete;

    private:
        const GuestCallbackTracker& m_tracker;
    };

    bool IsActive() const {
        return m_depth.load(std::memory_order_relaxed) != 0;
    }

private:
    friend class Scope;

    mutable std::atomic<int> m_depth{0};
};

} // namespace Libraries::AvPlayer
