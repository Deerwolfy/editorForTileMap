#include"changeTileIdCallback.h"
#include<iostream>
#include<sstream>

void ChangeTileIdCallback::operator()(const PopupInputBox &b)
{
  std::istringstream userInput(b.getInput());
  int newId;
  int oldId = b.getElementId();
  userInput >> newId;
  if(userInput.fail())
    return;
  auto newIdTexture = idToTextureName.find(newId);
  if(newIdTexture != std::end(idToTextureName)){
    std::string message("Id already mapped to ");
    message += newIdTexture->second.name;
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",message.c_str(),nullptr);
    return;
  }
  std::map<int,TextureName> newMap;
  for(const auto &entry : idToTextureName){
    if(entry.first == oldId)
      newMap.insert({newId,entry.second});
    else
      newMap.insert({entry.first,entry.second});
  }
  idToTextureName = std::move(newMap);
}