SRCDIR = ./src
LIBTCODDIR = ../libtcod
INCDIR = $(LIBTCODDIR)/include
SODIR  = $(LIBTCODDIR)
BOOSTDIR = /usr/local/boost_1_46_0
BOOSTSO = /usr/local/boost_1_46_0/stage/lib
OBJDIR = ./obj
CXXFLAGS = -I$(INCDIR) -I$(SRCDIR) -I$(BOOSTDIR) -Wall -W -std=c++0x -O2 -fno-strict-aliasing
CXXDEBUGFLAGS = -I$(INCDIR) -I$(SRCDIR) -I$(BOOSTDIR) -Wall -W -std=c++0x -g -O0
LINKFLAGS = -L$(BOOSTSO) -Wl,-rpath,$(BOOSTSO) -L$(LIBTCODDIR) -ltcod -ltcodxx -Wl,-rpath,$(SODIR)
LINKDEBUGFLAGS = -L$(BOOSTSO) -Wl,-rpath,$(BOOSTSO) -L$(LIBTCODDIR) -ltcod_debug -ltcodxx_debug -Wl,-rpath,$(SODIR)
CC = gcc
CXX = ccache g++
.SUFFIXES: .o .h .c .hpp .cpp

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

$(OBJDIR)/%.dbg.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXDEBUGFLAGS) -o $@ -c $<
 

CXX_OBJS = \
	$(OBJDIR)/main.o \
	$(OBJDIR)/AbstractTile.o \
	$(OBJDIR)/Actor.o \
	$(OBJDIR)/Area.o \
	$(OBJDIR)/CheckedSave.o \
	$(OBJDIR)/Console.o \
	$(OBJDIR)/Display.o \
	$(OBJDIR)/DungeonGenerator.o \
	$(OBJDIR)/EventHandler.o \
	$(OBJDIR)/GameData.o \
	$(OBJDIR)/GameWindow.o \
	$(OBJDIR)/InitData.o \
	$(OBJDIR)/Level.o \
	$(OBJDIR)/LevelTree.o \
	$(OBJDIR)/Light.o \
	$(OBJDIR)/Map.o \
	$(OBJDIR)/MapGenerator.o \
	$(OBJDIR)/Point.o \
	$(OBJDIR)/Renderer.o \
	$(OBJDIR)/Tile.o \
	$(OBJDIR)/Tileset.o \
	$(OBJDIR)/Types.o \
	$(OBJDIR)/Utility.o


CXX_DEBUG_OBJS = \
	$(OBJDIR)/main.dbg.o \
	$(OBJDIR)/AbstractTile.dbg.o \
	$(OBJDIR)/Actor.dbg.o \
	$(OBJDIR)/Area.dbg.o \
	$(OBJDIR)/CheckedSave.dbg.o \
	$(OBJDIR)/Console.dbg.o \
	$(OBJDIR)/Display.dbg.o \
	$(OBJDIR)/DungeonGenerator.dbg.o \
	$(OBJDIR)/EventHandler.dbg.o \
	$(OBJDIR)/GameData.dbg.o \
	$(OBJDIR)/GameWindow.dbg.o \
	$(OBJDIR)/InitData.dbg.o \
	$(OBJDIR)/Level.dbg.o \
	$(OBJDIR)/LevelTree.dbg.o \
	$(OBJDIR)/Light.dbg.o \
	$(OBJDIR)/Map.dbg.o \
	$(OBJDIR)/MapGenerator.dbg.o \
	$(OBJDIR)/Point.dbg.o \
	$(OBJDIR)/Renderer.dbg.o \
	$(OBJDIR)/Tile.dbg.o \
	$(OBJDIR)/Tileset.dbg.o \
	$(OBJDIR)/Types.dbg.o \
	$(OBJDIR)/Utility.dbg.o

all : release debug e_fence

release : $(CXX_OBJS)
	$(CXX) $(CXX_OBJS) -o $@ $(LINKFLAGS)

debug : $(CXX_DEBUG_OBJS)
	$(CXX) $(CXX_DEBUG_OBJS) -o $@ $(LINKDEBUGFLAGS)

e_fence : $(CXX_DEBUG_OBJS)
	$(CXX) $(CXX_DEBUG_OBJS) -o $@ $(LINKDEBUGFLAGS) -lefence

clean :
	\rm -f $(CXX_OBJS) $(CXX_DEBUG_OBJS)

cleanAll :
	\rm -f $(CXX_OBJS) $(CXX_DEBUG_OBJS) release debug e_fence

cleanSaves :
	\rm -f ./save/*.sav

