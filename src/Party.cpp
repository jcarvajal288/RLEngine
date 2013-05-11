#include "Party.hpp"

namespace rlns
{
    namespace model
    {
        PartyPtr Party::playerParty(new Party());

        /*--------------------------------------------------------------------------------
            Function    : Party::moveLeader
            Description : Moves the party leader one tile in the given direction.
            Inputs      : direction to move
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Party::moveLeader(const DirectionType dir)
        {
            members.at(leader)->shiftPosition(dir);
        }
    }
}
