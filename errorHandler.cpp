#include"errorHandler.h"

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