#include"button.h"
#include"collisionDetector.h"
#include<iostream>

Button &Button::setText(WindowWrapper &w, const Font &font, const std::string &t)
{
  text.loadText(w,font,t);
  frame.w += text.getWidth();
  frame.h += text.getHeight();
  textIsSet = true;
  return *this;
}

Button &Button::setBackgroundColor(SDL_Color color)
{
  backgroundColor = color;
  activeBackground = backgroundColor;
  backgroundIsSet = true;
  return *this;
}

Button &Button::setBorderColor(SDL_Color color)
{
   borderColor = color;
   borderIsSet = true;
   return *this;
}

Button &Button::setHoverColor(SDL_Color color)
{
  hoverColor = color;
  hoverIsSet = true;
  return *this;
}

Button &Button::setIcon(const Texture &iconTexture, int iconRightPadding)
{
  if(iconTexture.getWidth() > 32 || iconTexture.getHeight() > 32){
    std::cout << "Only support icons up to 32 pixels" << std::endl;
    return *this;
  }
  icon = iconTexture;
  if(text.getHeight() < iconTexture.getHeight()){
    frame.h += iconTexture.getHeight() - text.getHeight();
  }
  frame.w += iconTexture.getWidth() + rightPadding;
  textOffsetX = iconTexture.getWidth() + rightPadding;
  textOffsetY = iconTexture.getHeight()/2 - text.getHeight()/2;
  iconIsSet = true;
  return *this;
}

Button &Button::setTopPadding(int padding)
{
  topPadding = padding;
  frame.h += padding;
  return *this;
}
Button &Button::setBottomPadding(int padding)
{
  bottomPadding = padding;
  frame.h += padding;
  return *this;
}
Button &Button::setRightPadding(int padding)
{
  rightPadding = padding;
  frame.w += padding;
  return *this;
}
Button &Button::setLeftPadding(int padding)
{
  leftPadding = padding;
  frame.w += padding;
  return *this;
}

void Button::render(WindowWrapper &w) const
{
  SDL_Color prev = w.getColor();
  if(backgroundIsSet){
    w.setColor(activeBackground);
    SDL_RenderFillRect(w.getRenderer(),&frame);
  }
  if(borderIsSet){
    w.setColor(borderColor);
    SDL_RenderDrawRect(w.getRenderer(),&frame);
  }
  if(iconIsSet)
    icon.render(w,frame.x+leftPadding,frame.y+topPadding);
  if(textIsSet)
    text.render(w,frame.x+leftPadding+textOffsetX,frame.y+topPadding+textOffsetY);
  w.setColor(prev);
}

int Button::leftClick(SDL_Event &e) const
{
  if(e.button.button == SDL_BUTTON_LEFT && isCollide({e.button.x,e.button.y},frame))
    if(leftClickCallback){
      leftClickCallback(*this);
      return 1;
    }
  return 0;
}

int Button::rightClick(SDL_Event &e) const
{
  if(e.button.button == SDL_BUTTON_RIGHT && isCollide({e.button.x,e.button.y},frame))
    if(rightClickCallback){
      rightClickCallback(*this);
      return 1;
    }
  return 0;
}

void Button::mouseMove(SDL_Event &e)
{
  if(backgroundIsSet && hoverIsSet){
    if(isCollide({e.motion.x,e.motion.y},frame)){
      activeBackground = hoverColor;
      hover = true;
    }
    else if(hover)
      activeBackground = backgroundColor;
  }
}
