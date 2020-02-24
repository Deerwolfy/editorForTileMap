#include"texture.h"
#include<SDL_image.h>
#include<iostream>

struct PixelData {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
};

void Texture::loadImg(WindowWrapper &w, const std::string &path)
{
  std::shared_ptr<SDL_Surface> tempSurf(IMG_Load(path.c_str()),SDL_FreeSurface);
  if(tempSurf == nullptr){
    std::cout << "Failed to load image " << path <<
                 " reason: " << IMG_GetError() << std::endl;
    return;
  }
  surface = tempSurf;
  updateTextureFromSurface(w);
}

void Texture::updateTextureFromSurface(WindowWrapper &w)
{
  std::shared_ptr<SDL_Texture> tempTexture(SDL_CreateTextureFromSurface(w.getRenderer(),surface.get()),
    SDL_DestroyTexture);
  if(tempTexture == nullptr){
    std::cout << "Failed to create texture, reason: " << SDL_GetError() << std::endl;
    return;
  }
  texture = tempTexture;
  SDL_SetTextureBlendMode(texture.get(),SDL_BLENDMODE_BLEND);
  width = surface.get()->w;
  height = surface.get()->h;
}

void Texture::loadText(WindowWrapper &w, const Font &font, const std::string &text)
{
  std::shared_ptr<SDL_Surface> tempSurface(TTF_RenderText_Blended(font.getFont().get(),text.c_str(),font.getColor()),SDL_FreeSurface);
  if(tempSurface == nullptr){
    std::cout << "Failed to render text" << std::endl;
    return;
  }
  surface = tempSurface;
  updateTextureFromSurface(w);
}

Texture::Texture(WindowWrapper &w, std::shared_ptr<SDL_Surface> surf)
{
  surface = surf;
  updateTextureFromSurface(w);
}

void Texture::render(WindowWrapper &w, int x, int y, const SDL_Rect *clip, double angle,
                           const SDL_Point *center, SDL_RendererFlip flip) const
{
  SDL_Rect renderRect = { x, y, width, height };
  if(clip != nullptr){
    renderRect.w = clip->w;
    renderRect.h = clip->h;
  }
  SDL_RenderCopyEx(w.getRenderer(), texture.get(), clip, &renderRect, angle, center, flip);
}

Texture Texture::copy(WindowWrapper &w) const
{
  return Texture(w,surface);
}

void Texture::resize(WindowWrapper &window, int w, int h)
{
  if(w <= 0 || h <= 0){
    std::cout << "Destination texture size must be bigger than zero. Current: "
    << w << "x" << h << std::endl;
    return;
  }
  if(surface == nullptr){
    std::cout << "No texture to resize" << std::endl;
    return;
  }
  std::shared_ptr<SDL_Surface> destSurf(SDL_CreateRGBSurfaceWithFormat(0,w,h,32,SDL_PIXELFORMAT_RGBA32),SDL_FreeSurface);
  std::shared_ptr<SDL_Surface> sourceSurf(SDL_ConvertSurfaceFormat(surface.get(),SDL_PIXELFORMAT_RGBA32,0),SDL_FreeSurface);
  SDL_LockSurface(destSurf.get());
  SDL_LockSurface(sourceSurf.get());
  Uint32 *sourcePixels = static_cast<Uint32*>(sourceSurf.get()->pixels);
  Uint32 *destPixels = static_cast<Uint32*>(destSurf.get()->pixels);
  const int sourcePixelsPerLine = sourceSurf.get()->pitch/4;
  const int destPixelsPerLine = destSurf.get()->pitch/4;
  const double ratioX = sourceSurf.get()->w/static_cast<double>(w);
  const double ratioY = sourceSurf.get()->h/static_cast<double>(h);
  double sourceX = 0;
  double sourceY = 0;
  int ix = 0;
  int iy = 0;
  Uint32 pointA = 0;
  Uint32 pointB = 0;
  Uint32 pointC = 0;
  Uint32 pointD = 0;
  PixelData colorsA;
  PixelData colorsB;
  PixelData colorsC;
  PixelData colorsD;
  PixelData outColors;
  for(int destY = 0; destY < h; ++destY){
    sourceY = destY * ratioY;
    iy = static_cast<int>(sourceY);
    for(int destX = 0; destX < w; ++destX){
      sourceX = destX * ratioX;
      ix = static_cast<int>(sourceX);
      pointA = sourcePixels[ix+iy*sourcePixelsPerLine];
      pointB = sourcePixels[(ix+1)+iy*sourcePixelsPerLine];
      pointC = sourcePixels[(ix+1)+(iy+1)*sourcePixelsPerLine];
      pointD = sourcePixels[ix+(iy+1)*sourcePixelsPerLine];
      SDL_GetRGBA(pointA,sourceSurf.get()->format,&colorsA.r,&colorsA.g,&colorsA.b,&colorsA.a);
      SDL_GetRGBA(pointB,sourceSurf.get()->format,&colorsB.r,&colorsB.g,&colorsB.b,&colorsB.a);
      SDL_GetRGBA(pointC,sourceSurf.get()->format,&colorsC.r,&colorsC.g,&colorsC.b,&colorsC.a);
      SDL_GetRGBA(pointD,sourceSurf.get()->format,&colorsD.r,&colorsD.g,&colorsD.b,&colorsD.a);
      outColors.r = lerp(lerp(colorsA.r,colorsB.r,ix,ix+1,sourceX),lerp(colorsC.r,colorsD.r,ix,ix+1,sourceX),iy,iy+1,sourceY);
      outColors.g = lerp(lerp(colorsA.g,colorsB.g,ix,ix+1,sourceX),lerp(colorsC.g,colorsD.g,ix,ix+1,sourceX),iy,iy+1,sourceY);
      outColors.b = lerp(lerp(colorsA.b,colorsB.b,ix,ix+1,sourceX),lerp(colorsC.b,colorsD.b,ix,ix+1,sourceX),iy,iy+1,sourceY);
      outColors.a = lerp(lerp(colorsA.a,colorsB.a,ix,ix+1,sourceX),lerp(colorsC.a,colorsD.a,ix,ix+1,sourceX),iy,iy+1,sourceY);
      destPixels[destX+destY*destPixelsPerLine] = SDL_MapRGBA(destSurf.get()->format,outColors.r,outColors.g,outColors.b,outColors.a);
    }
  }
  SDL_UnlockSurface(destSurf.get());
  SDL_UnlockSurface(sourceSurf.get());
  sourcePixels = nullptr;
  destPixels = nullptr;
  surface = destSurf;
  updateTextureFromSurface(window);
}

Uint8 Texture::lerp(Uint8 fLeft, Uint8 fRight, double left, double right, double target) const
{
  return (right-target)/(right-left)*fLeft+(target-left)/(right-left)*fRight;
}