#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include"guiElement.h"
#include<string>
#include"font.h"
#include"windowWrapper.h"
#include"texture.h"
#include<SDL.h>
#include<stdexcept>

class TextField : public GuiElement {
public:
  TextField(int x, int y, int width, int height, std::string fontPath, int pSize, SDL_Color fontColor = {0x00, 0x00, 0x00, 0xFF}):
    GuiElement(x, y, width, height), textFont(fontPath,pSize,fontColor) { }
  void render(WindowWrapper&) const override;
  void append(const std::string &str) { text.append(str); empty = false; }
  void append(const char *str) { text.append(str); empty = false; }
  void popBack() { if(text.length()) text.pop_back(); }
  void regenerateText(WindowWrapper&);
  std::string str() const { return text; }
private:
  std::string text;
  Font textFont;
  bool empty = true;
  Texture textTexture;
  bool inFocus = true;
};

#endif