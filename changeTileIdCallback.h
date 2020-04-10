#ifndef CHANGETILEIDCALLBACK_H
#define CHANGETILEIDCALLBACK_H

#include<map>
#include<vector>
#include"guiElement.h"
#include"windowWrapper.h"
#include"texture.h"
#include"textureName.h"
#include"popupInputBox.h"
#include"tile.h"

class ChangeTileIdCallback {
public:
  ChangeTileIdCallback(std::shared_ptr<WindowWrapper> window,std::map<int,TextureName> &idToTextureName, std::vector<Tile> &tiles):
    window(window), idToTextureName(idToTextureName), tiles(tiles) { }
  void operator()(const PopupInputBox&);
private:
  std::shared_ptr<WindowWrapper> window;
  std::vector<Tile> &tiles;
  std::map<int,TextureName> &idToTextureName;
};

#endif