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
  background = true;
}

void Button::render(WindowWrapper &w) const
{
  SDL_Color prev;
  SDL_GetRenderDrawColor(w.getRenderer(),&prev.r,&prev.g,&prev.b,&prev.a);  
  if(background){
    SDL_SetRenderDrawColor(w.getRenderer(),activeBackground.r,activeBackground.g,
        activeBackground.b,activeBackground.a);
    SDL_RenderFillRect(w.getRenderer(),&border);
  }
  SDL_SetRenderDrawColor(w.getRenderer(),borderColor.r,borderColor.g,borderColor.b,borderColor.a);
  SDL_RenderDrawRect(w.getRenderer(),&border);
  text.render(w,border.x+rlPadding,border.y+tbPadding);
  SDL_SetRenderDrawColor(w.getRenderer(),prev.r,prev.g,prev.b,prev.a);
}

int Button::click(SDL_Event &e)
{
    if(e.button.button == SDL_BUTTON_LEFT && isCollide({e.button.x,e.button.y},border))
      return actionCode;
    return 0;
}
void Button::mouseMove(SDL_Event &e)
{
    if(background){
      if(isCollide({e.motion.x,e.motion.y},border)){
        activeBackground = hoverColor;
        hover = true;
      }
      else if(hover)
        activeBackground = backgroundColor;
    }
}
