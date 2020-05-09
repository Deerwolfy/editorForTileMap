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
  canvas.updateTileId(oldId, newId);
  TextureName oldTexture = idToTextureName.find(oldId)->second;
  idToTextureName.erase(oldId);
  idToTextureName.insert({newId,oldTexture});
}