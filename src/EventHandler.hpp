#ifndef RLNS_EVENT_HANDLER_HPP
#define RLNS_EVENT_HANDLER_HPP

// see warning disable rationale in lcrl.hpp
#ifdef _WIN32
    #pragma warning( disable : 4482 )
#endif

#include <iostream>
#include <vector>

#include "Point.hpp"
#include "Types.hpp"
#include "Utility.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace control
    {
        /*--------------------------------------------------------------------------------
            Class       : EventHandler
            Description : Manages all the interaction between the game and the user.
                          Interprets keystrokes and (possibly) mouse events.  
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class EventHandler
        {
            // Member Functions
            private:
                EventType promptDirection() const;
                EventType getDirection(const TCOD_key_t) const;

                EventType systemCommands(const TCOD_key_t) const;
                EventType letterCommands(const TCOD_key_t) const;

            public:
                EventHandler() {}

                EventType keyToEvent(const TCOD_key_t) const;

                EventType getPlayerInput() const;
        };
    }
}

#endif
