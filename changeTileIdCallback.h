#ifndef CHANGETILEIDCALLBACK_H
#define CHANGETILEIDCALLBACK_H

#include<map>
#include"button.h"
#include"windowWrapper.h"
#include"texture.h"

class ChangeTileIdCallback {
public:
  ChangeTileIdCallback(WindowWrapper &w,std::map<int,Texture> &idToTexture,std::map<int,std::string> &idToName):
    w(w), idToTexture(idToTexture), idToName(idToName) { }
  void operator()(const Button&);
private:
  WindowWrapper &w;
  std::map<int,Texture> &idToTexture;
  std::map<int,std::string> &idToName;
};

#endif