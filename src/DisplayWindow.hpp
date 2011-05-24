#ifndef RLNS_DISPLAYWINDOW_HPP
#define RLNS_DISPLAYWINDOW_HPP

#include <iostream>

#include "Point.hpp"

namespace rlns
{
    namespace view
    {
        /*--------------------------------------------------------------------------------
            Class       : DisplayWindow
            Description : A partition of the root libtcod window used for a specific
                          purpose.  This abstract class protects the positional data
                          contained by each of its children.
            Parents     : None
            Children    : GameWindow, Console, PartyWindow
            Friends     : None
        --------------------------------------------------------------------------------*/
        class DisplayWindow
        {
            // Member Variables
            private:
                const utl::Point tl;
                const utl::Point br;

            // Member Functions
            public:
                DisplayWindow(const utl::Point& _tl, const utl::Point& _br)
                : tl(_tl), br(_br) {}

                utl::Point topLeft()     const { return tl; }
                utl::Point bottomRight() const { return br; }
        };
    }
}

#endif
