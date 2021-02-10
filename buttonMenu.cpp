#include"buttonMenu.h"
#include"collisionDetector.h"

void ButtonMenu::generate(const std::map<int,TextureName> &textures, const AppColors &colors)
{
  int currentY = yOffset;
  int currentX = frame.w/16;
  int padding = 5;
  int iconMaxSide = 32;
  int iconSep = 5;
  int TileMenuItemsMargin = 5;
  int buttonWidth = frame.w - currentX - currentX;
  int maxTextWidth;
  Font buttonFont("NotoSans-Regular.ttf", 14, colors.buttonText);
  buttons.clear();
  for(const auto &t : textures){ // Create buttons
    buttons.emplace_back(parentWindow,frame.x+currentX,currentY,padding,padding);
    Button &current = buttons.back();
    auto iconDimensions = current.setIcon(t.second.texture,iconMaxSide,iconSep);
    maxTextWidth = buttonWidth-padding-padding-iconDimensions.first-iconSep;
    current.setTextAreaWidth(maxTextWidth);
    current.setText(buttonFont,t.second.name);
    currentY += current.getHeight() + TileMenuItemsMargin;
    current.setBackgroundColor(colors.buttonBackground);
    current.setHoverColor(colors.buttonHover);
    current.setBorderColor(colors.buttonBorder);
    current.setElementId(t.first);
    current.setLeftClickCallback(leftCallback);
    current.setRightClickCallback(rightCallback);
    current.setRightPadding(buttonWidth - current.getWidth());
  }
  camera.setYBound(0,currentY+TileMenuItemsMargin);
}

void ButtonMenu::render() const
{
  SDL_Color prev = parentWindow->getColor();
  if(backgroundColorIsSet){
    parentWindow->setColor(backgroundColor);
    parentWindow->fillRect(frame);
  }
  if(borderColorIsSet){
    parentWindow->setColor(borderColor);
    parentWindow->drawRect(frame);
  }
  parentWindow->setColor(prev);
  SDL_Rect clip = frame;
  parentWindow->setClip(&clip);
  for(auto &b : buttons)
    b.render(camera);
  parentWindow->setClip();
}

void ButtonMenu::click(const SDL_Event &e) const
{
  if(Collision::between({e.button.x,e.button.y},frame)){
    for(auto &b : buttons)
      b.click(e,camera);
  }
}

void ButtonMenu::mouseMove(const SDL_Event &e)
{
  if(Collision::between({e.motion.x,e.motion.y},frame)){
    for(auto &b : buttons)
      b.mouseMove(e,camera);
  }
}

void ButtonMenu::scroll(const SDL_Event &e)
{
  if(e.wheel.y > 0){
    camera.moveUp();
  }
  else if(e.wheel.y < 0){
    camera.moveDown();
  }
}