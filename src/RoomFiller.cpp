#include "RoomFiller.hpp"

using namespace rlns::utl;

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Function    : RoomFiller::genItem
            Description : Generates an item at a point in the given area.
            Inputs      : area of the map in which to place an item.
            Outputs     : None
            Return      : ItemPtr
        --------------------------------------------------------------------------------*/
        ItemPtr RoomFiller::genItem(const AreaPtr area) const
        {
            // find an empty point in the room
            Point pt;
            TCODRandom rand;
            do { pt = area->getRandomPoint(rand); }
            while(!map->moveLegal(pt, MovementType::WALKING));

            // add an item
            ItemPtr newItem(new Item(pt));
            return newItem;
        }



        /*--------------------------------------------------------------------------------
            Function    : RoomFiller::genMonsterGroup 
            Description : Adds a group of monsters to the specified area of the map.
            Inputs      : AreaPtr
            Outputs     : None
            Return      : PartyPtr
        --------------------------------------------------------------------------------*/
		PartyPtr RoomFiller::genMonsterGroup(const AreaPtr area) const		
		{
			
		}
    }
}
