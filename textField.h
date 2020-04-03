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
    TextField(window, x, y, width, height, {fontPath,pSize,fontColor}) { }
  TextField(std::shared_ptr<WindowWrapper> window, int x, int y, int width, int height, const Font &font):
    GuiElement(window, x, y, width, height), textFont(font) { animationTimer.start(); regenerateText(); }
  void render() const override;
  void append(const std::string &str) { text.append(str); empty = false; regenerateText(); }
  void append(const char *str) { text.append(str); empty = false; regenerateText(); }
  void popBack() { if(text.length()) text.pop_back(); regenerateText(); }
  void setFont(std::string fontPath, int pSize, SDL_Color fontColor = {0x00, 0x00, 0x00, 0xFF}) { textFont = Font(fontPath,pSize,fontColor); regenerateText(); }
  const Font &getFont() const { return textFont; }
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