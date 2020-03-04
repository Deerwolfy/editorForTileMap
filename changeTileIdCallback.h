#ifndef CHANGETILEIDCALLBACK_H
#define CHANGETILEIDCALLBACK_H

#include<map>
#include"button.h"
#include"windowWrapper.h"
#include"texture.h"
#include"textureName.h"

class ChangeTileIdCallback {
public:
  ChangeTileIdCallback(WindowWrapper &w,std::map<int,TextureName> &idToTextureName):
    w(w), idToTextureName(idToTextureName) { }
  void operator()(const Button&);
private:
  WindowWrapper &w;
  std::map<int,TextureName> &idToTextureName;
};

#endif