#include"errorHandler.h"
#include<codecvt>

void ErrorHandler::raise(const std::string &msg)
{
  raise(msg.c_str());
}

void ErrorHandler::raise(const char *msg)
{
  throw std::runtime_error(msg);
}

void ErrorHandler::raise(std::string msg, const char *cmsg)
{
  msg.append(cmsg);
  raise(msg.c_str());
}

void ErrorHandler::errorMessageBox(const std::string &msg)
{
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",msg.c_str(),nullptr);
}
void ErrorHandler::createMessage(const std::string &msg, MessageLevel level)
{
  createMessage(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(msg),level);
}
void ErrorHandler::createMessage(const std::wstring &msg, MessageLevel level)
{
  switch(level){
    case CRITICAL:
      std::wcout << msg << std::endl;
    break;
    case WARNING:
      std::wcout << msg << std::endl;
    break;
    case INFO:
      std::wcout << msg << std::endl;
    break;
    default:
      std::wcout << msg << std::endl;
    break;
  }
}