#ifndef FREEGUI_OBJECT_HPP
#define FREEGUI_OBJECT_HPP

#include "signal_slot.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace FreeGui {

class Object
{
public:
  Object() {};
  virtual ~Object() {};

  template<typename... Args>
  void connectSignal(const std::string& name,
                     const typename Signal<Args...>::Slot& slot)
  {
    // 实现信号连接
  }

protected:
  template<typename... Args>
  Signal<Args...>* createSignal(const std::string& name)
  {
    // 创建信号
  }

private:
  std::unordered_map<std::string, std::unique_ptr<SignalBase>> signals_;
};

} // namespace FreeGui

#endif // FREEGUI_OBJECT_HPP