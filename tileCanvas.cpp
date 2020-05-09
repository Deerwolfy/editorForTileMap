#include"tileCanvas.h"
#include"texture.h"
#include"collisionDetector.h"

void TileCanvas::placeTiles(const SelectionBox &selection, const Camera &cam, const Texture &texture, int id)
{
  SDL_Point origin = selection.getOrigin(cam);
  int xOffset = origin.x%tileSize;
  int yOffset = origin.y%tileSize;
  int startX = origin.x - xOffset;
  int countX = (selection.getWidth()+xOffset)/tileSize + 1;
  int w = selection.getWidth();
  int currentY = origin.y - yOffset;
  int countY = (selection.getHeight()+yOffset)/tileSize + 1;
  while(countY--){
    int currentX = startX;
    int count = countX;
    while(count--){
      if(Collision::between({currentX,currentY},frame) && tileDoesExistAt(currentX,currentY) == tiles.size())
        tiles.push_back(std::make_pair(id,Tile(texture,currentX,currentY)));
      currentX += tileSize;
    }
    currentY += tileSize;
  }
}

void TileCanvas::removeTiles(const SelectionBox &selection, const Camera &cam)
{
  SDL_Point origin = selection.getOrigin(cam);
  int xOffset = origin.x%tileSize;
  int yOffset = origin.y%tileSize;
  int startX = origin.x - xOffset;
  int countX = (selection.getWidth()+xOffset)/tileSize + 1;
  int w = selection.getWidth();
  int currentY = origin.y - yOffset;
  int countY = (selection.getHeight()+yOffset)/tileSize + 1;
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

void TileCanvas::placeTiles(const SelectionBox &selection, const Texture &texture, int id)
{
  SDL_Point origin = selection.getOrigin();
  int xOffset = origin.x%tileSize;
  int yOffset = origin.y%tileSize;
  int startX = origin.x - xOffset;
  int countX = (selection.getWidth()+xOffset)/tileSize + 1;
  int w = selection.getWidth();
  int currentY = origin.y - yOffset;
  int countY = (selection.getHeight()+yOffset)/tileSize + 1;
  while(countY--){
    int currentX = startX;
    int count = countX;
    while(count--){
      if(Collision::between({currentX,currentY},frame) && tileDoesExistAt(currentX,currentY) == tiles.size())
        tiles.push_back(std::make_pair(id,Tile(texture,currentX,currentY)));
      currentX += tileSize;
    }
    currentY += tileSize;
  }
}

void TileCanvas::removeTiles(const SelectionBox &selection)
{
  SDL_Point origin = selection.getOrigin();
  int xOffset = origin.x%tileSize;
  int yOffset = origin.y%tileSize;
  int startX = origin.x - xOffset;
  int countX = (selection.getWidth()+xOffset)/tileSize + 1;
  int w = selection.getWidth();
  int currentY = origin.y - yOffset;
  int countY = (selection.getHeight()+yOffset)/tileSize + 1;
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
  if(backgroundColorIsSet){
    parentWindow->setColor(backgroundColor);
    parentWindow->fillRect(relativeFrame);
  }
  if(borderColorIsSet){
    parentWindow->setColor(borderColor);
    parentWindow->drawRect(relativeFrame);
  }
  for(const auto &tile : tiles)
    tile.second.render(parentWindow->getRenderer(),cam);
  parentWindow->setColor(prev);
}

void TileCanvas::render() const
{
  SDL_Color prev = parentWindow->getColor();
  if(backgroundColorIsSet){
    parentWindow->setColor(backgroundColor);
    parentWindow->fillRect(frame);
  }
  if(borderColorIsSet){
    parentWindow->setColor(borderColor);
    parentWindow->drawRect(frame);
  }
  for(const auto &tile : tiles)
    tile.second.render(parentWindow->getRenderer());
  parentWindow->setColor(prev);
}

void TileCanvas::updateTileId(int oldId, int newId)
{
  for(auto &tile : tiles)
    if(tile.first == oldId)
      tile.first = newId;
}