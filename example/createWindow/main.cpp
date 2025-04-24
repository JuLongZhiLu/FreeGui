#include "FreeGui/widgets/button.hpp"
#include "FreeGui/widgets/window.hpp"
#include "windows.h"
#include <iostream>

int
main()
{
  FreeGui::Window window("MyWindow");
  window.setTitle("Window");
  // 连接信号
  window.resized.connect([](int w, int h) {
    std::cout << "Window resized to: " << w << "x" << h << std::endl;
  });
  window.closed.connect([]() {
    std::cout << "Window closed." << std::endl;
    // 在这里添加退出应用程序的逻辑
    // 例如，调用exit(0);
    exit(0);
  });
  window.show();

  // 创建按钮
  FreeGui::Button button("Click Me", (HWND)window.getNativeHandle());
  button.clicked.connect([]() { std::cout << "Button clicked!" << std::endl; });
  window.addChild(&button);

  // 主循环
  while (true) {
    window.processPlatformEvents(); // 处理事件
                                    // 可以添加Sleep或其他逻辑
  }
  return 0;
}