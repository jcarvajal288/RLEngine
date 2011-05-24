#ifndef RLNS_DISPLAY_HPP
#define RLNS_DISPLAY_HPP

#include <utility>

#include "Console.hpp"
#include "GameWindow.hpp"
#include "InitData.hpp"
#include "Point.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace view
    {
        /*--------------------------------------------------------------------------------
            Class       : Display
            Description : Manages all of the display functions and data that go into the
                          main game window. Uses the Singleton design.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Display
        {
            // Member Variables
            private:
                static DisplayPtr _instance;
                bool initialized; // lets get() know if Display has been properly initialized.

                GameWindow gameWindow;
                Console console;

                std::string commandLine;
                //DisplayWindow partyWindow;

            // Member Functions
            protected:
                Display() // throwaway default constructor
                : initialized(false),
                  gameWindow(utl::Point(0,0), utl::Point(0,0)),
                  console(utl::Point(0,0), utl::Point(0,0)) {}

                // single parameter constructor overload for when after InitData() has
                // read init.txt and so has enough data to properly construct Display
                Display(const bool b)
                : initialized(b),
                  gameWindow(utl::Point(0,0), utl::Point(InitData::get()->getGwTileWidth(),
                                                         InitData::get()->getGwTileHeight())),
                  console(InitData::get()->getConsoleTL(), InitData::get()->getConsoleBR()) {}

            public:
                static DisplayPtr get();

                std::pair<utl::Point, utl::Point> gameWindowCoords() const;
                utl::Point gameWindowTL() const { return gameWindow.topLeft(); }
                utl::Point gameWindowBR() const { return gameWindow.bottomRight(); }
                utl::Point getFocalPoint() const { return gameWindow.getFocalPoint(); }
                void setFocalPoint(const utl::Point& pt) { gameWindow.setFocalPoint(pt); }
                void moveFocalPoint(const DirectionType d) { gameWindow.moveFocalPoint(d); }

                // Command Line functions
                void setCommandPrompt(const std::string& cmd) { commandLine = cmd; }
                void drawCommandPrompt();

                // Console functions
                void drawConsole() const { console.draw(); }
                void printToConsole(const std::string& str) { console.printStdMessage(str); }
        };
    }
}

#endif
