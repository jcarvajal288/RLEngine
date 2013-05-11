#ifndef RLNS_EVENTS_HPP
#define RLNS_EVENTS_HPP

#include <iostream>
#include <string>

#include "Display.hpp"
#include "Level.hpp"
#include "MenuScreen.hpp"
#include "Party.hpp"
#include "Point.hpp"
#include "Types.hpp"
#include "Utility.hpp"

namespace rlns
{
    namespace control
    {
        // Event Functions
        //std::string getTextInput(const view::DisplayPtr);
        //bool movePlayer(const model::DirectionType, const view::DisplayPtr);
        //bool pickUpItem(const view::DisplayPtr);
        //bool showInventory(const view::DisplayPtr);


        // Event Context Functions
        bool mainEventContext(const EventType, const view::DisplayPtr);
    }
}

#endif
