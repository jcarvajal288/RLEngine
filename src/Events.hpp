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
    // Event Functions
    //std::string getTextInput(const DisplayPtr);
    //bool movePlayer(const model::DirectionType, const DisplayPtr);
    //bool pickUpItem(const DisplayPtr);
    //bool showInventory(const DisplayPtr);


    // Event Context Functions
    bool mainEventContext(const EventType, const DisplayPtr);
}

#endif
