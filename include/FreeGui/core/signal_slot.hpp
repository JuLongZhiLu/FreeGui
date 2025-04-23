#ifndef FREEGUI_SIGNAL_SLOT_HPP
#define FREEGUI_SIGNAL_SLOT_HPP

#include <functional>
#include <memory>
#include <vector>
#include <mutex>

namespace FreeGui {

class Object;

class SignalBase {
public:
    virtual ~SignalBase() = default;
    virtual void disconnectAll() = 0;
};

template<typename... Args>
class Signal : public SignalBase {
public:
    using Slot = std::function<void(Args...)>;
    using SlotID = size_t;

    SlotID connect(const Slot& slot) {
        std::lock_guard<std::mutex> lock(mutex_);
        slots_.emplace_back(++currentId_, slot);
        return currentId_;
    }

    void disconnect(SlotID id) {
        std::lock_guard<std::mutex> lock(mutex_);
        slots_.erase(std::remove_if(slots_.begin(), slots_.end(),
            [id](const auto& pair) { return pair.first == id; }), slots_.end());
    }

    void emit(Args... args) const {
        std::vector<std::pair<SlotID, Slot>> slotsCopy;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            slotsCopy = slots_;
        }
        for (const auto& [id, slot] : slotsCopy) {
            slot(args...);
        }
    }

    void disconnectAll() override {
        std::lock_guard<std::mutex> lock(mutex_);
        slots_.clear();
    }

private:
    mutable std::mutex mutex_;
    std::vector<std::pair<SlotID, Slot>> slots_;
    SlotID currentId_ = 0;
};

} // namespace FreeGui

#endif // FREEGUI_SIGNAL_SLOT_HPP