#ifndef RLNS_GAMEWINDOW_HPP
#define RLNS_GAMEWINDOW_HPP

#include <utility>

#include "DisplayWindow.hpp"
#include "Point.hpp"
#include "Tile.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace view
    {
        /*--------------------------------------------------------------------------------
            Class       : GameWindow
            Description : Manages the game play area in the main Display Window
            Parents     : DisplayWindow
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class GameWindow: public DisplayWindow
        {
            // Member Variables
            private:
                utl::Point focalPoint;

            // Member Functions
            public:
                GameWindow(const utl::Point& tl, const utl::Point& br)
                : DisplayWindow(tl, br), focalPoint(0,0) {} 

                utl::Point getFocalPoint() const { return focalPoint; }
                void setFocalPoint(const utl::Point& p) { focalPoint = p; }
                void moveFocalPoint(const DirectionType& d) { focalPoint.shift(d); }

                //void update() const;
        };
    }
}

#endif
