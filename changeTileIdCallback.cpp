#include"changeTileIdCallback.h"
#include"errorHandler.h"
#include<iostream>
#include<sstream>

void ChangeTileIdCallback::operator()(const PopupInputBox &b)
{
  std::istringstream userInput(b.getInput());
  int newId;
  int oldId = b.getElementId();
  userInput >> newId;
  if(userInput.fail()){
    ErrorHandler::errorMessageBox("Id must be a number!");
    return;
  }
  auto newIdTexture = idToTextureName.find(newId);
  if(newIdTexture != std::end(idToTextureName)){
    ErrorHandler::errorMessageBox(std::string("Id is already mapped to ")+newIdTexture->second.name);
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