#ifndef BUTTONMENU_H
#define BUTTONMENU_H

#include<SDL.h>
#include<vector>
#include"camera.h"
#include"button.h"
#include"appColors.h"
#include<map>
#include"textureName.h"
#include"windowWrapper.h"
#include"guiElement.h"
#include<memory>
#include<functional>

class ButtonMenu : public GuiElement {
public:
  ButtonMenu(std::shared_ptr<WindowWrapper> w, int x, int y, int width, int height):
    GuiElement(w,x,y,width,height), camera(x,y,width,height), yOffset(y) { }
  void setCameraScrollSpeed(int speed) { camera.setYMovementSpeed(speed); }
  void setButtonYOffset(int y) { yOffset = frame.y+y; }
  void setButtonsLeftCallback(std::function<void(const GuiElement&)> callback) { leftCallback = callback; }
  void setButtonsRightCallback(std::function<void(const GuiElement&)> callback) { rightCallback = callback; }
  void generate(const std::map<int,TextureName> &textures, const AppColors &colors);
  void render() const override;
  void click(const SDL_Event& e) const;
  void mouseMove(const SDL_Event& e);
  void scroll(const SDL_Event& e);
private:
  std::vector<Button> buttons;
  int yOffset;
  Camera camera;
  std::function<void(const GuiElement&)> leftCallback;
  std::function<void(const GuiElement&)> rightCallback;
};

#endif