#include"textField.h"

void TextField::render(WindowWrapper &w) const
{
  if(!shown)
    return;
  SDL_Color prev = w.getColor();
  if(borderColorIsSet){
      w.setColor(borderColor);
      w.drawRect(frame);
  }
  if(backgroundColorIsSet){
      w.setColor(backgroundColor);
      w.fillRect(frame);
  }
  if(!empty){
    if(textTexture.getWidth() > frame.w-10){
      SDL_Rect lastChars = {textTexture.getWidth()-frame.w+10,0,frame.w-10,textTexture.getHeight()};
      textTexture.render(w,frame.x+5,frame.y+(frame.h-textTexture.getHeight())/2,&lastChars);
    } else {
      textTexture.render(w,frame.x+5,frame.y+(frame.h-textTexture.getHeight())/2);
    }
  }
  w.setColor(prev);
}

void TextField::regenerateText(WindowWrapper &w)
{
  if(!text.length())
    empty = true;
  else
    textTexture.loadText(w,textFont,text);
}