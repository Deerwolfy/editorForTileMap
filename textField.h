#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include"guiElement.h"
#include<string>
#include"font.h"
#include"windowWrapper.h"
#include"texture.h"
#include"timer.h"
#include<SDL.h>
#include<stdexcept>
#include<memory>

class TextField : public GuiElement {
public:
  TextField(std::shared_ptr<WindowWrapper> window, int x, int y, int width, int height, std::string fontPath, int pSize, SDL_Color fontColor = {0x00, 0x00, 0x00, 0xFF}):
    GuiElement(window, x, y, width, height), textFont(fontPath,pSize,fontColor) { animationTimer.start(); regenerateText(); }
  void render() const override;
  void append(const std::string &str) { text.append(str); empty = false; regenerateText(); }
  void append(const char *str) { text.append(str); empty = false; regenerateText(); }
  void popBack() { if(text.length()) text.pop_back(); regenerateText(); }
  std::string str() const { return text; }
private:
  void regenerateText();
  std::string text;
  Font textFont;
  bool empty = true;
  Texture textTexture;
  Texture cursor;
  mutable Timer animationTimer;
};

#endif