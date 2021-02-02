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
  Texture(): width(0), height(0), xScaleDiff(0), yScaleDiff(0) { }
  Texture(WindowWrapper &win, const std::string &path) { loadImg(win ,path); }
  Texture(WindowWrapper &win, const Font &font, const std::string &text)
    { loadText(win,font,text); }
  void loadImg(WindowWrapper&, const std::string&);
  void loadText(WindowWrapper&, const Font&, const std::string&);
  int getWidth() const { return width+xScaleDiff; }
  int getHeight() const { return height+yScaleDiff; }
  int getYScaleDiff() const { return yScaleDiff; }
  int getXScaleDiff() const { return xScaleDiff; }
  void render(WindowWrapper &win, int x, int y, const SDL_Rect *clip = nullptr, double angle = 0,
              const SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
  bool isEmpty() const { return texture == nullptr; }
  void resize(WindowWrapper&,int,int);
  void resizeApply(WindowWrapper&,int,int);
  void reload(WindowWrapper &win);
  Texture copy(WindowWrapper&) const;
private:
  std::shared_ptr<SDL_Surface> resizeSurface(int,int);
  Texture(WindowWrapper&,std::shared_ptr<SDL_Surface>);
  Uint32 getSourcePixel(std::shared_ptr<SDL_Surface>,int,int) const;
  Uint8 lerp(Uint8,Uint8,double) const;
  void updateTextureFromSurface(WindowWrapper&);
  void updateTextureFromSurface(WindowWrapper&, std::shared_ptr<SDL_Surface> surface);
  std::shared_ptr<SDL_Texture> texture;
  std::shared_ptr<SDL_Surface> surface;
  int width;
  int height;
  int xScaleDiff;
  int yScaleDiff;
};

#endif
