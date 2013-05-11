#ifndef RLNS_LEVEL_HPP
#define RLNS_LEVEL_HPP

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include "Area.hpp"
#include "CheckedSave.hpp"
#include "CaveBuilder.hpp"
#include "DungeonBuilder.hpp"
#include "Item.hpp"
#include "Map.hpp"
#include "RoomFiller.hpp"
#include "Tile.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Struct      : TileInfo
            Description : Packages information about what to render in a specific tile.
                          This is passed back to the rendering functions to deal with.
        --------------------------------------------------------------------------------*/
        struct TileInfo
        {
            TCODColor bgColor, fgColor;
            int ascii;
        };



        /*--------------------------------------------------------------------------------
            Class       : Level
            Description : Contains a map, items, monsters, etc.  Also has a static vector
                          containing all of the game's levels.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Level
        {
            // Member Variables
            private:
                MapPtr map;
                std::vector<AreaPtr> areas;

                // groups of players or monsters in the level
                std::vector<PartyPtr> parties; 

                // items present
                std::vector<ItemPtr> items;

            // Static Variables
            private:
                static std::vector<LevelPtr> levels;
                static unsigned int currentLevel;

            // Member Functions
            private:
                Level() {}

            public:
                Level(const std::string&);
                Level(utl::RLNSZip&);

                // Map Functions
                int getMapWidth()  const { return map->getWidth(); }
                int getMapHeight() const { return map->getHeight(); }
                TileInfo getTileInfo(const int, const int) const;
                utl::Point getUpStairLocation() const;
                bool moveLegal(const utl::Point&, const MovementType) const;
                int  signalTile(const utl::Point&, const TileActionType);

                // Party Functions
                void addParty(const PartyPtr);
                std::vector<PartyPtr> getParties() const
                { return parties; }

                // Item Functions
                void addItem(const ItemPtr);
                std::vector<ItemPtr> getItems() const
                { return items; }

                std::vector<ItemPtr> fetchItemsAtLocation(const utl::Point&);
                void inspectTileContents(std::string&, const utl::Point&) const;


                void saveToDisk(utl::RLNSZip&) const;

            // Static Functions
            public:
                static void addLevel(const std::string&);
                static LevelPtr getCurrentLevel() { return levels.at(currentLevel); }
                static void gotoNextLevel() { ++currentLevel; }
                static void gotoPreviousLevel() { if(currentLevel > 0) --currentLevel; }
                static void saveLevelsToDisk(utl::RLNSZip&);
                static void loadLevelsFromDisk(utl::RLNSZip&);

        };


        // Inline Functions

        inline utl::Point Level::getUpStairLocation() const
        {
            return map->getUpStairLocation();
        }

        inline void Level::addParty(const PartyPtr party)
        {
            parties.push_back(party);
        }

        inline void Level::addItem(const ItemPtr item)
        {
            items.push_back(item);
        }

        inline bool Level::moveLegal(const utl::Point& pt, const MovementType moveType) const
        {
            return map->moveLegal(pt, moveType);
        }

        inline int Level::signalTile(const utl::Point& pt, const TileActionType signal)
        {
            return map->signalTile(pt, signal);
        }
    }
}

#endif
