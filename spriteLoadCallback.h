#ifndef SPRITELOADCALLBACK_H
#define SPRITELOADCALLBACK_H

#include<map>
#include"button.h"
#include<string>
#include"windowWrapper.h"
#include"texture.h"

class SpriteLoadCallback {
public:
  SpriteLoadCallback(std::map<int,Texture> &idToTexture, std::map<int,std::string> &idToName, WindowWrapper &w, bool &reg):
    idToTexture(idToTexture), idToName(idToName), w(w), regenerate(reg) { }
  void operator()(const Button&);
private:
  std::map<int,Texture> &idToTexture;
  std::map<int,std::string> &idToName;
  WindowWrapper &w;
  bool &regenerate;
};

#endif