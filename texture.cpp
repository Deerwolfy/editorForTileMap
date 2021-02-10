#include"texture.h"
#include"errorHandler.h"
#include<SDL_image.h>

struct PixelData {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
};

Texture::Texture(WindowWrapper &win, int width, int height, accessLevel access):
  width(width), height(height), yScaleDiff(0), xScaleDiff(0),
  texture(SDL_CreateTexture(win.getRenderer(),SDL_GetWindowPixelFormat(win.getWindow()),access,width,height),SDL_DestroyTexture),
  surface(SDL_CreateRGBSurfaceWithFormat(0,width,height,32,SDL_PIXELFORMAT_RGBA8888),SDL_FreeSurface)
{
  if(surface == nullptr){
    ErrorHandler::createMessage(std::string("Failed to create surface, reason: ") + SDL_GetError(),ErrorHandler::MessageLevel::WARNING);
  }
  if(texture == nullptr){
    ErrorHandler::createMessage(std::string("Failed to create texture, reason: ") + SDL_GetError(),ErrorHandler::MessageLevel::WARNING);
  }
  else {
    SDL_SetTextureBlendMode(texture.get(),SDL_BLENDMODE_BLEND);
  }
}

void Texture::loadImg(const WindowWrapper &win, const std::string &path)
{
  std::shared_ptr<SDL_Surface> tempSurf(IMG_Load(path.c_str()),SDL_FreeSurface);
  if(tempSurf == nullptr){
    ErrorHandler::createMessage("Failed to load image " + path + " reason: " + IMG_GetError(),ErrorHandler::MessageLevel::WARNING);
    return;
  }
  surface = tempSurf;
  updateTextureFromSurface(win);
}

void Texture::updateTextureFromSurface(const WindowWrapper &win)
{
  std::shared_ptr<SDL_Texture> tempTexture(SDL_CreateTextureFromSurface(win.getRenderer(),surface.get()),
    SDL_DestroyTexture);
  if(tempTexture == nullptr){
    ErrorHandler::createMessage(std::string("Failed to create texture, reason: ") + SDL_GetError(),ErrorHandler::MessageLevel::WARNING);
    return;
  }
  texture = tempTexture;
  SDL_SetTextureBlendMode(texture.get(),SDL_BLENDMODE_BLEND);
  width = surface.get()->w;
  height = surface.get()->h;
  xScaleDiff = 0;
  yScaleDiff = 0;
}

void Texture::updateTextureFromSurface(const WindowWrapper &win, std::shared_ptr<SDL_Surface> sourceSurface)
{
  std::shared_ptr<SDL_Texture> tempTexture(SDL_CreateTextureFromSurface(win.getRenderer(),sourceSurface.get()),
    SDL_DestroyTexture);
  if(tempTexture == nullptr){
    ErrorHandler::createMessage(std::string("Failed to create texture, reason: ") + SDL_GetError(),ErrorHandler::MessageLevel::WARNING);
    return;
  }
  texture = tempTexture;
  SDL_SetTextureBlendMode(texture.get(),SDL_BLENDMODE_BLEND);
  width = sourceSurface.get()->w;
  height = sourceSurface.get()->h;
}

void Texture::loadText(const WindowWrapper &win, const Font &font, const std::string &text)
{
  std::shared_ptr<SDL_Surface> tempSurface(TTF_RenderText_Blended(font.getFont().get(),text.c_str(),font.getColor()),SDL_FreeSurface);
  if(tempSurface == nullptr){
    ErrorHandler::createMessage(std::string("Failed to render text") + TTF_GetError(),ErrorHandler::MessageLevel::WARNING);
    return;
  }
  surface = tempSurface;
  updateTextureFromSurface(win);
}

Texture::Texture(const WindowWrapper &win, std::shared_ptr<SDL_Surface> surf)
{
  surface = surf;
  updateTextureFromSurface(win);
}

void Texture::render(const WindowWrapper &win, int x, int y, const SDL_Rect *clip, double angle,
                           const SDL_Point *center, SDL_RendererFlip flip) const
{
  SDL_Rect renderRect = { x, y, width, height };
  if(clip != nullptr){
    renderRect.w = clip->w;
    renderRect.h = clip->h;
  }
  SDL_RenderCopyEx(win.getRenderer(), texture.get(), clip, &renderRect, angle, center, flip);
}

Texture Texture::copy(const WindowWrapper &win) const
{
  return Texture(win,surface);
}

bool Texture::resize(const WindowWrapper &win,int w, int h)
{
  if(width == w && height == h)
    return false;
  std::shared_ptr<SDL_Surface> resizedSurf = resizeSurface(w,h);
  if(resizedSurf == nullptr){
    return false;
  }
  updateTextureFromSurface(win,resizedSurf);
  xScaleDiff = w-width;
  yScaleDiff = h-height;
  return true;
}

bool Texture::resizeApply(const WindowWrapper &win,int w, int h)
{
  if(width == w && height == h)
    return false;
  std::shared_ptr<SDL_Surface> resizedSurf = resizeSurface(w,h);
  if(resizedSurf == nullptr){
    return false;
  }
  surface = resizedSurf;
  updateTextureFromSurface(win);
  return true;
}

std::shared_ptr<SDL_Surface> Texture::resizeSurface(int w, int h)
{
  if(w <= 0 || h <= 0){
    ErrorHandler::createMessage("Destination texture size must be bigger than zero. Current: "
      + std::to_string(w) + "x" + std::to_string(h),ErrorHandler::MessageLevel::WARNING);
    return nullptr;
  }
  if(surface == nullptr){
    ErrorHandler::createMessage("No surface to resize", ErrorHandler::MessageLevel::WARNING);
    return nullptr;
  }
  std::shared_ptr<SDL_Surface> destSurf(SDL_CreateRGBSurfaceWithFormat(0,w,h,surface->format->BytesPerPixel,SDL_PIXELFORMAT_RGBA8888),SDL_FreeSurface);
  std::shared_ptr<SDL_Surface> sourceSurf(SDL_ConvertSurfaceFormat(surface.get(),SDL_PIXELFORMAT_RGBA8888,0),SDL_FreeSurface);
  SDL_LockSurface(destSurf.get());
  SDL_LockSurface(sourceSurf.get());
  Uint32 *sourcePixels = static_cast<Uint32*>(sourceSurf->pixels);
  Uint32 *destPixels = static_cast<Uint32*>(destSurf->pixels);
  const int destPixelsPerLine = destSurf->pitch/destSurf->format->BytesPerPixel;
  const double ratioX = sourceSurf->w/static_cast<double>(w);
  const double ratioY = sourceSurf->h/static_cast<double>(h);
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
      pointA = getSourcePixel(sourceSurf,ix,iy);
      pointB = getSourcePixel(sourceSurf,ix+1,iy);
      pointC = getSourcePixel(sourceSurf,ix,iy+1);
      pointD = getSourcePixel(sourceSurf,ix+1,iy+1);
      SDL_GetRGBA(pointA,sourceSurf->format,&colorsA.r,&colorsA.g,&colorsA.b,&colorsA.a);
      SDL_GetRGBA(pointB,sourceSurf->format,&colorsB.r,&colorsB.g,&colorsB.b,&colorsB.a);
      SDL_GetRGBA(pointC,sourceSurf->format,&colorsC.r,&colorsC.g,&colorsC.b,&colorsC.a);
      SDL_GetRGBA(pointD,sourceSurf->format,&colorsD.r,&colorsD.g,&colorsD.b,&colorsD.a);
      outColors.r = lerp(lerp(colorsA.r,colorsB.r,sourceX-ix),lerp(colorsC.r,colorsD.r,sourceX-ix),sourceY-iy);
      outColors.g = lerp(lerp(colorsA.g,colorsB.g,sourceX-ix),lerp(colorsC.g,colorsD.g,sourceX-ix),sourceY-iy);
      outColors.b = lerp(lerp(colorsA.b,colorsB.b,sourceX-ix),lerp(colorsC.b,colorsD.b,sourceX-ix),sourceY-iy);
      outColors.a = lerp(lerp(colorsA.a,colorsB.a,sourceX-ix),lerp(colorsC.a,colorsD.a,sourceX-ix),sourceY-iy);
      destPixels[destX+destY*destPixelsPerLine] = SDL_MapRGBA(destSurf->format,outColors.r,outColors.g,outColors.b,outColors.a);
    }
  }
  SDL_UnlockSurface(destSurf.get());
  SDL_UnlockSurface(sourceSurf.get());
  sourcePixels = nullptr;
  destPixels = nullptr;
  return destSurf;
}

Uint8 Texture::lerp(Uint8 left, Uint8 right, double target) const
{
  return (1-target)*left + target*right;
}

Uint32 Texture::getSourcePixel(std::shared_ptr<SDL_Surface> surf, int x, int y) const
{
  if(x >= surf->w)
    x -= x-surf->w+1;
  if(y >= surf->h)
    y -= y-surf->h+1;
  return static_cast<Uint32*>(surf->pixels)[x+y*surf->pitch/surf->format->BytesPerPixel];
}

void Texture::reload(const WindowWrapper &win)
{
  updateTextureFromSurface(win);
  xScaleDiff = 0;
  yScaleDiff = 0;
}

void Texture::setAsTarget(const WindowWrapper &win) const
{
  int access = 0;
  SDL_QueryTexture(texture.get(),NULL,&access,NULL,NULL);
  if(!win.textureTargetSupport()){
    ErrorHandler::createMessage(std::string("Renderer does not support texture target, ") + SDL_GetError(),ErrorHandler::MessageLevel::WARNING);
  }
  else if(access != accessLevel::TARGET){
    ErrorHandler::createMessage(std::string("The texture cannot be used as target, ") + SDL_GetError(),ErrorHandler::MessageLevel::WARNING);
  }
  else {
    SDL_SetRenderTarget(win.getRenderer(),texture.get());
  }
}