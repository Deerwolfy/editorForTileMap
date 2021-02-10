#include"tileCanvas.h"
#include"collisionDetector.h"
#include<locale>
#include<codecvt>

void TileCanvas::placeTiles(const SelectionBox &selection, const Camera &cam, const Texture &texture, int id)
{
  SDL_Point origin = selection.getOrigin(cam);
  int xOffset = (origin.x-frame.x)%tileSize;
  int yOffset = (origin.y-frame.y)%tileSize;
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
  int xOffset = (origin.x-frame.x)%tileSize;
  int yOffset = (origin.y-frame.y)%tileSize;
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
  parentWindow->setClip(&relativeFrame);
  if(backgroundTextureIsSet){
    backgroundTexture.render(*parentWindow,relativeFrame.x,relativeFrame.y);
  }
  for(const auto &tile : tiles)
    tile.second.render(*parentWindow,cam);
  parentWindow->setClip();
  parentWindow->setColor(prev);
}

void TileCanvas::render() const
{
  SDL_Color prev = parentWindow->getColor();
  SDL_Rect nonConstFrame = frame;
  if(backgroundColorIsSet){
    parentWindow->setColor(backgroundColor);
    parentWindow->fillRect(frame);
  }
  if(borderColorIsSet){
    parentWindow->setColor(borderColor);
    parentWindow->drawRect(frame);
  }
  parentWindow->setClip(&nonConstFrame);
  if(backgroundTextureIsSet){
    backgroundTexture.render(*parentWindow,frame.x,frame.y);
  }
  for(const auto &tile : tiles)
    tile.second.render(*parentWindow);
  parentWindow->setClip();
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

void TileCanvas::scaleUp(double coefficient)
{

}

void TileCanvas::scaleDown(double coefficient)
{
  
}