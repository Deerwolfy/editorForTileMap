OBJS = main.cpp app.cpp windowWrapper.cpp timer.cpp texture.cpp font.cpp button.cpp collisionDetector.cpp \
  spriteLoadCallback.cpp listMenu.cpp changeTileIdCallback.cpp selectionBox.cpp guiElement.cpp textField.cpp
CC = g++
INCLUDE_PATHS = -IC:\mingw_dev_lib\include\SDL2
LIBRARY_PATHS = -LC:\mingw_dev_lib\lib
COMPILER_FLAGS = -mwindows
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
OBJ_NAME = TileMapEditor.exe
all : $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)
