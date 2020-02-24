#include"listMenu.h"
#include<iostream>
#include"collisionDetector.h"

ListMenu::ListMenu(int x, int y, int tbPadding, int rlPadding):
    mainButton(x,y,tbPadding,rlPadding), currentY(y+mainButton.getHeight()), frame({x,currentY,0,0}) { }

void ListMenu::setListButtonClickCallback(std::function<void()> callback)
{
  mainButton.setLeftClickCallback([callback,this] (const Button&)->void {
      if(shown) shown = false;
      else shown = true;
      callback();
    });
}

void ListMenu::setTitle(WindowWrapper &w, const Font &font, const std::string &title)
{
  mainButton.setText(w,font,title);
  currentY = mainButton.getHeight() + mainButton.getY();
  frame.y = currentY;
}

void ListMenu::addEntry(WindowWrapper &w, const Font &font, const std::string &text,
                            std::function<void(const Button&)> callback)
{
  menuButtons.emplace_back(frame.x,currentY,mainButton.getTopPadding(),mainButton.getLeftPadding());
  Button &current = menuButtons.back();
  current.setText(w,font,text);
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
  if(backgroundIsSet)
    current.setBackgroundColor(background);
  if(hoverIsSet)
    current.setHoverColor(hover);
  if(borderIsSet)
    current.setBorderColor(border);
}

void ListMenu::setBackgroundColor(SDL_Color color)
{
  mainButton.setBackgroundColor(color);
  for(auto &b : menuButtons)
    b.setBackgroundColor(color);
  background = color;
  backgroundIsSet = true;
}

void ListMenu::setBorderColor(SDL_Color color)
{
  mainButton.setBorderColor(color);
  for(auto &b : menuButtons)
    b.setBorderColor(color);
  border = color;
  borderIsSet = true;
}

void ListMenu::setHoverColor(SDL_Color color)
{
  mainButton.setHoverColor(color);
  for(auto &b : menuButtons)
    b.setHoverColor(color);
  hover = color;
  hoverIsSet = true;
}

void ListMenu::render(WindowWrapper &w) const
{
  mainButton.render(w);
  if(shown){
    for(const auto &b : menuButtons)
      b.render(w);
  }
}

void ListMenu::mouseMove(const SDL_Event &e)
{
  mainButton.mouseMove(e);
    for(auto &b : menuButtons)
      b.mouseMove(e);
}

int ListMenu::leftClick(const SDL_Event &e)
{
  if(mainButton.leftClick(e))
    return 1;
  if(shown && isCollide({e.button.x,e.button.y},frame)){
      for(auto &b : menuButtons)
        if(b.leftClick(e)) return 1;
  }
  return 0;
}