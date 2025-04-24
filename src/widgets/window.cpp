#include "FreeGui/widgets/window.hpp"
#include "FreeGui/core/object.hpp"
#include "FreeGui/widgets/widget.hpp"

#ifdef FREEGUI_PLATFORM_WINDOWS
#include <Windows.h>
#elif defined(FREEGUI_PLATFORM_LINUX)
#include <X11/Xlib.h>
#endif

namespace FreeGui {

Window::Window(const std::string& title)
  : title_(title)
{
  createNativeWindow();
}

Window::~Window()
{
  destroyNativeWindow();
}

void
Window::createNativeWindow()
{
#ifdef FREEGUI_PLATFORM_WINDOWS
  // Windows平台窗口创建
  HINSTANCE hInstance = GetModuleHandle(nullptr);

  WNDCLASSEX wc = { 0 };
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WindowProc; // 修改这里
  wc.hInstance = hInstance;
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.lpszClassName = "FreeGuiWindowClass";

  RegisterClassEx(&wc);

  nativeHandle_ = CreateWindowEx(0,
                                 "FreeGuiWindowClass",
                                 title_.c_str(),
                                 WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT,
                                 CW_USEDEFAULT,
                                 800,
                                 600,
                                 nullptr,
                                 nullptr,
                                 hInstance,
                                 nullptr);

  if (nativeHandle_) {
    ShowWindow((HWND)nativeHandle_, SW_SHOW);
    UpdateWindow((HWND)nativeHandle_);
    // 设置窗口用户数据
    SetWindowLongPtr((HWND)nativeHandle_, GWLP_USERDATA, (LONG_PTR)this);
  }
#elif defined(FREEGUI_PLATFORM_LINUX)
  // Linux平台窗口创建
  Display* display = XOpenDisplay(nullptr);
  if (!display)
    return;

  int screen = DefaultScreen(display);
  Window root = RootWindow(display, screen);

  nativeHandle_ = XCreateSimpleWindow(display,
                                      root,
                                      0,
                                      0,
                                      800,
                                      600,
                                      1,
                                      BlackPixel(display, screen),
                                      WhitePixel(display, screen));

  XStoreName(display, (::Window)nativeHandle_, title_.c_str());
  XMapWindow(display, (::Window)nativeHandle_);
  XFlush(display);
#endif
}

void
Window::destroyNativeWindow()
{
#ifdef FREEGUI_PLATFORM_WINDOWS
  if (nativeHandle_) {
    DestroyWindow((HWND)nativeHandle_);
    nativeHandle_ = nullptr;
  }
#elif defined(FREEGUI_PLATFORM_LINUX
  if (nativeHandle_) {
    Display* display = XOpenDisplay(nullptr);
    if (display) {
      XDestroyWindow(display, (::Window)nativeHandle_);
      XCloseDisplay(display);
    }
    nativeHandle_ = nullptr;
  }
#endif
}

void
Window::show()
{
#ifdef FREEGUI_PLATFORM_WINDOWS
  if (nativeHandle_) {
    ShowWindow((HWND)nativeHandle_, SW_SHOW);
  }
#elif defined(FREEGUI_PLATFORM_LINUX)
  if (nativeHandle_) {
    Display* display = XOpenDisplay(nullptr);
    if (display) {
      XMapWindow(display, (::Window)nativeHandle_);
      XFlush(display);
      XCloseDisplay(display);
    }
  }
#endif
}

void
Window::hide()
{
#ifdef FREEGUI_PLATFORM_WINDOWS
  if (nativeHandle_) {
    ShowWindow((HWND)nativeHandle_, SW_HIDE);
  }
#elif defined(FREEGUI_PLATFORM_LINUX)
  if (nativeHandle_) {
    Display* display = XOpenDisplay(nullptr);
    if (display) {
      XUnmapWindow(display, (::Window)nativeHandle_);
      XFlush(display);
      XCloseDisplay(display);
    }
  }
#endif
}

void
Window::setTitle(const std::string& title)
{
  title_ = title;
#ifdef FREEGUI_PLATFORM_WINDOWS
  if (nativeHandle_) {
    SetWindowText((HWND)nativeHandle_, title_.c_str());
  }
#elif defined(FREEGUI_PLATFORM_LINUX)
  if (nativeHandle_) {
    Display* display = XOpenDisplay(nullptr);
    if (display) {
      XStoreName(display, (::Window)nativeHandle_, title_.c_str());
      XFlush(display);
      XCloseDisplay(display);
    }
  }
#endif
}

#ifdef FREEGUI_PLATFORM_WINDOWS
LRESULT CALLBACK
Window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  Window* window =
    reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

  if (window) {
    switch (msg) {
      case WM_SIZE:
        window->resized.emit(LOWORD(lParam), HIWORD(lParam));
        break;
      case WM_CLOSE:
        window->closed.emit();
        break;
        // 其他事件处理...
    }
  }
  return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif

void
Window::processPlatformEvents()
{
#ifdef FREEGUI_PLATFORM_WINDOWS
  MSG msg;
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
#endif
}

void
Window::addChild(Widget* widget)
{
  children_.push_back(widget);
}

void
Window::removeChild(Widget* widget)
{
  children_.erase(std::remove(children_.begin(), children_.end(), widget),
                  children_.end());
}

}