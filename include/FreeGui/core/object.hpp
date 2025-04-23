#ifndef FREEGUI_OBJECT_HPP
#define FREEGUI_OBJECT_HPP

#include "signal_slot.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <mutex>

namespace FreeGui {

class Object {
public:
    Object() = default;
    virtual ~Object() {
        disconnectAllSignals();
    }

    template<typename... Args>
    using SignalConnection = std::pair<typename Signal<Args...>::SlotID, Signal<Args...>*>;

    template<typename... Args>
    SignalConnection<Args...> connectSignal(const std::string& name, 
                                          const typename Signal<Args...>::Slot& slot) {
        std::lock_guard<std::mutex> lock(signalsMutex_);
        auto* signal = getSignal<Args...>(name);
        auto id = signal->connect(slot);
        return {id, signal};
    }

    template<typename... Args>
    void disconnectSignal(const std::string& name, typename Signal<Args...>::SlotID id) {
        std::lock_guard<std::mutex> lock(signalsMutex_);
        if (auto it = signals_.find(name); it != signals_.end()) {
            auto* signal = dynamic_cast<Signal<Args...>*>(it->second.get());
            if (signal) {
                signal->disconnect(id);
            }
        }
    }

    void disconnectAllSignals() {
        std::lock_guard<std::mutex> lock(signalsMutex_);
        for (auto& [name, signal] : signals_) {
            signal->disconnectAll();
        }
        signals_.clear();
    }

protected:
    template<typename... Args>
    Signal<Args...>* createSignal(const std::string& name) {
        std::lock_guard<std::mutex> lock(signalsMutex_);
        auto signal = std::make_unique<Signal<Args...>>();
        auto* ptr = signal.get();
        signals_[name] = std::move(signal);
        return ptr;
    }

    template<typename... Args>
    Signal<Args...>* getSignal(const std::string& name) {
        if (auto it = signals_.find(name); it != signals_.end()) {
            return dynamic_cast<Signal<Args...>*>(it->second.get());
        }
        return createSignal<Args...>(name);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<SignalBase>> signals_;
    mutable std::mutex signalsMutex_;
};

} // namespace FreeGui

#endif // FREEGUI_OBJECT_HPP