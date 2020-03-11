#include"selectionBox.h"

void SelectionBox::setEnd(int x, int y)
{
  if(startPoint.x < x){
    selection.x = startPoint.x;
    selection.w = x - startPoint.x;
  } else {
    selection.x = x;
    selection.w = startPoint.x - x;
  }
  if(startPoint.y < y){
    selection.y = startPoint.y;
    selection.h = y - startPoint.y;
  } else {
    selection.y = y;
    selection.h = startPoint.y - y;
  }
}

void SelectionBox::setStart(int x, int y)
{
  startPoint.x = x;
  startPoint.y = y;
  selection.x = x;
  selection.y = y;
  selection.w = 0;
  selection.h = 0;
  hold = true;
}

void SelectionBox::render(WindowWrapper &w) const
{
  SDL_Color prev = w.getColor();
  w.setColor(borderColor);
  w.drawRect(selection);
  w.setColor(backgroundColor);
  w.fillRect(selection);
  w.setColor(prev);
}