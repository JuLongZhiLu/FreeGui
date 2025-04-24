#include "FreeGui/widgets/button.hpp"
#include "windows.h"

namespace FreeGui {

Button::Button(const std::string& label, HWND parentHandle)
  : label_(label)
{
#ifdef FREEGUI_PLATFORM_WINDOWS
  HINSTANCE hInstance = GetModuleHandle(nullptr);

  HWND hwnd =
    CreateWindow("BUTTON",
                 label.c_str(),
                 WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                 10,
                 10,
                 100,
                 30,
                 parentHandle,
                 nullptr,
                 hInstance,
                 nullptr);

  if (hwnd) {
    nativeHandle_ = hwnd;
    ShowWindow(hwnd, SW_SHOW);
  }
#endif
}

Button::~Button()
{
#ifdef FREEGUI_PLATFORM_WINDOWS
  if (nativeHandle_) {
    DestroyWindow((HWND)nativeHandle_);
    nativeHandle_ = nullptr;
  }
#endif
}

// void
// Button::createNativeButton()
// {
// #ifdef FREEGUI_PLATFORM_WINDOWS
//   HINSTANCE hInstance = GetModuleHandle(nullptr);

//   HWND hwnd =
//     CreateWindow("BUTTON",
//                  label_.c_str(),
//                  WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
//                  10,
//                  10,
//                  100,
//                  30,
//                  (HWND)GetParent((HWND)nativeHandle_),
//                  nullptr,
//                  hInstance,
//                  nullptr);

//   if (hwnd) {
//     nativeHandle_ = hwnd;
//     ShowWindow(hwnd, SW_SHOW);
//     SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
//     SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)ButtonProc);
//   }
// #endif
// }

void
Button::setLabel(const std::string& label)
{
  label_ = label;
#ifdef FREEGUI_PLATFORM_WINDOWS
  if (nativeHandle_) {
    SetWindowText((HWND)nativeHandle_, label_.c_str());
  }
#endif
}

std::string
Button::getLabel() const
{
  return label_;
}

#ifdef FREEGUI_PLATFORM_WINDOWS
LRESULT CALLBACK
Button::ButtonProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  Button* button =
    reinterpret_cast<Button*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  if (button) {
    switch (msg) {
      case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED) {
          button->handleClickEvent();
        }
        break;
    }
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif

} // namespace FreeGui