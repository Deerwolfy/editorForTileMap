#ifndef FILESYSTEMEXPLORER_H
#define FILESYSTEMEXPLORER_H

#include<string>
#include<windows.h>
#include<ShObjIdl.h>
#include<memory>

class FileSystemExplorer {
public:
  FileSystemExplorer() = delete;
  static std::wstring openFile(const std::wstring &title = L"");
  static std::wstring openDirectory(const std::wstring &title = L"");
  static std::wstring saveFile(const std::wstring &title = L"");
private:
  static std::wstring chooseFileOpen(const std::wstring &title = L"", std::shared_ptr<FILEOPENDIALOGOPTIONS> options = nullptr);
  static std::wstring chooseFileSave(const std::wstring &title = L"", std::shared_ptr<FILEOPENDIALOGOPTIONS> options = nullptr);
};

#endif