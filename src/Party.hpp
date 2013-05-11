#ifndef RLNS_PARTY_HPP
#define RLNS_PARTY_HPP

#include "Actor.hpp"

#include <vector>

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : Party
            Description : Groups Actors together into a unit that fights together.  The 
                          player's party is one of these, but monsters are also grouped 
                          and spawned as parties.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Party
        {
            // Member Variables
            private:
                std::vector<ActorPtr> members;

                // If this is the player's party, the leader is the member that the player
                // is currently controlling.  If it's a monster party, this is the member
                // that all the other members follow.
                size_t leader;

            // Static Members
            private:
                static PartyPtr playerParty;

            // Member Functions
            public:
                Party(): leader(0) {}
                Party(std::vector<ActorPtr> m)
                : members(m), leader(0) {}

                ActorPtr getLeader() const { return members.at(leader); }
                void setLeader(const size_t l) { leader = l; }

                void addMember(const ActorPtr);
                std::vector<ActorPtr> getMembers() const;

                void moveLeader(const DirectionType);

            // Static Functions
                static PartyPtr getPlayerParty() { return playerParty; }
        };


        // Inline Functions

        inline void Party::addMember(const ActorPtr actor)
        {
            members.push_back(actor);
        }

        inline std::vector<ActorPtr> Party::getMembers() const
        {
            return members;
        }
    }
}

#endif
