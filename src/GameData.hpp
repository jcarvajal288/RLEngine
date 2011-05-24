#ifndef RLNS_GAMEDATA_HPP
#define RLNS_GAMEDATA_HPP

#include <iostream>
#include <vector>

#include "Actor.hpp"
#include "CheckedSave.hpp"
#include "Display.hpp"
#include "Level.hpp"
#include "LevelTree.hpp"
#include "Light.hpp"
#include "Point.hpp"
#include "Types.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : GameData
            Description : Object containing all of the game data.  This overarching class
                          coordinates all of the levels, characters, and other objects in 
                          the game and is reponsible for managing saving and loading.
                          Uses the Singleton design.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class GameData
        {
            // Member Variables
            private:
                static GameDataPtr _instance;

                std::vector<ActorPtr> party; // tracks party members
                int currentPlayer;
                LevelTreePtr levelTree;

            // Member Functions
            private:
                GameData(): currentPlayer(0) {}

            public:
                static GameDataPtr get();

                LevelPtr getCurrentLevel() const { return levelTree->getCurrentLevel(); }
                ActorPtr getActivePlayer() const { return party.at(currentPlayer); }
                std::vector<model::ActorPtr> getParty() const { return party; }
                std::vector<utl::Point> getPartyPositions() const;
                std::vector<int> getPartyLights() const;

                void createNewGameState();
                void useStairs(const utl::Point&, const bool);

                void moveActivePlayer(const DirectionType&);
                void movePartyToPoint(const utl::Point& pt);

                void calculatePartyFOV(std::vector< std::vector<bool> >& map);

                std::vector<int> getLights() const;

                void saveToDisk(utl::RLNSZip&) const;
                void loadFromDisk(utl::RLNSZip&);
        };
    }
}

#endif
