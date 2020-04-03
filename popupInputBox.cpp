#include"popupInputBox.h"

PopupInputBox::PopupInputBox(std::shared_ptr<WindowWrapper> w, int width, int height, const std::string &fontPath, const AppColors &colors):
    GuiElement(w,(w->getWidth()-width)/2,(w->getHeight()-height)/2,width,height),
    inputField(w,frame.x+width/6,frame.y+height/3,width*4/6,height/4,fontPath,height/6,colors.textFieldText), closeButton(w,frame.x+width,frame.y,0,5),
    actionButton(w,frame.x+width/2,frame.y+height*2/3,5,5)
{
  closeButton.setText(inputField.getFont(),"X");
  closeButton.setX(frame.x+width-closeButton.getWidth());
  closeButton.setHoverColor(colors.buttonHover);
  closeButton.setBackgroundColor(colors.buttonBackground);
  closeButton.setBorderColor(colors.buttonBorder);
  actionButton.setBackgroundColor(colors.buttonBackground);
  actionButton.setHoverColor(colors.buttonHover);
  actionButton.setBorderColor(colors.buttonBorder);
  inputField.setBackgroundColor(colors.textFieldBackground);
}

void PopupInputBox::setActionButtonLabel(const std::string &label)
{
  actionButton.setText(inputField.getFont(),label);
  actionButton.setX(frame.x+(frame.w-actionButton.getWidth())/2);
}

void PopupInputBox::render() const
{
  if(!shown)
    return;
  SDL_Color prev = parentWindow->getColor();
  if(backgroundColorIsSet){
    parentWindow->setColor(backgroundColor);
    parentWindow->fillRect(frame);
  }
  if(borderColorIsSet){
    parentWindow->drawRect(frame);
    parentWindow->setColor(borderColor);
  }
  if(titleIsSet)
    title.render(parentWindow->getRenderer(),frame.x+5,frame.y);
  parentWindow->setColor(prev);
  closeButton.render();
  actionButton.render();
  inputField.render();
}

void PopupInputBox::click(const SDL_Event &event) const
{
  if(!closeButton.click(event))
    actionButton.click(event);
}

void PopupInputBox::mouseMove(const SDL_Event &event)
{
  closeButton.mouseMove(event);
  actionButton.mouseMove(event);
}

void PopupInputBox::textInput(const SDL_Event &event)
{
  inputField.append(event.text.text);
}

void PopupInputBox::backspace()
{
  inputField.popBack();
}