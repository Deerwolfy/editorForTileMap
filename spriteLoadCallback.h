#ifndef SPRITELOADCALLBACK_H
#define SPRITELOADCALLBACK_H

#include<map>
#include"guiElement.h"
#include<string>
#include"textureName.h"
#include"windowWrapper.h"
#include"texture.h"

class SpriteLoadCallback {
public:
  SpriteLoadCallback(std::map<int,TextureName> &idToTextureName, WindowWrapper &w, bool &reg):
    idToTextureName(idToTextureName), w(w), regenerate(reg) { }
  void operator()(const GuiElement&);
private:
  std::map<int,TextureName> &idToTextureName;
  WindowWrapper &w;
  bool &regenerate;
};

#endif