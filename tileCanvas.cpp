#include"tileCanvas.h"
#include"collisionDetector.h"
#include<locale>
#include<codecvt>

void TileCanvas::placeTiles(const SelectionBox &selection, const Camera &cam, const Texture &texture, int id)
{
  SDL_Point origin = selection.getOrigin(cam);
  actualPlace({origin.x,origin.y,selection.getWidth(),selection.getHeight()},texture,id);
}

void TileCanvas::placeTiles(const SelectionBox &selection, const Texture &texture, int id)
{
  SDL_Point origin = selection.getOrigin();
  actualPlace({origin.x,origin.y,selection.getWidth(),selection.getHeight()},texture,id);
}

void TileCanvas::removeTiles(const SelectionBox &selection, const Camera &cam)
{
  SDL_Point origin = selection.getOrigin(cam);
  actualDelete({origin.x,origin.y,selection.getWidth(),selection.getHeight()});
  
}

void TileCanvas::removeTiles(const SelectionBox &selection)
{
  SDL_Point origin = selection.getOrigin();
  actualDelete({origin.x,origin.y,selection.getWidth(),selection.getHeight()});
}

void TileCanvas::actualPlace(const SDL_Rect &selection, const Texture &texture, int id)
{
  int xOffset = (selection.x-frame.x)%tileSize;
  int yOffset = (selection.y-frame.y)%tileSize;
  int startX = selection.x - frame.x - xOffset;
  int countX = selection.w/tileSize + 1;
  int countY = selection.h/tileSize + 1;
  int currentY = selection.y - frame.y - yOffset;
  while(countY--){
    int currentX = startX;
    int count = countX;
    while(count--){
      if(Collision::between({currentX,currentY},{0,0,frame.w,frame.h}) && tileDoesExistAt(currentX,currentY) == tiles.size())
        tiles.push_back(std::make_pair(id,Tile(texture,currentX,currentY)));
      currentX += tileSize;
    }
    currentY += tileSize;
  }
}

void TileCanvas::actualDelete(const SDL_Rect &selection)
{
  int xOffset = selection.x%tileSize;
  int yOffset = selection.y%tileSize;
  int startX = selection.x - frame.x - xOffset;
  int countX = selection.w/tileSize + 1;
  int countY = selection.h/tileSize + 1;
  int currentY = selection.y - frame.y - yOffset;
  while(countY--){
    int currentX = startX;
    int count = countX;
    while(count--){
      std::size_t index = tileDoesExistAt(currentX,currentY);
      if(index != tiles.size())
        tiles.erase(std::begin(tiles) + index);
        currentX += tileSize;
      }
    currentY += tileSize;
  }
}

std::size_t TileCanvas::tileDoesExistAt(int x, int y) const
{
  auto size = tiles.size();
  for(std::size_t i = 0; i < size; ++i){
    if(Collision::between({x,y},tiles[i].second.getBoundBox()))
      return i;
  }
  return size;
}

void TileCanvas::render(const Camera &cam) const
{
  if(!cam.isVisible(frame))
    return;
  SDL_Rect relativeFrame = cam.getRelativeRect(frame);
  SDL_Color prev = parentWindow->getColor();
  SDL_Rect textureFrame = {0,0,frame.w,frame.h};
  SDL_Rect dstFrame = relativeFrame;
  if(cam.getZoom() != 1){
    dstFrame.w *= cam.getZoom();
    dstFrame.h *= cam.getZoom();
  }
  target.setAsTarget(*parentWindow);
  if(backgroundColorIsSet){
    parentWindow->setColor(backgroundColor);
    parentWindow->fillRect(textureFrame);
  }
  if(borderColorIsSet){
    parentWindow->setColor(borderColor);
    parentWindow->drawRect(textureFrame);
  }
  if(backgroundTextureIsSet){
    backgroundTexture.render(*parentWindow,0,0);
  }
  for(const auto &tile : tiles)
    tile.second.render(*parentWindow);
  parentWindow->resetTarget();
  target.render(*parentWindow,&textureFrame,&dstFrame);
  parentWindow->setColor(prev);
}

void TileCanvas::render() const
{
  SDL_Color prev = parentWindow->getColor();
  SDL_Rect textureFrame = {0,0,frame.w,frame.h};
  target.setAsTarget(*parentWindow);
  if(backgroundColorIsSet){
    parentWindow->setColor(backgroundColor);
    parentWindow->fillRect(textureFrame);
  }
  if(borderColorIsSet){
    parentWindow->setColor(borderColor);
    parentWindow->drawRect(textureFrame);
  }
  if(backgroundTextureIsSet){
    backgroundTexture.render(*parentWindow,0,0);
  }
  for(const auto &tile : tiles)
    tile.second.render(*parentWindow);
  parentWindow->resetTarget();
  target.render(*parentWindow,frame.x,frame.y,&textureFrame);
  parentWindow->setColor(prev);
}

void TileCanvas::updateTileId(int oldId, int newId)
{
  for(auto &tile : tiles)
    if(tile.first == oldId)
      tile.first = newId;
}

void TileCanvas::setBackgroundTexture(const std::wstring &path)
{
  std::string pathNarrow = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(path);
  backgroundTexture.loadImg(*parentWindow, pathNarrow);
  backgroundTextureIsSet = true;
  clearBackgoundColor();
}

void TileCanvas::setBackgroundColor(const SDL_Color &color)
{
  backgroundColor = color;
  backgroundColorIsSet = true;
  clearBackgroundTexture();
}