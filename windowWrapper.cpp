#include"windowWrapper.h"
#include"errorHandler.h"
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
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
  SDL_SetRenderDrawColor(renderer,0xFF,0xFF,0xFF,0xFF);
  SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
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

SDL_Color WindowWrapper::getColor() const
{
  SDL_Color color;
  SDL_GetRenderDrawColor(renderer,&color.r,&color.g,&color.b,&color.a);
  return color;
}

bool WindowWrapper::textureTargetSupport() const
{
  SDL_RendererInfo info;
  SDL_GetRendererInfo(renderer,&info);
  if(info.flags & SDL_RENDERER_TARGETTEXTURE){
    return true;
  }
  return false;
}

void WindowWrapper::resetTarget()
{
  if(textureTargetSupport()){
    SDL_SetRenderTarget(renderer,NULL);
  }
  else {
    ErrorHandler::createMessage(std::string("Renderer does not support texture target, ") + SDL_GetError(),ErrorHandler::MessageLevel::WARNING);
  }
}