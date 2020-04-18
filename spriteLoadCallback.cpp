#include"spriteLoadCallback.h"
#include<iostream>
#include<utility>
#include"texture.h"
#include<windows.h>
#include<string>
#include<vector>
#include<locale>
#include<codecvt>
#include"fileSystemExplorer.h"

void SpriteLoadCallback::operator()(const GuiElement&)
{
  std::wstring path = FileSystemExplorer::openDirectory(L"Select sprite folder");
  if(path.empty())
    return;
  if(path.back() != '/' || path.back() != '\\')
    path += '\\';
  std::wstring filePath = path + L"*";
  const int formatLength = 4;
  WIN32_FIND_DATAW data;
  HANDLE dHandle = FindFirstFileW(filePath.c_str(),&data);
  if(dHandle == INVALID_HANDLE_VALUE){
    std::wcout << "Can't open directory" << path << std::endl;
    return;
  }
  int spriteCount = 0;
  idToTextureName.clear();
  do {
    std::wstring fileName(data.cFileName);
    if(fileName == L"." || fileName == L".." || fileName.length() <= formatLength)
      continue;
    auto formatIndex = fileName.find_last_of(L".");
    std::wstring format;
    if(formatIndex != std::string::npos)
      format = fileName.substr(formatIndex, formatLength);
    if(format == L".png" || format == L".jpg"){
      std::wcout << "Loading " << path + fileName << std::endl;
      std::string fileNameNarrow = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(fileName);
      std::string pathNarrow = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(path);
      idToTextureName.emplace(spriteCount,TextureName{Texture(window->getRenderer(),pathNarrow + fileNameNarrow), fileNameNarrow});
      ++spriteCount;
    }
    else
      std::wcout << L"Unsupported format: " << format << std::endl;
  } while(FindNextFileW(dHandle,&data) != 0);
  regenerate = true;
  FindClose(dHandle);
}