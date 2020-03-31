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
  Texture(std::shared_ptr<WindowWrapper> window, const std::string &path) { loadImg(window ,path); }
  Texture(std::shared_ptr<WindowWrapper> window, const Font &font, const std::string &text)
    { loadText(window,font,text); }
  void loadImg(std::shared_ptr<WindowWrapper>, const std::string&);
  void loadText(std::shared_ptr<WindowWrapper>, const Font&, const std::string&);
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  void render(std::shared_ptr<WindowWrapper> window, int x, int y, const SDL_Rect *clip = nullptr, double angle = 0,
              const SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
  bool isEmpty() const { return texture == nullptr; }
  void resize(std::shared_ptr<WindowWrapper>,int,int);
  Texture copy(std::shared_ptr<WindowWrapper>) const;
private:
  Texture(std::shared_ptr<WindowWrapper>,std::shared_ptr<SDL_Surface>);
  Uint32 getSourcePixel(std::shared_ptr<SDL_Surface>,int,int) const;
  Uint8 lerp(Uint8,Uint8,double) const;
  void updateTextureFromSurface(std::shared_ptr<WindowWrapper>);
  std::shared_ptr<SDL_Texture> texture;
  std::shared_ptr<SDL_Surface> surface;
  int width;
  int height;
};

#endif
