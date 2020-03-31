#include"selectionBox.h"

void SelectionBox::setEnd(int x, int y)
{
  if(startPoint.x < x){
    frame.x = startPoint.x;
    frame.w = x - startPoint.x;
  } else {
    frame.x = x;
    frame.w = startPoint.x - x;
  }
  if(startPoint.y < y){
    frame.y = startPoint.y;
    frame.h = y - startPoint.y;
  } else {
    frame.y = y;
    frame.h = startPoint.y - y;
  }
}

void SelectionBox::setStart(int x, int y)
{
  startPoint.x = x;
  startPoint.y = y;
  frame.x = x;
  frame.y = y;
  frame.w = 0;
  frame.h = 0;
  hold = true;
}

void SelectionBox::render() const
{
  SDL_Color prev = parentWindow->getColor();
  parentWindow->setColor(borderColor);
  parentWindow->drawRect(frame);
  parentWindow->setColor(backgroundColor);
  parentWindow->fillRect(frame);
  parentWindow->setColor(prev);
}