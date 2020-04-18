#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include<string>
#include<stdexcept>
#include<SDL.h>

class ErrorHandler {
public:
  static void raise(const std::string &msg);
  static void raise(const char *msg);
  static void raise(std::string msg, const char *cmsg);
  static void errorMessageBox(const std::string &msg);
};

#endif