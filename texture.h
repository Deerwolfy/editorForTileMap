#ifndef TEXTURE_H
#define TEXTURE_H

#include<SDL.h>
#include<SDL_ttf.h>
#include<string>
#include<memory>
#include"windowWrapper.h"
#include"font.h"

class Texture {
public:
  Texture(): width(0), height(0) { }
  Texture(WindowWrapper &w, const std::string &path) { loadImg(w,path); }
  Texture(WindowWrapper &w, const Font &font, const std::string &text)
    { loadText(w,font,text); }
  void loadImg(WindowWrapper&, const std::string&);
  void loadText(WindowWrapper&, const Font&, const std::string&);
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  void render(WindowWrapper &w, int x, int y, const SDL_Rect *clip = nullptr, double angle = 0,
              const SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
  bool isEmpty() const { return texture == nullptr; }
  void resize(WindowWrapper&,int,int);
  Texture copy(WindowWrapper&) const;
private:
  Texture(WindowWrapper&,std::shared_ptr<SDL_Surface>);
  Uint8 lerp(Uint8,Uint8,double,double,double) const;
  void updateTextureFromSurface(WindowWrapper&);
  std::shared_ptr<SDL_Texture> texture;
  std::shared_ptr<SDL_Surface> surface;
  int width;
  int height;
};
#endif
