#ifndef WINDOWWRAPPER_H
#define WINDOWWRAPPER_H

#include<SDL.h>
#include<string>

class WindowWrapper {
public:
  WindowWrapper(int width, int height):
        window(nullptr), renderer(nullptr), width(width), height(height) { create(); }
  WindowWrapper(const WindowWrapper&) = delete;
  WindowWrapper &operator=(const WindowWrapper&) = delete;
  void show() const { SDL_ShowWindow(window); }
  void hide() const { SDL_HideWindow(window); }
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  SDL_Renderer *getRenderer() const { return renderer; }
  SDL_Window *getWindow() const { return window; }
  void clear() const;
  void redraw() const { SDL_RenderPresent(renderer); }
  void drawRect(const SDL_Rect &r) const { SDL_RenderDrawRect(renderer,&r); }
  void fillRect(const SDL_Rect &r) const { SDL_RenderFillRect(renderer,&r); }
  void drawPoint(const SDL_Point &p) const { SDL_RenderDrawPoint(renderer,p.x,p.y); }
  void drawPoint(int x, int y) const { SDL_RenderDrawPoint(renderer,x,y); }
  void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF) const { SDL_SetRenderDrawColor(renderer,r,g,b,a); }
  void setColor(const SDL_Color &color) const { SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a); }
  void setViewport(SDL_Rect *viewport = nullptr) const { SDL_RenderSetViewport(renderer,viewport); }
  void getViewport(SDL_Rect *viewport) const { SDL_RenderGetViewport(renderer,viewport); }
  void setClip(SDL_Rect *clip = nullptr) const { SDL_RenderSetClipRect(renderer,clip); }
  SDL_Color getColor() const;
  bool textureTargetSupport() const;
  void resetTarget();
  ~WindowWrapper() { destroy(); }
private:
  void destroy();
  void create();
  void throwError(std::string,const char*) const;
  SDL_Window *window;
  SDL_Renderer *renderer;
  int width;
  int height;
};

#endif
