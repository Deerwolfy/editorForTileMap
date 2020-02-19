#include"spriteLoadCallback.h"
#include<iostream>
#include<utility>
#include"texture.h"
#include<windows.h>
#include<string>
#include<vector>

void SpriteLoadCallback::operator()(const Button&)
{
  std::string path = "sprites/";
  std::string filePath = path + "*";
  const int formatLength = 4;
  WIN32_FIND_DATAA data;
  HANDLE dHandle = FindFirstFileA(filePath.c_str(),&data);
  if(dHandle == INVALID_HANDLE_VALUE){
    std::cout << "Can't open directory" << path << std::endl;
    return;
  }
  int spriteCount = 0;
  idToTexture.clear();
  idToName.clear();
  do {
    std::string fileName(data.cFileName);
    if(fileName == "." || fileName == ".." || fileName.length() <= formatLength)
      continue;
    std::string format = fileName.substr(fileName.length() - formatLength, formatLength);
    if(format == ".png" || format == ".jpg"){
      std::cout << "Loading " << path + fileName << std::endl;
      idToTexture.emplace(std::make_pair(spriteCount,Texture(w,path + fileName)));
      idToName.emplace(std::make_pair(spriteCount,fileName));
      ++spriteCount;
    }
  } while(FindNextFileA(dHandle,&data) != 0);
  regenerate = true;
  FindClose(dHandle);
}