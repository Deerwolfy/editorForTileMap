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
  Texture(SDL_Renderer *renderer, const std::string &path) { loadImg(renderer ,path); }
  Texture(SDL_Renderer *renderer, const Font &font, const std::string &text)
    { loadText(renderer,font,text); }
  void loadImg(SDL_Renderer*, const std::string&);
  void loadText(SDL_Renderer*, const Font&, const std::string&);
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  void render(SDL_Renderer *renderer, int x, int y, const SDL_Rect *clip = nullptr, double angle = 0,
              const SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
  bool isEmpty() const { return texture == nullptr; }
  void resize(SDL_Renderer*,int,int);
  Texture copy(SDL_Renderer*) const;
private:
  Texture(SDL_Renderer*,std::shared_ptr<SDL_Surface>);
  Uint32 getSourcePixel(std::shared_ptr<SDL_Surface>,int,int) const;
  Uint8 lerp(Uint8,Uint8,double) const;
  void updateTextureFromSurface(SDL_Renderer*);
  std::shared_ptr<SDL_Texture> texture;
  std::shared_ptr<SDL_Surface> surface;
  int width;
  int height;
};

#endif
