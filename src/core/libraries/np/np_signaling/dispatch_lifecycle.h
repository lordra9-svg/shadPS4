// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <mutex>

namespace Libraries::Np::NpSignaling {

enum class DispatchLifecycleState {
    Stopped,
    Running,
    Stopping,
};

/// Serialises start/stop of the NP signaling dispatch thread. Callers must hold Mutex() while
/// reading or changing the state and while creating or joining the worker.
class DispatchLifecycle {
public:
    std::mutex& Mutex() {
        return m_mutex;
    }

    bool IsStopped() const {
        return m_state == DispatchLifecycleState::Stopped;
    }
    bool IsRunning() const {
        return m_state == DispatchLifecycleState::Running;
    }
    DispatchLifecycleState State() const {
        return m_state;
    }
    void MarkRunning() {
        m_state = DispatchLifecycleState::Running;
    }
    void MarkStopping() {
        m_state = DispatchLifecycleState::Stopping;
    }
    void MarkStopped() {
        m_state = DispatchLifecycleState::Stopped;
    }

private:
    std::mutex m_mutex;
    DispatchLifecycleState m_state{DispatchLifecycleState::Stopped};
};

} // namespace Libraries::Np::NpSignaling
