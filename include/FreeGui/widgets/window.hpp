#ifndef FREEGUI_WINDOW_HPP
#define FREEGUI_WINDOW_HPP

#include "widget.hpp"
#include "windows.h"
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

  // 窗口事件信号
  Signal<> closed;
  Signal<int, int> resized;
  Signal<int, int> moved;
  Signal<bool> focusChanged;

protected:
  // 平台相关实现
  void createNativeWindow();
  void destroyNativeWindow();

public:
// 添加平台事件处理函数
#ifdef FREEGUI_PLATFORM_WINDOWS
  static LRESULT CALLBACK WindowProc(HWND hwnd,
                                     UINT msg,
                                     WPARAM wParam,
                                     LPARAM lParam);
#endif
  void processPlatformEvents();
  void* getNativeHandle() { return nativeHandle_; };

protected:
  virtual void handleCloseEvent() { closed.emit(); }
  virtual void handleResizeEvent(int width, int height)
  {
    resized.emit(width, height);
  }
  virtual void handleMoveEvent(int x, int y) { moved.emit(x, y); }
  virtual void handleFocusEvent(bool hasFocus) { focusChanged.emit(hasFocus); }

private:
  void* nativeHandle_ = nullptr; // 平台相关的窗口句柄
  std::string title_;
  std::vector<Widget*> children_; // 子控件列表

public:
  void addChild(Widget* widget);
  void removeChild(Widget* widget);
};

} // namespace FreeGui

#endif // FREEGUI_WINDOW_HPP