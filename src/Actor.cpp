#include "Actor.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : Actor::Actor
        Description : Default constructor for the Actor class.  
        Inputs      : None
        Outputs     : None
        Return      : None (constructor)
    --------------------------------------------------------------------------------*/
    Actor::Actor(const Point& pos, const int ch, const TCODColor& col)
    : MapObject(ch, col, TCODColor::fuchsia, "","", pos) {}



    /*--------------------------------------------------------------------------------
        Function    : Actor::giveItem
        Description : Adds an item to the Actor's inventory.
        Inputs      : ItemPtr
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Actor::giveItem(const ItemPtr item)
    {
        inventory.addItem(item);
    }
}
