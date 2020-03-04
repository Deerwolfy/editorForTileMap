#ifndef CHANGETILEIDCALLBACK_H
#define CHANGETILEIDCALLBACK_H

#include<map>
#include"button.h"
#include"windowWrapper.h"
#include"texture.h"
#include<utility>

class ChangeTileIdCallback {
public:
  ChangeTileIdCallback(WindowWrapper &w,std::map<int,std::pair<Texture,std::string>> &idToTextureName):
    w(w), idToTextureName(idToTextureName) { }
  void operator()(const Button&);
private:
  WindowWrapper &w;
  std::map<int,std::pair<Texture,std::string>> &idToTextureName;
};

#endif