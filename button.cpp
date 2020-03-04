#include"button.h"
#include"collisionDetector.h"
#include<iostream>

void Button::setText(WindowWrapper &w, const Font &font, const std::string &t)
{
  int textWidth = font.getTextWH(t).first;
  std::string newText = t;
  if(textAreaWidth != -1){
    frame.w += textAreaWidth;
    if(textWidth >= textAreaWidth){
      int perChar = textWidth/newText.length();
      int numOfChars = textAreaWidth/perChar;
      newText = newText.substr(0,numOfChars-4);
      newText.append("...");
    }
  }
  else
    frame.w += textWidth;
  text.loadText(w,font,newText);
  if(iconIsSet){
    if(text.getHeight() > icon.getHeight())
      frame.h += text.getHeight() - icon.getHeight();
    textOffsetY = icon.getHeight()/2 - text.getHeight()/2;
  }
  else {
    frame.h += text.getHeight();
  }
  textIsSet = true;
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

void Button::setHoverColor(SDL_Color color)
{
  hoverColor = color;
  hoverIsSet = true;
}

std::pair<int,int> Button::setIcon(WindowWrapper &w, Texture iconTexture,  int maxDimension, int iconRightPadding)
{
  if(iconTexture.getWidth() != maxDimension || iconTexture.getHeight() != maxDimension){
    if(iconTexture.getWidth() > iconTexture.getHeight()){
      double ratio = iconTexture.getHeight()/static_cast<double>(iconTexture.getWidth());
      iconTexture.resize(w,maxDimension,maxDimension*ratio);
    }
    else if(iconTexture.getWidth() < iconTexture.getHeight()){
      double ratio = iconTexture.getWidth()/static_cast<double>(iconTexture.getHeight());
      iconTexture.resize(w,maxDimension*ratio,maxDimension);
    }
    else{
      iconTexture.resize(w,maxDimension,maxDimension);
    }
  }
  icon = iconTexture;
  if(text.getHeight() < iconTexture.getHeight()){
    frame.h += iconTexture.getHeight() - text.getHeight();
  }
  frame.w += iconTexture.getWidth() + rightPadding;
  textOffsetX = iconTexture.getWidth() + rightPadding;
  textOffsetY = iconTexture.getHeight()/2 - text.getHeight()/2;
  iconIsSet = true;
  return std::make_pair(iconTexture.getWidth(),iconTexture.getHeight());
}

void Button::setTopPadding(int padding)
{
  topPadding = padding;
  frame.h += padding;
}
void Button::setBottomPadding(int padding)
{
  bottomPadding = padding;
  frame.h += padding;
}
void Button::setRightPadding(int padding)
{
  rightPadding = padding;
  frame.w += padding;
}
void Button::setLeftPadding(int padding)
{
  leftPadding = padding;
  frame.w += padding;
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

void Button::render(WindowWrapper &w, SDL_Rect camera) const
{
  SDL_Color prev = w.getColor();
  int relativeX = frame.x-camera.x;
  int relativeY = frame.y-camera.y;
  if(relativeX+frame.w <= 0 || relativeX >= camera.w)
    return;
  if(relativeY+frame.h <= 0 || relativeY >= camera.h)
    return;
  SDL_Rect relativeFrame = {relativeX,relativeY,frame.w,frame.h};
  if(backgroundIsSet){
    w.setColor(activeBackground);
    SDL_RenderFillRect(w.getRenderer(),&relativeFrame);
  }
  if(borderIsSet){
    w.setColor(borderColor);
    SDL_RenderDrawRect(w.getRenderer(),&relativeFrame);
  }
  if(iconIsSet)
    icon.render(w,frame.x+leftPadding-camera.x,frame.y+topPadding-camera.y);
  if(textIsSet)
    text.render(w,frame.x+leftPadding+textOffsetX-camera.x,frame.y+topPadding+textOffsetY-camera.y);
  w.setColor(prev);
}

int Button::leftClick(const SDL_Event &e) const
{
  if(e.button.button == SDL_BUTTON_LEFT && isCollide({e.button.x,e.button.y},frame))
    if(leftClickCallback){
      leftClickCallback(*this);
      return 1;
    }
  return 0;
}

int Button::leftClick(const SDL_Event &e, SDL_Rect camera) const
{
  if(e.button.button == SDL_BUTTON_LEFT && isCollide({e.button.x+camera.x,e.button.y+camera.y},frame))
    if(leftClickCallback){
      leftClickCallback(*this);
      return 1;
    }
  return 0;
}

int Button::rightClick(const SDL_Event &e) const
{
  if(e.button.button == SDL_BUTTON_RIGHT && isCollide({e.button.x,e.button.y},frame))
    if(rightClickCallback){
      rightClickCallback(*this);
      return 1;
    }
  return 0;
}

int Button::rightClick(const SDL_Event &e, SDL_Rect camera) const
{
  if(e.button.button == SDL_BUTTON_RIGHT && isCollide({e.button.x+camera.x,e.button.y+camera.y},frame))
    if(rightClickCallback){
      rightClickCallback(*this);
      return 1;
    }
  return 0;
}

void Button::mouseMove(const SDL_Event &e)
{
  if(backgroundIsSet && hoverIsSet){
    int x,y;
    SDL_GetMouseState(&x,&y);
    if(isCollide({x,y},frame)){
      activeBackground = hoverColor;
      hover = true;
    }
    else if(hover)
      activeBackground = backgroundColor;
  }
}

void Button::mouseMove(const SDL_Event &e, SDL_Rect camera)
{
  if(backgroundIsSet && hoverIsSet){
    int x,y;
    SDL_GetMouseState(&x,&y);
    if(isCollide({x+camera.x,y+camera.y},frame)){
      activeBackground = hoverColor;
      hover = true;
    }
    else if(hover)
      activeBackground = backgroundColor;
  }
}