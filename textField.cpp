#include"textField.h"

void TextField::render() const
{
  if(!shown)
    return;
  SDL_Color prev = parentWindow->getColor();
  if(borderColorIsSet){
      parentWindow->setColor(borderColor);
      parentWindow->drawRect(frame);
  }
  if(backgroundColorIsSet){
      parentWindow->setColor(backgroundColor);
      parentWindow->fillRect(frame);
  }
  int cursorX = frame.x+5+textTexture.getWidth();
  if(!empty){
    int rightPadding = 10+cursor.getWidth();
    if(textTexture.getWidth() > frame.w-rightPadding){
      SDL_Rect lastChars = {textTexture.getWidth()-frame.w+rightPadding,0,frame.w-rightPadding,textTexture.getHeight()};
      textTexture.render(parentWindow->getRenderer(),frame.x+5,frame.y+(frame.h-textTexture.getHeight())/2,&lastChars);
      cursorX = frame.x+5+lastChars.w;
    } else {
      textTexture.render(parentWindow->getRenderer(),frame.x+5,frame.y+(frame.h-textTexture.getHeight())/2);
    }
  }
  if(animationTimer.getTicks() >= 2000)
    animationTimer.start();
  else if(animationTimer.getTicks() >= 1000)
    cursor.render(parentWindow->getRenderer(),cursorX,frame.y+(frame.h-cursor.getHeight())/2);
  parentWindow->setColor(prev);
}

void TextField::regenerateText()
{
  cursor.loadText(parentWindow->getRenderer(),textFont,"_");
  if(!text.length())
    empty = true;
  else
    textTexture.loadText(parentWindow->getRenderer(),textFont,text);
}