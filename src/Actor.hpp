#ifndef RLNS_ACTOR_HPP
#define RLNS_ACTOR_HPP

// see warning disable rationale in lcrl.hpp
#ifdef _WIN32
    #pragma warning( disable : 4482 )
#endif

#include "Inventory.hpp"
#include "MapObject.hpp"
#include "Point.hpp"
#include "Types.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Class       : Actor
        Description : Represents players, creatures, etc.  Essentially anything that
                      has stats and can move.
        Parents     : MapObject
        Children    : None
        Friends     : InventoryScreen
    --------------------------------------------------------------------------------*/
    class Actor: public MapObject
    {
        // Member Variables
        private:
            Inventory inventory;

        // Member Functions
        public:
            Actor(const Point&, const int, const TCODColor&);

            virtual ~Actor() {}

            void giveItem(const ItemPtr);

            void shiftPosition(const DirectionType); 

            MovementType getMovementType() const 
            { return MovementType::WALKING; }

            friend class InventoryScreen;
    };


    // Inline Functions

    inline void Actor::shiftPosition(const DirectionType dir)
    {
        Point pt = getPosition();
        pt.shift(dir);
        setPosition(pt);
    }
}

#endif
