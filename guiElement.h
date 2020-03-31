#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include<SDL.h>
#include<functional>
#include<memory>
#include"windowWrapper.h"

class GuiElement {
public:
  GuiElement(std::shared_ptr<WindowWrapper> window, int x, int y, int width, int height): frame({x,y,width,height}),
  parentWindow(window) { }
  GuiElement(const GuiElement&) = default;
  GuiElement(GuiElement&&) = default;
  GuiElement& operator=(const GuiElement&) = default;
  GuiElement& operator=(GuiElement&&) = default;
  virtual ~GuiElement() = default;
  void setX(int x) { frame.x = x; }
  void setY(int y) { frame.y = y; }
  int getX() const { return frame.x; }
  int getY() const { return frame.y; }
  void show() { shown = true; }
  void hide() { shown = false; }
  int getElementId() const { return elementId; }
  void setElementId(int id) { elementId = id; }
  int getWidth() const { return frame.w; }
  int getHeight() const { return frame.h; }
  void setLeftClickCallback(std::function<void(const GuiElement&)> callback)
    { leftClickCallback = callback; }
  void setRightClickCallback(std::function<void(const GuiElement&)> callback)
    { rightClickCallback = callback; }
  virtual void setBackgroundColor(const SDL_Color&);
  virtual void setBorderColor(const SDL_Color&);
  virtual void render() const = 0;
protected:
  void rightClick() const;
  void leftClick() const;
  SDL_Rect frame;
  bool shown = true;
  bool borderColorIsSet = false;
  bool backgroundColorIsSet = false;
  int elementId;
  SDL_Color borderColor;
  SDL_Color backgroundColor;
  std::function<void(const GuiElement&)> leftClickCallback;
  std::function<void(const GuiElement&)> rightClickCallback;
  std::shared_ptr<WindowWrapper> parentWindow;
};

#endif