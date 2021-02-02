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
  int cursorX = frame.x+5;
  if(!empty){
    int rightPadding = 10+cursor.getWidth();
    if(textTexture.getWidth() > frame.w-rightPadding){
      SDL_Rect lastChars = {textTexture.getWidth()-frame.w+rightPadding,0,frame.w-rightPadding,textTexture.getHeight()};
      textTexture.render(*parentWindow,frame.x+5,frame.y+(frame.h-textTexture.getHeight())/2,&lastChars);
      cursorX += lastChars.w;
    } else {
      textTexture.render(*parentWindow,frame.x+5,frame.y+(frame.h-textTexture.getHeight())/2);
      cursorX += textTexture.getWidth();
    }
  }
  if(animationTimer.getTicks() >= 2000)
    animationTimer.start();
  else if(animationTimer.getTicks() >= 1000)
    cursor.render(*parentWindow,cursorX,frame.y+(frame.h-cursor.getHeight())/2);
  parentWindow->setColor(prev);
}

void TextField::regenerateText()
{
  cursor.loadText(*parentWindow,textFont,"_");
  if(!text.length())
    empty = true;
  else
    textTexture.loadText(*parentWindow,textFont,text);
}