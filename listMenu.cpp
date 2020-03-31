#include"listMenu.h"
#include<iostream>
#include"collisionDetector.h"

ListMenu::ListMenu(std::shared_ptr<WindowWrapper> window, int x, int y, int tbPadding, int rlPadding):
    GuiElement(window, x,currentY,0,0), mainButton(window, x,y,tbPadding,rlPadding), currentY(y+mainButton.getHeight())
{
  mainButton.setLeftClickCallback([this](const GuiElement&)->void {
    toggle();
  });
}

void ListMenu::setListButtonClickCallback(std::function<void()> callback)
{
  mainButton.setLeftClickCallback([callback,this] (const GuiElement&)->void {
      toggle();
      callback();
    });
}

void ListMenu::toggle()
{
  if(open)
    open = false;
  else
    open = true;
  
}

void ListMenu::setTitle(const Font &font, const std::string &title)
{
  mainButton.setText(font,title);
  currentY = mainButton.getHeight() + mainButton.getY();
  frame.y = currentY;
}

void ListMenu::addEntry(const Font &font, const std::string &text,
                            std::function<void(const GuiElement&)> callback)
{
  menuButtons.emplace_back(parentWindow,frame.x,currentY,mainButton.getTopPadding(),mainButton.getLeftPadding());
  Button &current = menuButtons.back();
  current.setText(font,text);
  current.setLeftClickCallback(callback);
  if(frame.w < current.getWidth()){
    frame.w = current.getWidth();
    for(auto &b : menuButtons){
      b.setRightPadding(frame.w - b.getWidth());
    }
  }
  else
    current.setRightPadding(frame.w - current.getWidth());
  currentY += current.getHeight();
  frame.h += current.getHeight();
  if(backgroundColorIsSet)
    current.setBackgroundColor(backgroundColor);
  if(hoverIsSet)
    current.setHoverColor(hover);
  if(borderColorIsSet)
    current.setBorderColor(borderColor);
}

void ListMenu::setBackgroundColor(const SDL_Color &color)
{
  mainButton.setBackgroundColor(color);
  for(auto &b : menuButtons)
    b.setBackgroundColor(color);
  backgroundColor = color;
  backgroundColorIsSet = true;
}

void ListMenu::setBorderColor(const SDL_Color &color)
{
  mainButton.setBorderColor(color);
  for(auto &b : menuButtons)
    b.setBorderColor(color);
  borderColor = color;
  borderColorIsSet = true;
}

void ListMenu::setHoverColor(SDL_Color color)
{
  mainButton.setHoverColor(color);
  for(auto &b : menuButtons)
    b.setHoverColor(color);
  hover = color;
  hoverIsSet = true;
}

void ListMenu::render() const
{
  if(!shown)
    return;
  mainButton.render();
  if(open){
    for(const auto &b : menuButtons)
      b.render();
  }
}

void ListMenu::mouseMove(const SDL_Event &e)
{
  mainButton.mouseMove(e);
    for(auto &b : menuButtons)
      b.mouseMove(e);
}

void ListMenu::click(const SDL_Event &e)
{
  if(!shown)
    return;
  if(mainButton.click(e))
    return;
  if(open){
    if(isCollide({e.button.x,e.button.y},frame)){
        for(auto &b : menuButtons)
          if(b.click(e)){
            open = false;
            return;
          }
    } else {
      open = false;
    }
  }
}
