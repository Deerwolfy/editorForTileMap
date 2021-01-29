#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include<string>
#include<iostream>
#include<stdexcept>
#include<SDL.h>

class ErrorHandler {
public:
  enum MessageLevel { CRITICAL, WARNING, INFO };
  static void raise(const std::string &msg);
  static void raise(const char *msg);
  static void raise(std::string msg, const char *cmsg);
  static void errorMessageBox(const std::string &msg);
  static void createMessage(const std::string &msg, MessageLevel level);
  static void createMessage(const std::wstring &msg, MessageLevel level);
};

#endif