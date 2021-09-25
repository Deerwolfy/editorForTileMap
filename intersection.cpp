#include"intersection.h"
#include<utility>

std::shared_ptr<SDL_Rect> Intersection::between(const SDL_Rect &rectOne, const SDL_Rect &rectTwo)
{
  std::shared_ptr<SDL_Rect> intersection = std::make_shared<SDL_Rect>();
  // guess which box starts first
  const SDL_Rect *first = &rectOne;
  const SDL_Rect *second = &rectTwo;
  // horizontal dimension
  if(first->x > second->x){
    std::swap(first,second); // swap if initial guess is wrong
  }
  if(first->x + first->w < second->x){
    return nullptr; // first box ends before second starts
  }
  intersection->x = second->x;
  if(first->x + first->w < second->x + second->w){
    intersection->w = first->x + first->w - second->x;
  }
  else {
    intersection->w = second->w;
  }
  // vertical dimension
  if(first->y > second->y){
    std::swap(first,second);
  }
  if(first->y + first->h < second->y){
    return nullptr; // first box ends before second starts
  }
  intersection->y = second->y;
  if(first->y + first->h < second->y + second->h){
    intersection->h = first->y + first->h - second->y;
  }
  else {
    intersection->h = second->h;
  }
  return intersection;
}