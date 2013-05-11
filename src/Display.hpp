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
    namespace view
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
                utl::Point focalPt; // where on the map the playfield is focusing (usually the player)

                utl::Point _playfieldTL;
                TCODConsolePtr _playfield;

                utl::Point _partyBarTL;
                TCODConsolePtr _partyBar;

                utl::Point _consoleTL;
                TCODConsolePtr _console;

                // These values are updated before every refresh.
                utl::Point dispTL;      // top left of the display area
                utl::Point dispBR;      // bottom right of the display area
                utl::Point dispCenter;  // center of the display area
                utl::Point camTL;       // top left of what the camera is looking at 
                utl::Point camBR;       // bottom right of what the camera is looking at

            public:
                MessageTrackerPtr messageTracker;


            // Member Functions
            private:
                void drawLevelItems(const model::LevelPtr);
                void drawLevelOccupants(const model::LevelPtr);

            public:
                Display(const InitData&);

                void setFocalPoint(const utl::Point& pt) { focalPt = pt; }
                void shiftFocalPoint(const model::DirectionType);

                void setDisplayValues();

                void drawObject(const model::MapObjectPtr);

                void inspectTile(const model::LevelPtr, const utl::Point&);

                void drawPlayfield();
                void drawPartyBar();
                void drawConsole();

                TCODConsolePtr playfield() { return _playfield; }
                TCODConsolePtr partyBar() { return _partyBar; }
                TCODConsolePtr console() { return _console; }

                utl::Point playfieldTL() { return _playfieldTL; }
                utl::Point partyBarTL() { return _partyBarTL; }
                utl::Point consoleTL() { return _consoleTL; }

                void draw() const;
                void refresh(); 
        };
    }
}

#endif
