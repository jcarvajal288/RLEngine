#ifndef RLNS_MENUSCREEN_HPP
#define RLNS_MENUSCREEN_HPP

#include <boost/lexical_cast.hpp>
#include <vector>

#include "Actor.hpp"
#include "Display.hpp"
#include "EventHandler.hpp"
#include "Level.hpp"
#include "Point.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Class       : MenuScreen
        Description : Base class for all the menuscreens in the game.  A MenuScreen
                      replaces the main game window with various textual information,
                      and has its own event loop.  Examples of a MenuScreen are the
                      inventory screen, character record screen, etc.
        Parents     : None
        Children    : InventoryScreen, CharacterRecordScreen, and more
        Friends     : None
    --------------------------------------------------------------------------------*/
    class MenuScreen
    {
        // Member Variables
        private:
            static const int TABWIDTH = 5;
            static const int TABHEIGHT = 3;

        protected:
            TCODConsole screen;
            DisplayPtr display;
            int selectedLine;
            int currentPage;
            int selectedTab;
            int linesPerPage;

            bool keepRunning;

        // Member Functions
        protected:
            void incSelectedLine(const int);
            void decSelectedLine(const int);
            void incSelectedTab(const int);
            void decSelectedTab(const int);
            void pageUp(const int);
            void pageDown(const int);
            Point drawFolders(int&, int&, const Point&, 
                                       const std::vector< std::pair<int, TCODColor> >&);
            void drawScrollBar(const Point&, const int, const int, const int, const bool vert=true);
            void drawTabs(int&, int&, const std::vector< std::pair<int, TCODColor> >&);

            void update();

            virtual void refresh();

        public: 
            MenuScreen(const DisplayPtr _display)
            : screen(_display->playfield()->getWidth(), 
                     _display->playfield()->getHeight()),
              display(_display), selectedLine(0),
              currentPage(0), selectedTab(0), linesPerPage(0),
              keepRunning(true)
            {}

            virtual ~MenuScreen() {}

            virtual bool eventLoop();
    };



    /*--------------------------------------------------------------------------------
        Class       : InventoryScreen
        Description : Displays the contents of the character's inventory organized by
                      Item type.
        Parents     : MenuScreen
        Children    : None
        Friends     : None
    --------------------------------------------------------------------------------*/
    class InventoryScreen: public MenuScreen
    {
        // Member Variables
        private:
            ActorPtr client;

        // Member Functions
        protected:
            void refresh();

        public:
            InventoryScreen(const DisplayPtr _display, const ActorPtr c)
            : MenuScreen(_display), client(c) {}

            void interpretEvent(const EventType);

            virtual bool eventLoop();
    };
}

#endif
