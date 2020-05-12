#ifndef TILECANVAS_H
#define TILECANVAS_H

#include<memory>
#include<utility>
#include<vector>
#include<SDL.h>
#include"guiElement.h"
#include"windowWrapper.h"
#include"tile.h"
#include"selectionBox.h"
#include"camera.h"
#include"texture.h"

class TileCanvas : public GuiElement {
public:
  TileCanvas(std::shared_ptr<WindowWrapper> win, int x, int y, int width, int height, int tileSize):
    GuiElement(win,x,y,width,height), tileSize(tileSize) { }
  void placeTiles(const SelectionBox &selection, const Camera &cam, const Texture &texture, int id);
  void removeTiles(const SelectionBox &selection, const Camera &cam);
  void placeTiles(const SelectionBox &selection, const Texture &texture, int id);
  void removeTiles(const SelectionBox &selection);
  void render() const override;
  void render(const Camera &cam) const;
  void updateTileId(int oldId, int newId);
  void setBackgroundTexture(const std::wstring &path);
  void setBackgroundColor(const SDL_Color&) override;
  void clearBackgroundTexture() { backgroundTextureIsSet = false; }
private:
  std::size_t tileDoesExistAt(int x, int y) const;
  std::vector<std::pair<int,Tile>> tiles;
  int tileSize;
  bool backgroundTextureIsSet = false;
  Texture backgroundTexture;
};

#endif