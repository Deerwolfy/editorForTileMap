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
  void setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const { SDL_SetRenderDrawColor(renderer,r,g,b,a); }
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
