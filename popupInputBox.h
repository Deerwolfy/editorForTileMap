#ifndef POPUPINPUTBOX_H
#define POPUPINPUTBOX_H

#include"guiElement.h"
#include"windowWrapper.h"
#include"appColors.h"
#include"button.h"
#include"textField.h"
#include"texture.h"
#include<memory>
#include<functional>
#include<string>

class PopupInputBox : public GuiElement {
public:
  PopupInputBox(std::shared_ptr<WindowWrapper> w, int width, int height, const std::string &fontPath, const AppColors &colors);
  void setActionCallback(std::function<void(const GuiElement&)> callback) { actionButton.setLeftClickCallback(callback); }
  void setCloseCallback(std::function<void(const GuiElement&)> callback) { closeButton.setLeftClickCallback(callback); }
  void setTitle(const std::string &title) { (this->title).loadText(*parentWindow,inputField.getFont(),title); titleIsSet = true; }
  void setActionButtonLabel(const std::string &label);
  void click(const SDL_Event&) const;
  void mouseMove(const SDL_Event&);
  void textInput(const SDL_Event&);
  std::string getInput() const { return inputField.str(); }
  void backspace();
  void render() const override;
  void confirm() const { actionButton.leftClick(); }
  void close() const { closeButton.leftClick(); }
  void markDone() { done = true; }
  bool isDone() const { return done; }
private:
  bool done = false;
  TextField inputField;
  Button closeButton;
  Button actionButton;
  Texture title;
  bool titleIsSet = false;
};

#endif