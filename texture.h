#ifndef TEXTURE_H
#define TEXTURE_H

#include<SDL.h>
#include<SDL_ttf.h>
#include<string>
#include<utility>
#include<memory>
#include"windowWrapper.h"
#include"font.h"

class Texture {
public:
  enum accessLevel {
    STATIC = SDL_TEXTUREACCESS_STATIC,
    STREAMING = SDL_TEXTUREACCESS_STREAMING,
    TARGET = SDL_TEXTUREACCESS_TARGET
  };
  Texture(): width(0), height(0), xScaleDiff(0), yScaleDiff(0) { }
  Texture(WindowWrapper &win, int width, int height, accessLevel access = accessLevel::STATIC);
  Texture(const WindowWrapper &win, const std::string &path) { loadImg(win ,path); }
  Texture(const WindowWrapper &win, const Font &font, const std::string &text)
    { loadText(win,font,text); }
  void loadImg(const WindowWrapper&, const std::string&);
  void loadText(const WindowWrapper&, const Font&, const std::string&);
  int getWidth() const { return width+xScaleDiff; }
  int getHeight() const { return height+yScaleDiff; }
  int getYScaleDiff() const { return yScaleDiff; }
  int getXScaleDiff() const { return xScaleDiff; }
  void render(const WindowWrapper &win, int x, int y, const SDL_Rect *clip = nullptr, double angle = 0,
              const SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
  bool isEmpty() const { return texture == nullptr; }
  bool resize(const WindowWrapper&,int,int);
  bool resizeApply(const WindowWrapper&,int,int);
  void reload(const WindowWrapper &win);
  Texture copy(const WindowWrapper&) const;
  void setAsTarget(const WindowWrapper&) const;
private:
  std::shared_ptr<SDL_Surface> resizeSurface(int,int);
  Texture(const WindowWrapper&,std::shared_ptr<SDL_Surface>);
  Uint32 getSourcePixel(std::shared_ptr<SDL_Surface>,int,int) const;
  Uint8 lerp(Uint8,Uint8,double) const;
  void updateTextureFromSurface(const WindowWrapper&);
  void updateTextureFromSurface(const WindowWrapper&, std::shared_ptr<SDL_Surface> surface);
  int width;
  int height;
  int xScaleDiff;
  int yScaleDiff;
  std::shared_ptr<SDL_Texture> texture;
  std::shared_ptr<SDL_Surface> surface;
};

#endif
