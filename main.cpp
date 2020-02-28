#include"app.h"
#include<stdexcept>

int main(int argc, char *argv[])
{
  try{
    App editor;
    editor.run();
  }
  catch(std::runtime_error e){
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",e.what(),nullptr);
  }
  return 0;
}
