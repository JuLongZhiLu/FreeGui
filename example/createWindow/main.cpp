#include "widgets/window.hpp"
#include "windows.h"

int
main()
{
  FreeGui::Window window("MyWindow");
  window.show();

  // 进入消息循环
#ifdef FREEGUI_PLATFORM_WINDOWS
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
#elif defined(FREEGUI_PLATFORM_LINUX)
  Display* display = XOpenDisplay(nullptr);
  XEvent event;
  while (true) {
    XNextEvent(display, &event);
    // 处理事件...
  }
#endif

  return 0;
}