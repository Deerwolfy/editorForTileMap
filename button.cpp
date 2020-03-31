#include"button.h"
#include"collisionDetector.h"
#include<iostream>

void Button::setText(const Font &font, const std::string &t)
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
  text.loadText(parentWindow,font,newText);
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

void Button::setBackgroundColor(const SDL_Color &color)
{
  backgroundColor = color;
  activeBackground = backgroundColor;
  backgroundColorIsSet = true;
}

void Button::setHoverColor(const SDL_Color &color)
{
  hoverColor = color;
  hoverIsSet = true;
}

std::pair<int,int> Button::setIcon(Texture iconTexture,  int maxDimension, int iconRightPadding)
{
  if(iconTexture.getWidth() != maxDimension || iconTexture.getHeight() != maxDimension){
    if(iconTexture.getWidth() > iconTexture.getHeight()){
      double ratio = iconTexture.getHeight()/static_cast<double>(iconTexture.getWidth());
      iconTexture.resize(parentWindow,maxDimension,maxDimension*ratio);
    }
    else if(iconTexture.getWidth() < iconTexture.getHeight()){
      double ratio = iconTexture.getWidth()/static_cast<double>(iconTexture.getHeight());
      iconTexture.resize(parentWindow,maxDimension*ratio,maxDimension);
    }
    else{
      iconTexture.resize(parentWindow,maxDimension,maxDimension);
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

void Button::render() const
{
  if(!shown)
    return;
  SDL_Color prev = parentWindow->getColor();
  if(backgroundColorIsSet){
    parentWindow->setColor(activeBackground);
    SDL_RenderFillRect(parentWindow->getRenderer(),&frame);
  }
  if(borderColorIsSet){
    parentWindow->setColor(borderColor);
    SDL_RenderDrawRect(parentWindow->getRenderer(),&frame);
  }
  if(iconIsSet)
    icon.render(parentWindow,frame.x+leftPadding,frame.y+topPadding);
  if(textIsSet)
    text.render(parentWindow,frame.x+leftPadding+textOffsetX,frame.y+topPadding+textOffsetY);
  parentWindow->setColor(prev);
}

void Button::render(SDL_Rect camera) const
{
  if(!shown)
    return;
  int relativeX = frame.x-camera.x;
  int relativeY = frame.y-camera.y;
  if(frame.x+frame.w <= camera.x || frame.x >= camera.x+camera.w)
    return;
  if(frame.y+frame.h <= camera.y || frame.y >= camera.y+camera.h)
    return;
  SDL_Color prev = parentWindow->getColor();
  SDL_Rect relativeFrame = {relativeX,relativeY,frame.w,frame.h};
  if(backgroundColorIsSet){
    parentWindow->setColor(activeBackground);
    SDL_RenderFillRect(parentWindow->getRenderer(),&relativeFrame);
  }
  if(borderColorIsSet){
    parentWindow->setColor(borderColor);
    SDL_RenderDrawRect(parentWindow->getRenderer(),&relativeFrame);
  }
  if(iconIsSet)
    icon.render(parentWindow,frame.x+leftPadding-camera.x,frame.y+topPadding-camera.y);
  if(textIsSet)
    text.render(parentWindow,frame.x+leftPadding+textOffsetX-camera.x,frame.y+topPadding+textOffsetY-camera.y);
  parentWindow->setColor(prev);
}

int Button::click(const SDL_Event &e) const
{
  if(isCollide({e.button.x,e.button.y},frame) && shown){
    if(e.button.button == SDL_BUTTON_LEFT)
      leftClick();
    else if(e.button.button == SDL_BUTTON_RIGHT)
      rightClick();
    return 1;
  }
  return 0;
}

int Button::click(const SDL_Event &e, const SDL_Rect &camera) const
{
  SDL_Point mousePos{e.button.x+camera.x,e.button.y+camera.y};
  if(isCollide(mousePos,frame) && shown){
    if(e.button.button == SDL_BUTTON_LEFT)
      leftClick();
    else if(e.button.button == SDL_BUTTON_RIGHT)
      rightClick();
    return 1;
  }
  return 0;
}

void Button::mouseMove(const SDL_Event &e)
{
  if(backgroundColorIsSet && hoverIsSet)
    updateHover({e.motion.x,e.motion.y});
}

void Button::mouseMove(const SDL_Event &e, SDL_Rect camera)
{
  if(backgroundColorIsSet && hoverIsSet){
    SDL_Point mousePos;
    mousePos.x = e.motion.x + camera.x;
    mousePos.y = e.motion.y + camera.y;
    updateHover(mousePos);
  }
}

void Button::updateHover(const SDL_Point &mousePos)
{
  if(isCollide(mousePos,frame)){
      activeBackground = hoverColor;
      hover = true;
    }
    else if(hover)
      activeBackground = backgroundColor;
}