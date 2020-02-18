#include"windowWrapper.h"
#include<stdexcept>
#include<string>

void WindowWrapper::create()
{
  window = SDL_CreateWindow("TileMapEditor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
  if(window == NULL){
    throwError("Unable to create window SDL_Error: ", SDL_GetError());
  }
  renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(renderer == NULL){
    throwError("Unable to create renderer SDL_Error: ", SDL_GetError());
  }
  SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
}

void WindowWrapper::destroy()
{
  SDL_DestroyRenderer(renderer);
  renderer = nullptr;
  SDL_DestroyWindow(window);
  window = nullptr;
}

void WindowWrapper::throwError(std::string msg, const char *sdlMsg) const
{
  throw std::runtime_error(msg.append(sdlMsg)); 
}

void WindowWrapper::clear() const
{
  SDL_RenderClear(renderer);
}
