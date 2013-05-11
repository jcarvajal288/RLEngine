SRCDIR = ./src
TESTSRCDIR = ./test
LIBTCODDIR = ../libtcod-1.5.1
GTESTDIR = /usr/local/gtest-1.6.0
INCDIR = $(LIBTCODDIR)/include
SODIR  = $(LIBTCODDIR)
BOOSTDIR = /usr/local/boost_1_53_0
BOOSTSO = $(BOOSTDIR)/stage/lib
OBJDIR = ./obj
CXXFLAGS = -I$(INCDIR) -I$(SRCDIR) -I$(BOOSTDIR) -Wall -W -std=c++0x -O2 -fno-strict-aliasing
CXXDEBUGFLAGS = -I$(INCDIR) -I$(SRCDIR) -I$(BOOSTDIR) -Wall -W -std=c++0x -g -O0
TESTFLAGS = -I$(GTESTDIR)/include -I$(GTESTDIR) $(CXXFLAGS)
TESTDEBUGFLAGS = -I$(GTESTDIR)/include -I$(GTESTDIR) $(CXXDEBUGFLAGS)
LINKFLAGS = -L$(BOOSTSO) -Wl,-rpath,$(BOOSTSO) -L$(LIBTCODDIR) -ltcod -ltcodxx -Wl,-rpath,$(SODIR)
LINKDEBUGFLAGS = -L$(BOOSTSO) -Wl,-rpath,$(BOOSTSO) -L$(LIBTCODDIR) -ltcod_debug -ltcodxx_debug -Wl,-rpath,$(SODIR)
GTESTLINKFLAGS = -L$(OBJDIR) -Wl,-rpath,$(OBJDIR) -lgtest
CC = gcc
CXX = ccache g++
.SUFFIXES: .o .h .c .hpp .cpp .cc

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

$(OBJDIR)/%.dbg.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXDEBUGFLAGS) -o $@ -c $<
 
$(OBJDIR)/%.o : $(TESTSRCDIR)/%.cpp
	$(CXX) $(TESTFLAGS) -o $@ -c $< 

$(OBJDIR)/%.dbg.o : $(TESTSRCDIR)/%.cpp
	$(CXX) $(TESTDEBUGFLAGS) -o $@ -c $<

CXX_OBJS = \
	$(OBJDIR)/AbstractTile.o \
	$(OBJDIR)/Actor.o \
	$(OBJDIR)/Area.o \
	$(OBJDIR)/CaveBuilder.o \
	$(OBJDIR)/CheckedSave.o \
	$(OBJDIR)/Display.o \
	$(OBJDIR)/DungeonBuilder.o \
	$(OBJDIR)/Events.o \
	$(OBJDIR)/EventHandler.o \
	$(OBJDIR)/InitData.o \
	$(OBJDIR)/Inventory.o \
	$(OBJDIR)/Level.o \
	$(OBJDIR)/Map.o \
	$(OBJDIR)/MapBuilder.o \
	$(OBJDIR)/MenuScreen.o \
	$(OBJDIR)/MessageTracker.o \
	$(OBJDIR)/Party.o \
	$(OBJDIR)/Point.o \
	$(OBJDIR)/RoomFiller.o \
	$(OBJDIR)/Tile.o \
	$(OBJDIR)/Tileset.o \
	$(OBJDIR)/Types.o \
	$(OBJDIR)/Utility.o 

CXX_TEST_OBJS = \
	$(OBJDIR)/gtest-all.o \
	$(OBJDIR)/gtest_main.o \
	$(OBJDIR)/CaveBuilder_unittest.o \
	$(OBJDIR)/Display_unittest.o \
	$(OBJDIR)/DungeonBuilder_unittest.o \
	$(OBJDIR)/EventHandler_unittest.o \
	$(OBJDIR)/InitData_unittest.o \
	$(OBJDIR)/Map_unittest.o \
	$(OBJDIR)/Tile_unittest.o \
	$(OBJDIR)/Tileset_unittest.o \
	$(OBJDIR)/Utility_unittest.o \
	$(OBJDIR)/UnitTest.o 

CXX_DEBUG_OBJS = \
	$(OBJDIR)/AbstractTile.dbg.o \
	$(OBJDIR)/Actor.dbg.o \
	$(OBJDIR)/Area.dbg.o \
	$(OBJDIR)/CaveBuilder.dbg.o \
	$(OBJDIR)/CheckedSave.dbg.o \
	$(OBJDIR)/Display.dbg.o \
	$(OBJDIR)/DungeonBuilder.dbg.o \
	$(OBJDIR)/Events.dbg.o \
	$(OBJDIR)/EventHandler.dbg.o \
	$(OBJDIR)/InitData.dbg.o \
	$(OBJDIR)/Inventory.dbg.o \
	$(OBJDIR)/Level.dbg.o \
	$(OBJDIR)/Map.dbg.o \
	$(OBJDIR)/MapBuilder.dbg.o \
	$(OBJDIR)/MenuScreen.dbg.o \
	$(OBJDIR)/MessageTracker.dbg.o \
	$(OBJDIR)/Party.dbg.o \
	$(OBJDIR)/Point.dbg.o \
	$(OBJDIR)/RoomFiller.dbg.o \
	$(OBJDIR)/Tile.dbg.o \
	$(OBJDIR)/Tileset.dbg.o \
	$(OBJDIR)/Types.dbg.o \
	$(OBJDIR)/Utility.dbg.o

CXX_DEBUG_TEST_OBJS = \
	$(OBJDIR)/gtest-all.o \
	$(OBJDIR)/gtest_main.o \
	$(OBJDIR)/CaveBuilder_unittest.dbg.o \
	$(OBJDIR)/Display_unittest.dbg.o \
	$(OBJDIR)/DungeonBuilder_unittest.dbg.o \
	$(OBJDIR)/EventHandler_unittest.dbg.o \
	$(OBJDIR)/InitData_unittest.dbg.o \
	$(OBJDIR)/Map_unittest.dbg.o \
	$(OBJDIR)/Tile_unittest.dbg.o \
	$(OBJDIR)/Tileset_unittest.dbg.o \
	$(OBJDIR)/Utility_unittest.dbg.o \
	$(OBJDIR)/UnitTest.dbg.o

all : release debug 

lib : $(OBJDIR)/gtest-all.o
	ar -rv $(OBJDIR)/libgtest.a $(OBJDIR)/gtest-all.o

release : $(OBJDIR)/lcrl.o $(CXX_OBJS)
	$(CXX) $(OBJDIR)/lcrl.o $(CXX_OBJS) -o $@ $(LINKFLAGS)

debug : $(OBJDIR)/lcrl.dbg.o $(CXX_DEBUG_OBJS)
	$(CXX) $(OBJDIR)/lcrl.dbg.o $(CXX_DEBUG_OBJS) -o $@ $(LINKDEBUGFLAGS)

test-release : $(CXX_OBJS) $(CXX_TEST_OBJS)
	$(CXX) $(CXX_OBJS) $(CXX_TEST_OBJS) -o $@ $(LINKFLAGS) $(GTESTLINKFLAGS)

test-debug : $(CXX_DEBUG_OBJS) $(CXX_DEBUG_TEST_OBJS)
	$(CXX) $(CXX_DEBUG_OBJS) $(CXX_DEBUG_TEST_OBJS) -o $@ $(LINKDEBUGFLAGS) $(GTESTLINKFLAGS)

clean :
	\rm -f $(CXX_OBJS) $(CXX_TEST_OBJS) $(CXX_DEBUG_OBJS) $(CXX_DEBUG_TEST_OBJS) 

cleanAll :
	\rm -f $(CXX_OBJS) $(CXX_TEST_OBJS) $(CXX_DEBUG_OBJS) $(CXX_DEBUG_TEST_OBJS) release debug test-release test-debug

cleanSaves :
	\rm -f ./save/*.sav

