#ifndef RLNS_DISPLAY_HPP
#define RLNS_DISPLAY_HPP

#include <iostream>
#include <stdexcept>

#include "Actor.hpp"
#include "InitData.hpp"
#include "Level.hpp"
#include "MessageTracker.hpp"
#include "Party.hpp"
#include "Point.hpp"
#include "Types.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Class       : Display
        Description : Manages and draws the various parts of the game screen. Uses the
                      Singleton design pattern.
        Parents     : None
        Children    : None
        Friends     : None
    --------------------------------------------------------------------------------*/
    class Display
    {
        // Member Variables
        private:
            Point focalPt; // where on the map the playfield is focusing (usually the player)

            Point _playfieldTL;
            TCODConsolePtr _playfield;

            Point _partyBarTL;
            TCODConsolePtr _partyBar;

            Point _consoleTL;
            TCODConsolePtr _console;

            // These values are updated before every refresh.
            Point dispTL;      // top left of the display area
            Point dispBR;      // bottom right of the display area
            Point dispCenter;  // center of the display area
            Point camTL;       // top left of what the camera is looking at 
            Point camBR;       // bottom right of what the camera is looking at

        public:
            MessageTrackerPtr messageTracker;


        // Member Functions
        private:
            void drawLevelItems(const LevelPtr);
            void drawLevelOccupants(const LevelPtr);

        public:
            Display(const InitData&);

            void setFocalPoint(const Point& pt) { focalPt = pt; }
            void shiftFocalPoint(const DirectionType);

            void setDisplayValues();

            void drawObject(const MapObjectPtr);

            void inspectTile(const LevelPtr, const Point&);

            void drawPlayfield();
            void drawPartyBar();
            void drawConsole();

            TCODConsolePtr playfield() { return _playfield; }
            TCODConsolePtr partyBar() { return _partyBar; }
            TCODConsolePtr console() { return _console; }

            Point playfieldTL() { return _playfieldTL; }
            Point partyBarTL() { return _partyBarTL; }
            Point consoleTL() { return _consoleTL; }

            void draw() const;
            void refresh(); 
    };
}

#endif
