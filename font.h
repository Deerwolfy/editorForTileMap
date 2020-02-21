#ifndef FONT_H
#define FONT_H

#include<SDL.h>
#include<SDL_ttf.h>
#include<memory>
#include<string>
#include<utility>

class Font {
public:
  Font(const std::string &fontPath, int pSize, SDL_Color color = {0x00,0x00,0x00,0xFF});
  void loadFont(const std::string&);
  SDL_Color getColor() const { return color; }
  std::shared_ptr<TTF_Font> getFont() const { return font; }
  std::pair<int,int> getTextWH(const std::string&) const;
private:
  SDL_Color color;
  int size;
  std::shared_ptr<TTF_Font> font;
};

#endif
