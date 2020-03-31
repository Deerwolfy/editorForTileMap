#ifndef CHANGETILEIDCALLBACK_H
#define CHANGETILEIDCALLBACK_H

#include<map>
#include"guiElement.h"
#include"windowWrapper.h"
#include"texture.h"
#include"textureName.h"

class ChangeTileIdCallback {
public:
  ChangeTileIdCallback(std::shared_ptr<WindowWrapper> window,std::map<int,TextureName> &idToTextureName):
    window(window), idToTextureName(idToTextureName) { }
  void operator()(const GuiElement&);
private:
  std::shared_ptr<WindowWrapper> window;
  std::map<int,TextureName> &idToTextureName;
};

#endif