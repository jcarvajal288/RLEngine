#ifndef RLNS_HPP
#define RLNS_HPP

// disabled warning rationale
//
// #pragma warning( disable : 4482 )
//
// C4482 warns when I fully specify enum names, such as DirectionType::NORTH
// It seems to prefer shortened names, so the above would be just NORTH.  I like
// fully specified names, as it lets the reader know exactly what NORTH is part of.

#include <iostream>

#include "Actor.hpp"
#include "Display.hpp"
#include "Events.hpp"
#include "EventHandler.hpp"
#include "InitData.hpp"
#include "Party.hpp"
#include "Tile.hpp"
#include "Tileset.hpp"
#include "Types.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Class       : LCRL
        Description : Main class for the game.  Initializes game data, runs the main
                      game loop, and cleans up game data on exit.  
        Parents     : None
        Children    : None
        Friends     : None
    --------------------------------------------------------------------------------*/
    class LCRL
    {
        // Member Variables
        private:
            InitData initData;
            DisplayPtr display;

        // Member Functions
        private:
            bool initialize();
            void gameLoop();
            void render(const DisplayPtr) const;
            void cleanup();

        public:
            LCRL() {}

            int run();
    };
}

#endif
