#ifndef FREEGUI_WINDOW_HPP
#define FREEGUI_WINDOW_HPP

#include "widget.hpp"
#include <string>

namespace FreeGui {

class Window : public Widget
{
public:
  explicit Window(const std::string& title = "FreeGui Window");
  virtual ~Window();

  void show();
  void hide();
  void setTitle(const std::string& title);

protected:
  // 平台相关实现
  void createNativeWindow();
  void destroyNativeWindow();

private:
  void* nativeHandle_ = nullptr; // 平台相关的窗口句柄
  std::string title_;
};

} // namespace FreeGui

#endif // FREEGUI_WINDOW_HPP