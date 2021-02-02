#include"font.h"
#include"errorHandler.h"

Font::Font(const std::string &fontPath, int pSize, SDL_Color color):
    color(color), size(pSize), font(TTF_OpenFont(fontPath.c_str(), size),TTF_CloseFont)
{
  if(font == nullptr)
    ErrorHandler::createMessage("Failed to load font " + fontPath + " reason: " + TTF_GetError(),ErrorHandler::MessageLevel::WARNING);
}

std::pair<int,int> Font::getTextWH(const std::string &text) const
{
  int w, h;
  if(TTF_SizeText(font.get(),text.c_str(),&w,&h) == 0)
    return std::make_pair(w,h);
  return std::make_pair(0,0);
}

void Font::loadFont(const std::string &path, int size)
{
  font = std::shared_ptr<TTF_Font>(TTF_OpenFont(path.c_str(), size),TTF_CloseFont);
}