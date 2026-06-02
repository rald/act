# Project paths
INCLUDE_DIR = -ISDL2/include
LIB_DIR = -LSDL2/lib

# Core compilation
CXX = g++
CXXFLAGS = -O2 -Wall -g

# Static compilation strings
STATIC_FLAGS = -Wl,-Bstatic -lSDL2main -lSDL2 -Wl,-Bdynamic
WIN_SYS_LIBS = -lmingw32 -mwindows -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

all:
	$(CXX) main.cpp sweetie.cpp -o game.exe $(INCLUDE_DIR) $(LIB_DIR) $(CXXFLAGS) $(STATIC_FLAGS) $(WIN_SYS_LIBS)
