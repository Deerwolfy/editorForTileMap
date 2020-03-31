#ifndef SPRITELOADCALLBACK_H
#define SPRITELOADCALLBACK_H

#include<map>
#include<memory>
#include"guiElement.h"
#include<string>
#include"textureName.h"
#include"windowWrapper.h"
#include"texture.h"

class SpriteLoadCallback {
public:
  SpriteLoadCallback(std::map<int,TextureName> &idToTextureName, std::shared_ptr<WindowWrapper> window, bool &reg):
    idToTextureName(idToTextureName), window(window), regenerate(reg) { }
  void operator()(const GuiElement&);
private:
  std::map<int,TextureName> &idToTextureName;
  std::shared_ptr<WindowWrapper> window;
  bool &regenerate;
};

#endif