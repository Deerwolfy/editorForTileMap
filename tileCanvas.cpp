#include"tileCanvas.h"
#include"collisionDetector.h"
#include"intersection.h"
#include<locale>
#include<codecvt>
#include<cmath>

void TileCanvas::placeTiles(const SelectionBox &selection, const Camera &cam, const Texture &texture, int id)
{
  SDL_Rect canvas = frame;
  getZoomedFrame(canvas,cam.getZoom());
  std::shared_ptr<SDL_Rect> intersection = Intersection::between(canvas,selection.getFrame(cam));
  if(intersection == nullptr)
    return;
  revertZoomOnFrame(*intersection,cam.getZoom());
  intersection->x -= frame.x;
  intersection->y -= frame.y;
  actualPlace(*intersection,texture,id);
}

void TileCanvas::placeTiles(const SelectionBox &selection, const Texture &texture, int id)
{
  std::shared_ptr<SDL_Rect> region = Intersection::between(frame,selection.getFrame());
  if(region == nullptr)
    return;
  region->x -= frame.x;
  region->y -= frame.y;
  actualPlace(*region,texture,id);
}

void TileCanvas::removeTiles(const SelectionBox &selection, const Camera &cam)
{
  SDL_Rect canvas = frame;
  getZoomedFrame(canvas,cam.getZoom());
  std::shared_ptr<SDL_Rect> intersection = Intersection::between(canvas,selection.getFrame(cam));
  if(intersection == nullptr)
    return;
  revertZoomOnFrame(*intersection,cam.getZoom());
  intersection->x -= frame.x;
  intersection->y -= frame.y;
  intersection->x = std::round(intersection->x*(1/cam.getZoom()));
  intersection->y = std::round(intersection->y*(1/cam.getZoom()));
  actualDelete(*intersection);
}

void TileCanvas::removeTiles(const SelectionBox &selection)
{
  std::shared_ptr<SDL_Rect> region = Intersection::between(frame,selection.getFrame());
  if(region == nullptr)
    return;
  region->x -= frame.x;
  region->y -= frame.y;
  actualDelete(*region);
}

void TileCanvas::actualPlace(const SDL_Rect &region, const Texture &texture, int id)
{
  int xOffset = region.x%tileSize;
  int yOffset = region.y%tileSize;
  int startX = region.x - xOffset;
  int countX = region.w/tileSize + 1;
  int countY = region.h/tileSize + 1;
  int currentY = region.y - yOffset;
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

void TileCanvas::actualDelete(const SDL_Rect &region)
{
  int xOffset = region.x%tileSize;
  int yOffset = region.y%tileSize;
  int startX = region.x - xOffset;
  int countX = region.w/tileSize + 1;
  int countY = region.h/tileSize + 1;
  int currentY = region.y - yOffset;
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
  SDL_Color prev = parentWindow->getColor();
  SDL_Rect textureFrame = {0,0,frame.w,frame.h};
  SDL_Rect dstFrame = cam.getRelativeRect(frame);
  getZoomedFrame(dstFrame,cam.getZoom());
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

void TileCanvas::getZoomedFrame(SDL_Rect &fr, double zoom) const
{
  if(zoom != 1){
    int hDimGain = std::round((std::round(fr.w*zoom)-fr.w)/3);
    int vDimGain = std::round((std::round(fr.h*zoom)-fr.h)/3);
    fr.w += hDimGain*2;
    fr.h += vDimGain*2;
    fr.x -= hDimGain;
    fr.y -= vDimGain;
  }
}

void TileCanvas::revertZoomOnFrame(SDL_Rect &fr,double zoom) const
{
  if(zoom != 1){
    int hDimGain = std::round((fr.w-std::round(fr.w*(1/zoom)))/3);
    int vDimGain = std::round((fr.h-std::round(fr.h*(1/zoom)))/3);
    fr.w -= hDimGain*2;
    fr.h -= vDimGain*2;
    fr.x += hDimGain;
    fr.y += vDimGain;
  }
}