#ifndef RLNS_ITEM_HPP
#define RLNS_ITEM_HPP

#include <string>

#include "MapObject.hpp"
#include "Point.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : Item
            Description : Base class for all of the various items in the game.  Defines
                          all of the common item functionality.
            Parents     : MapObject
            Children    : Weapon, Armor, etc.
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Item: public MapObject
        {
            // Member Variables
            private:
                std::string name;
                unsigned int count;

            // Member Functions
            public:
                // temporary constructor
                Item(const utl::Point& pos)
                : MapObject('*', TCODColor::white, TCODColor::fuchsia,
                               "a macguffin", "a valuable macguffin", pos),
                  name("Macguffin"), count(1) {}

                std::string getName() const 
                { return name; }

                unsigned int getCount() const
                { return count; }
        };
    }
}

#endif
