#ifndef RLNS_EVENT_HANDLER_HPP
#define RLNS_EVENT_HANDLER_HPP

#include <iostream>

#include "Display.hpp"
#include "GameData.hpp"
#include "Renderer.hpp"
#include "Point.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace control
    {
        /*--------------------------------------------------------------------------------
            Class       : EventHandler
            Description : Manages all the interaction between the game and the user.
                          Interprets keystrokes and (possibly) mouse events.  Uses the
                          Singleton design pattern.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class EventHandler
        {
            // Member Variables
            private:
                static EventHandlerPtr _instance;

            // Member Functions
            private:
                EventType useStairs(const char) const;
                EventType movePlayer(const DirectionType) const;
                EventType openCommand() const;

                DirectionType promptDirection() const;
                DirectionType getDirection(const TCOD_key_t) const;

                TCOD_key_t getKeypress() const;
                EventType getPlayerInput() const;
                EventType systemCommands(const TCOD_key_t) const;
                EventType letterCommands(const char) const;

            public:
                void gameLoop() const;

                static EventHandlerPtr get();
        };
    }
}

#endif
