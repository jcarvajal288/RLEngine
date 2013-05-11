#ifndef RLNS_ROOMFILLER_HPP
#define RLNS_ROOMFILLER_HPP

// see warning disable rationale in lcrl.hpp
#ifdef _WIN32
#pragma warning( disable : 4482 )
#endif

#include "Area.hpp"
#include "Item.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : RoomFiller
            Description : Determines what items and monsters will be in a particular
                          dungeon room.  The RoomFiller takes a map object and a depth
                          integer as parameters.  The map object lets the RoomFiller know
                          where it can legally place objects, while the depth integer lets
                          it know what kind of items and monsters it should place.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class RoomFiller
        {
            // Member Variables
            private:
                MapPtr map;
                unsigned int depth;

            // Member Functions
            public:
                RoomFiller(const MapPtr m, const unsigned int d) 
                : map(m), depth(d) {}

                ItemPtr genItem(const AreaPtr) const;
				PartyPtr genMonsterGroup(const AreaPtr) const;
        };
    }
}

#endif
