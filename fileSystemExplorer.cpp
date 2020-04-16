#include"fileSystemExplorer.h"
#include<SDL.h>

std::wstring FileSystemExplorer::chooseFileOpen(const std::wstring &title, std::shared_ptr<FILEOPENDIALOGOPTIONS> options)
{
  HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  if(SUCCEEDED(hr)){
    IFileOpenDialog *commonDialog;
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER,
           IID_IFileOpenDialog, reinterpret_cast<void**>(&commonDialog));
    if(SUCCEEDED(hr)){
      if(options){
        FILEOPENDIALOGOPTIONS *currentOptions = new FILEOPENDIALOGOPTIONS();
        hr = commonDialog->GetOptions(currentOptions);
        if(SUCCEEDED(hr))
          commonDialog->SetOptions((*currentOptions) | (*(options.get())));
        delete currentOptions;
      }
      if(!title.empty()){
        commonDialog->SetTitle(title.c_str());
      }
      hr = commonDialog->Show(NULL);
      if(SUCCEEDED(hr)){
        IShellItem *selectedItem;
        hr = commonDialog->GetResult(&selectedItem);
        if(SUCCEEDED(hr)){
          PWSTR filePath;
          hr = selectedItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
          if(SUCCEEDED(hr)){
            return filePath;
            CoTaskMemFree(filePath);
          }
          selectedItem->Release();
        }
      }
      commonDialog->Release();
    }
    CoUninitialize();
  }
  return L"";
}

std::wstring FileSystemExplorer::chooseFileSave(const std::wstring &title, std::shared_ptr<FILEOPENDIALOGOPTIONS> options)
{
  HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
  if(SUCCEEDED(hr)){
    IFileSaveDialog *commonDialog;
    hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER,
           IID_IFileSaveDialog, reinterpret_cast<void**>(&commonDialog));
    if(SUCCEEDED(hr)){
      if(options){
        FILEOPENDIALOGOPTIONS *currentOptions = new FILEOPENDIALOGOPTIONS();
        hr = commonDialog->GetOptions(currentOptions);
        if(SUCCEEDED(hr))
          commonDialog->SetOptions((*currentOptions) | (*(options.get())));
        delete currentOptions;
      }
      if(!title.empty()){
        commonDialog->SetTitle(title.c_str());
      }
      hr = commonDialog->Show(NULL);
      if(SUCCEEDED(hr)){
        IShellItem *selectedItem;
        hr = commonDialog->GetResult(&selectedItem);
        if(SUCCEEDED(hr)){
          PWSTR filePath;
          hr = selectedItem->GetDisplayName(SIGDN_FILESYSPATH, &filePath);
          if(SUCCEEDED(hr)){
            return filePath;
            CoTaskMemFree(filePath);
          }
          selectedItem->Release();
        }
      }
      commonDialog->Release();
    }
    CoUninitialize();
  }
  return L"";
}

std::wstring FileSystemExplorer::openFile(const std::wstring &title)
{
  return chooseFileOpen(title);
}

std::wstring FileSystemExplorer::openDirectory(const std::wstring &title)
{
  auto options = std::make_shared<FILEOPENDIALOGOPTIONS>(FOS_PICKFOLDERS);
  std::wstring path =  chooseFileOpen(title,options);
  return path;
}

std::wstring FileSystemExplorer::saveFile(const std::wstring &title)
{
  return chooseFileSave(title);
}