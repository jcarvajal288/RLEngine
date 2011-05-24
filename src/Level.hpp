#ifndef RLE_LEVEL_HPP
#define RLE_LEVEL_HPP

#include <iostream>
#include <map>
#include <string>

#include "Actor.hpp"
#include "CheckedSave.hpp"
#include "Display.hpp"
#include "DungeonGenerator.hpp"
#include "Map.hpp"
#include "Tile.hpp"
#include "Tileset.hpp"
#include "Types.hpp"
#include "Utility.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : Level
            Description : Holds all the information that goes into a level, such as the
                          map, monsters, and items.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Level
        {
            // Member Variables
            private:
                MapPtr map;
                //std::vector<FeaturePtr> features;
                // creatures, features, etc.

            // Member Functions
            private:
                void addLights();
                void generateMap(const std::string&, const bool);

            public:
                Level(const std::string& tileset, const bool descending)
                {
                    generateMap(tileset, descending);
                }

                Level(utl::RLNSZip&); // used for save game loading

                int getWidth() const { return map->getWidth(); }
                int getHeight() const { return map->getHeight(); }

                bool isWalkable(const utl::Point&) const;
                bool isWalkable(const int, const int) const;

                bool isTransparent(const utl::Point&) const;
                bool isTransparent(const int, const int) const;

                std::vector<TilePtr> getTiles(const int x, const int y) const { return map->getTiles(x,y); }
                std::vector<TilePtr> getTiles(const utl::Point& pt) const { return getTiles(pt.getX(), pt.getY()); }

                TCODColor getTileLightInfo(const utl::Point& p) const { return getTileLightInfo(p.getX(), p.getY()); }
                TCODColor getTileLightInfo(const int x, const int y) const { return map->getTileLightInfo(x,y); }

                void updateTileLightInfo(const int x, const int y, const TCODColor& l)  { map->updateTileLightInfo(x, y, l); }
                void updateTileLightInfo(const utl::Point& p, const TCODColor& l)       { map->updateTileLightInfo(p.getX(), p.getY(), l); }

                void examineTile(const utl::Point& pt) const { map->examineTile(pt); }

                std::vector<utl::Point> getUpStairs()   const { return map->getUpStairs(); }
                std::vector<utl::Point> getDownStairs() const { return map->getDownStairs(); }
                std::vector<utl::Point> getStairs();

                bool checkForTileChar(const utl::Point&, const char) const;

                void calculateFOV(const utl::Point& p, const int r) { map->calculateFOV(p,r); }
                void calculatePartyFOV(std::vector< std::vector<bool> >&, const std::vector<utl::Point>&);
                bool isInFOV(const int x, const int y) const { return map->isInFOV(x,y); }

                void drawTile(const int a, const int b, const int x, const int y) const { map->drawTile(a,b,x,y); }

                std::vector<int> getLights() const;
                void addLight(const utl::Point&, const int);
                //void removeLight(const utl::Point&);
                void clearLightMap() { map->clearLightMap(); }

                bool signalTile(const utl::Point& pt, const TileActionType t) { return map->signalTile(pt, t); }

                void saveToDisk(utl::RLNSZip&) const;
        };


        // inline functions

        inline bool Level::isWalkable(const utl::Point& pt) const
        {
            return isWalkable(pt.getX(), pt.getY());
        }
        inline bool Level::isWalkable(const int x, const int y) const
        {
            return map->isWalkable(x,y);
        }

        inline bool Level::isTransparent(const utl::Point& pt) const
        {
            return isTransparent(pt.getX(), pt.getY());
        }
        inline bool Level::isTransparent(const int x, const int y) const
        {
            return map->isTransparent(x,y);
        }

        inline void Level::calculatePartyFOV(std::vector< std::vector<bool> >& partyFOV, const std::vector<utl::Point>& pos) 
        { 
            map->calculatePartyFOV(partyFOV, pos); 
        }

        inline void Level::addLight(const utl::Point& pt, const int lightID)
        {
            map->addLight(lightID, pt);
        }
    }
}

#endif
