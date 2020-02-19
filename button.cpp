#include"button.h"
#include"collisionDetector.h"

Button &Button::setText(WindowWrapper &w, const Font &font, const std::string &t)
{
  text.loadText(w,font,t);
  frame.w += text.getWidth();
  frame.h += text.getHeight();
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
  text.render(w,frame.x+rlPadding,frame.y+tbPadding);
  w.setColor(prev);
}

void Button::leftClick(SDL_Event &e)
{
    if(e.button.button == SDL_BUTTON_LEFT && isCollide({e.button.x,e.button.y},frame))
      if(leftClickCallback)
        leftClickCallback(*this);
}

void Button::rightClick(SDL_Event &e)
{
    if(e.button.button == SDL_BUTTON_LEFT && isCollide({e.button.x,e.button.y},frame))
      if(rightClickCallback)
        rightClickCallback(*this);
}

void Button::mouseMove(SDL_Event &e)
{
    if(backgroundIsSet){
      if(isCollide({e.motion.x,e.motion.y},frame)){
        activeBackground = hoverColor;
        hover = true;
      }
      else if(hover)
        activeBackground = backgroundColor;
    }
}
