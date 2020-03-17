#include"guiElement.h"

void GuiElement::setBackgroundColor(const SDL_Color &color)
{
    backgroundColorIsSet = true;
    backgroundColor = color;
}

void GuiElement::setBorderColor(const SDL_Color &color)
{
    borderColorIsSet = true;
    borderColor = color;
}

void GuiElement::leftClick() const
{
  if(leftClickCallback)
    leftClickCallback(*this);
}

void GuiElement::rightClick() const
{
  if(rightClickCallback)
    rightClickCallback(*this);
}