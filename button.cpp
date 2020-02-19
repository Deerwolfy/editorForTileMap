#include"button.h"
#include"collisionDetector.h"

void Button::setText(WindowWrapper &w, const Font &font, const std::string &t)
{
  text.loadText(w,font,t);
  border.w += text.getWidth();
  border.h += text.getHeight();
}

void Button::setBackgroundColor(SDL_Color color)
{
  backgroundColor = color;
  activeBackground = backgroundColor;
  backgroundIsSet = true;
}

void Button::setBorderColor(SDL_Color color)
{
   borderColor = color;
   borderIsSet = true;
}

void Button::render(WindowWrapper &w) const
{
  SDL_Color prev = w.getColor();
  if(backgroundIsSet){
    w.setColor(activeBackground);
    SDL_RenderFillRect(w.getRenderer(),&border);
  }
  if(borderIsSet){
    w.setColor(borderColor);
    SDL_RenderDrawRect(w.getRenderer(),&border);
  }
  text.render(w,border.x+rlPadding,border.y+tbPadding);
  w.setColor(prev);
}

int Button::click(SDL_Event &e)
{
    if(e.button.button == SDL_BUTTON_LEFT && isCollide({e.button.x,e.button.y},border))
      return actionCode;
    return 0;
}
void Button::mouseMove(SDL_Event &e)
{
    if(backgroundIsSet){
      if(isCollide({e.motion.x,e.motion.y},border)){
        activeBackground = hoverColor;
        hover = true;
      }
      else if(hover)
        activeBackground = backgroundColor;
    }
}
