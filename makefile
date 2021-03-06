SRCDIR = ./src
OBJDIR = ./obj
LIBTCODDIR = ../libtcod-1.5.1
SODIR  = $(LIBTCODDIR)
INCDIR = $(LIBTCODDIR)/include
BOOSTDIR = /usr/include/boost_1_56_0
CXXFLAGS = -I$(INCDIR) -I$(BOOSTDIR) -std=c++11 -O1 
CXXDEBUGFLAGS = -I$(INCDIR) -I$(BOOSTDIR) -std=c++11 -g -O0
"WARNINGFLAGS = -Werror -Weverything -Wno-weak-vtables -Wno-c++98-compat -Wno-padded -Wno-global-constructors -Wno-exit-time-destructors
WARNINGFLAGS = -Wall
LINKFLAGS = -L$(BOOSTSO) -Wl,-rpath,$(BOOSTSO) -L$(LIBTCODDIR) -ltcod -ltcodxx -Wl,-rpath,$(SODIR)
LINKDEBUGFLAGS = -L$(BOOSTSO) -Wl,-rpath,$(BOOSTSO) -L$(LIBTCODDIR) -ltcod_debug -ltcodxx_debug -Wl,-rpath,$(SODIR)
CXX = clang++
.SUFFIXES: .o .h .c .hpp .cpp

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(WARNINGFLAGS) -o $@ -c $< 

$(OBJDIR)/%.dbg.o : $(SRCDIR)/%.cpp
	$(CXX) $(CXXDEBUGFLAGS) $(WARNINGFLAGS) -o $@ -c $<
 

CXX_OBJS = \
	$(OBJDIR)/AbilityScore.o \
	$(OBJDIR)/AbstractTile.o \
	$(OBJDIR)/Actor.o \
	$(OBJDIR)/Area.o \
	$(OBJDIR)/CaveBuilder.o \
	$(OBJDIR)/CheckedSave.o \
	$(OBJDIR)/Dice.o \
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
	$(OBJDIR)/Utility.o \
	$(OBJDIR)/VitalStats.o 


CXX_DEBUG_OBJS = \
	$(OBJDIR)/AbilityScore.dbg.o \
	$(OBJDIR)/AbstractTile.dbg.o \
	$(OBJDIR)/Actor.dbg.o \
	$(OBJDIR)/Area.dbg.o \
	$(OBJDIR)/CaveBuilder.dbg.o \
	$(OBJDIR)/CheckedSave.dbg.o \
	$(OBJDIR)/Dice.dbg.o \
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
	$(OBJDIR)/Utility.dbg.o \
	$(OBJDIR)/VitalStats.dbg.o

CXX_TEST_OBJS = \
	$(OBJDIR)/test.dbg.o

all : release debug

release : $(OBJDIR)/lcrl.o $(CXX_OBJS)
	$(CXX) $(CXX_OBJS) $(OBJDIR)/lcrl.o -o $@ $(LINKFLAGS)

debug : $(OBJDIR)/lcrl.dbg.o $(CXX_DEBUG_OBJS)
	$(CXX) $(CXX_DEBUG_OBJS) $(OBJDIR)/lcrl.dbg.o -o $@ $(LINKDEBUGFLAGS)

test : $(CXX_DEBUG_OBJS) $(CXX_TEST_OBJS)
	$(CXX) $(CXX_DEBUG_OBJS) $(CXX_TEST_OBJS) -o $@ $(LINKDEBUGFLAGS)

clean :
	\rm -f $(CXX_OBJS) $(CXX_DEBUG_OBJS) $(CXX_DEBUG_OBJS) $(OBJDIR)/lcrl.o $(OBJDIR)/lcrl.dbg.o 

cleanAll :
	\rm -f $(CXX_OBJS) $(CXX_DEBUG_OBJS) $(CXX_DEBUG_OBJS) $(OBJDIR)/lcrl.o $(OBJDIR)/lcrl.dbg.o 
