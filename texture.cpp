#include"texture.h"
#include<SDL_image.h>
#include<iostream>

void Texture::loadImg(WindowWrapper &w, const std::string &path)
{
  SDL_Surface *surface = nullptr;
  surface = IMG_Load(path.c_str());
  if(surface == nullptr){
    std::cout << "Failed to load image " << path <<
                 " reason: " << IMG_GetError() << std::endl;
    return;
  }
  std::shared_ptr<SDL_Texture> tempPtr(SDL_CreateTextureFromSurface(w.getRenderer(),surface),
    SDL_DestroyTexture);
  if(tempPtr == nullptr){
    std::cout << "Failed to create texture, reason: " << SDL_GetError() << std::endl;
    return;
  }
  texture = tempPtr;
  width = surface->w;
  height = surface->h;
  SDL_FreeSurface(surface);
}

void Texture::loadText(WindowWrapper &w, const Font &font, const std::string &text)
{
  SDL_Surface *surface = nullptr;
  surface = TTF_RenderText_Blended(font.getFont().get(),text.c_str(),font.getColor());
  if(surface == nullptr){
    std::cout << "Failed to render text" << std::endl;
    return;
  }
  std::shared_ptr<SDL_Texture> tempPtr(SDL_CreateTextureFromSurface(w.getRenderer(),surface),
    SDL_DestroyTexture);
  if(tempPtr == nullptr){
    std::cout << "Failed to create texture, reason: " << SDL_GetError() << std::endl;
    return;
  }
  texture = tempPtr;
  width = surface->w;
  height = surface->h;
  SDL_FreeSurface(surface);
}

void Texture::render(WindowWrapper &w, int x, int y, double scaleCoef, const SDL_Rect *clip, double angle,
                           const SDL_Point *center, SDL_RendererFlip flip) const
{
  SDL_Rect renderRect = { x, y, width, height };
  if(scaleCoef != 1){
    renderRect.w *= scaleCoef;
    renderRect.h *= scaleCoef;
  }
  if(clip != nullptr){
    renderRect.w = clip->w;
    renderRect.h = clip->h;
  }
  SDL_RenderCopyEx(w.getRenderer(), texture.get(), clip, &renderRect, angle, center, flip);
}
