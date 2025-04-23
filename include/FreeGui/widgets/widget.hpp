#ifndef FREEGUI_WIDGET_HPP
#define FREEGUI_WIDGET_HPP

#include "../core/object.hpp"

namespace FreeGui {

class Widget : public Object
{
public:
  Widget() {};
  virtual ~Widget() {};

  virtual void show() = 0;
  virtual void hide() = 0;
};

} // namespace FreeGui

#endif // FREEGUI_WIDGET_HPP