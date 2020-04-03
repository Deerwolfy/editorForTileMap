#include"changeTileIdCallback.h"
#include<iostream>

void ChangeTileIdCallback::operator()(const GuiElement &b)
{
  std::cout << "!";
}