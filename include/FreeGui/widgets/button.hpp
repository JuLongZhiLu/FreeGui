#ifndef FREEGUI_BUTTON_HPP
#define FREEGUI_BUTTON_HPP

#include "widget.hpp"
#include "windows.h"
#include <string>

namespace FreeGui {

class Button : public Widget
{
public:
  explicit Button(const std::string& label = "Button",
                  HWND parentHandle = nullptr);
  virtual ~Button();

  void setLabel(const std::string& label);
  std::string getLabel() const;

  // 按钮事件信号
  Signal<> clicked;

  void show() override
  {
    // 在这里添加显示按钮的具体实现
    // 例如，调用Windows API显示按钮
    ShowWindow((HWND)nativeHandle_, SW_SHOW);
  }
  void hide() override
  {
    // 在这里添加隐藏按钮的具体实现
    // 例如，调用Windows API隐藏按钮
    ShowWindow((HWND)nativeHandle_, SW_HIDE);
  }

  LRESULT CALLBACK ButtonProc(HWND hwnd,
                              UINT msg,
                              WPARAM wParam,
                              LPARAM lParam);

public:
  virtual void handleClickEvent() { clicked.emit(); }
  void* nativeHandle_ = nullptr;

private:
  std::string label_;
};

} // namespace FreeGui

#endif // FREEGUI_BUTTON_HPP