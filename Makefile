OBJS = main.cpp app.cpp windowWrapper.cpp timer.cpp texture.cpp font.cpp button.cpp collisionDetector.cpp \
  spriteLoadCallback.cpp listMenu.cpp changeTileIdCallback.cpp selectionBox.cpp guiElement.cpp textField.cpp popupInputBox.cpp \
  fileSystemExplorer.cpp camera.cpp errorHandler.cpp buttonMenu.cpp tileCanvas.cpp intersection.cpp
CC = "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe"
INCLUDE_PATHS = /ID:\\dev_libs\\sdl\\include
LIBRARY_PATHS = /LIBPATH:D:\\dev_libs\\sdl\\lib\\x64
COMPILER_FLAGS = /Zi /EHsc
LINKER_FLAGS = shell32.lib SDL2main.lib SDL2.lib SDL2_image.lib SDL2_ttf.lib Ole32.lib /SUBSYSTEM:CONSOLE
OBJ_NAME = TileMapEditor.exe
all : $(OBJS)
	$(CC) $(COMPILER_FLAGS) $(OBJS) $(INCLUDE_PATHS) /link $(LIBRARY_PATHS) $(LINKER_FLAGS) /Fe:$(OBJ_NAME)

clean :
	del *.obj *.pdb *.exe *.ilk *.db
