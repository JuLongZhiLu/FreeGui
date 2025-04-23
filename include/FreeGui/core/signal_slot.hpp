#ifndef FREEGUI_SIGNAL_SLOT_HPP
#define FREEGUI_SIGNAL_SLOT_HPP

#include <functional>
#include <memory>
#include <vector>

namespace FreeGui {

class Object;

class SignalBase
{
public:
  virtual ~SignalBase() = default;
};

template<typename... Args>
class Signal : public SignalBase
{
public:
  using Slot = std::function<void(Args...)>;

  void connect(const Slot& slot) { slots_.push_back(slot); }
  void emit(Args... args) const
  {
    for (const auto& slot : slots_) {
      slot(args...);
    }
  }

private:
  std::vector<Slot> slots_;
};

} // namespace FreeGui

#endif // FREEGUI_SIGNAL_SLOT_HPP