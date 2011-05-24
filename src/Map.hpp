#ifndef RLE_MAP_HPP
#define RLE_MAP_HPP

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "CheckedSave.hpp"
#include "Display.hpp"
#include "Light.hpp"
#include "Point.hpp"
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
            Class       : Map
            Description : Holds the array of tiles and other information that goes into a
                          level map.
            Members     : tileset - the tileset that this map follows.
                          tileMap - a 3D array of indices into the Tile list.  At
                          first, only 1 tile will be in each coordinate, but as features
                          are added, each coordinate will have several terrain indices.
                          lightMap - tracks the light level in each coordinate
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Map
        {
            // Member Variables
            private:
                TilesetPtr tileset;
                std::vector< std::vector< std::vector<int> > > tileMap; // 3D array of indices into the Tile list
                std::vector<int> lights; // tracks light sources by tracking the IDs of lights in the level
                std::vector< std::vector<TCODColor> > lightMap; 
                //std::vector<AreaPtr> areas;
                std::vector<utl::Point> upStairs;
                std::vector<utl::Point> downStairs;

                TCODMapPtr fovMap; // tracks field of view

            // Member Functions
            public:
                Map(const TilesetPtr t)
                : tileset(t),
                  tileMap(t->getMapWidth(), 
                          std::vector< std::vector<int> >(t->getMapHeight(), 
                                                          std::vector<int>(1, t->getFillerTileID()))),
                  lightMap(t->getMapWidth(), 
                           std::vector<TCODColor>(t->getMapHeight(), 
                                                  TCODColor::white * tileset->getAmbientLight())),
                  fovMap(new TCODMap(t->getMapWidth(), t->getMapHeight())) {}

                Map(utl::RLNSZip&); // used for loading save games

                int getWidth()  const  { return tileMap.size(); }
                int getHeight() const  { return tileMap.at(0).size(); }

                TilesetPtr getTileset() const { return tileset; }

                int getFloorTileID(const int i=0)  const { return tileset->getFloorTileID(i); }
                int getWallTileID(const int i=0)   const { return tileset->getWallTileID(i); }
                int getFillerTileID(const int i=0) const { return tileset->getFillerTileID(i); }

                std::vector<TilePtr> getTiles(const int, const int) const;
                std::vector<TilePtr> getTiles(const utl::Point&) const;
                int getTileID(const int, const int) const;
                int getTileID(const utl::Point&) const;
                void setTileID(const int, const int, const int);
                void setTileID(const utl::Point&, const int);

                void addLight(const int, const utl::Point&);
                std::vector<int> getLights() const { return lights; }

                bool signalTile(const utl::Point&, const TileActionType);
                void examineTile(const utl::Point&) const;

                TCODColor getTileLightInfo(const int, const int) const;
                TCODColor getTileLightInfo(const utl::Point&) const;
                void updateTileLightInfo(const int, const int, const TCODColor&);
                void updateTileLightInfo(const utl::Point&, const TCODColor&);
                void clearLightMap();

                std::vector<utl::Point> getUpStairs() const { return upStairs; }
                std::vector<utl::Point> getDownStairs() const { return downStairs; }
                void setUpStairs(const std::vector<utl::Point>& ups) { upStairs = ups; }
                void setDownStairs(const std::vector<utl::Point>& downs) { downStairs = downs; }

                void lightLights();

                void updateFOVMap(const int x, const int y);
                void calculateFOV(const utl::Point&, const int);
                void calculatePartyFOV(std::vector< std::vector<bool> >&, const std::vector<utl::Point>&);
                bool isInFOV(const int x, const int y) const { return fovMap->isInFov(x,y); }

                bool isWalkable(const utl::Point&) const;
                bool isWalkable(const int, const int) const;

                bool isTransparent(const utl::Point&) const;
                bool isTransparent(const int, const int) const;

                void drawTile(const int, const int, const int, const int) const;

                void addFeature(const int, const int, const int);
                void addFeature(const utl::Point&, const int);

                void saveToDisk(utl::RLNSZip&) const;
                void loadFromDisk(utl::RLNSZip&);
        };

        // inline functions
        inline int Map::getTileID(const int x, const int y) const
        {
            return tileMap.at(x).at(y).at(0);
        }
        inline int Map::getTileID(const utl::Point& p) const
        {
            return getTileID(p.getX(), p.getY());
        }
        inline std::vector<TilePtr> Map::getTiles(const utl::Point& p) const
        {
            return getTiles(p.getX(), p.getY());
        }

        inline void Map::setTileID(const int x, const int y, const int a)
        {
            tileMap.at(x).at(y).at(0) = a;

            // update fovMap's transparency value at this point to reflect the new terrain
            TilePtr tile(Tile::findTile(a));
            fovMap->setProperties(x,y, !tile->blocksLight(), !tile->blocksWalking());
        }
        inline void Map::setTileID(const utl::Point& p, const int a)
        {
            setTileID(p.getX(), p.getY(), a);
        }

        inline void Map::addLight(const int lightID, const utl::Point& pt)
        {
            Light::moveLight(lightID, pt);
            lights.push_back(lightID);
        }

        inline TCODColor Map::getTileLightInfo(const int x, const int y) const
        {
            return lightMap.at(x).at(y);
        }
        inline TCODColor Map::getTileLightInfo(const utl::Point& p) const
        {
            return getTileLightInfo(p.getX(), p.getY());
        }

        inline void Map::updateTileLightInfo(const int x, const int y, const TCODColor& newColor)
        {
            TCODColor oldColor(lightMap.at(x).at(y));
            TCODColor result(std::max(oldColor.r, newColor.r), 
                             std::max(oldColor.g, newColor.g), 
                             std::max(oldColor.b, newColor.b));
            lightMap.at(x).at(y) = result;
        }
        inline void Map::updateTileLightInfo(const utl::Point& p, const TCODColor& a)
        {
            updateTileLightInfo(p.getX(), p.getY(), a);
        }


        inline bool Map::isWalkable(const utl::Point& pt) const 
        { 
            return isWalkable(pt.getX(), pt.getY()); 
        }
        inline bool Map::isWalkable(const int x, const int y) const 
        { 
            if(x >= 0 && x < getWidth() && y >= 0 && y < getHeight())
                return fovMap->isWalkable(x,y); 
            else
                return false;
        }

        inline bool Map::isTransparent(const utl::Point& pt) const 
        { 
            return isTransparent(pt.getX(), pt.getY()); 
        }
        inline bool Map::isTransparent(const int x, const int y) const 
        { 
            if(x >= 0 && x < getWidth() && y >= 0 && y < getHeight())
                return fovMap->isTransparent(x,y); 
            else
                return false;
        }

        inline void Map::addFeature(const int x, const int y, const int fid)
        {
            TilePtr tile(Tile::findTile(fid));
            tileMap.at(x).at(y).push_back(fid);

            // update fovMap to account for the new feature
            fovMap->setProperties(x,y, !tile->blocksLight(), !tile->blocksWalking());
        }
        inline void Map::addFeature(const utl::Point& pt, const int fid)
        {
            addFeature(pt.getX(), pt.getY(), fid);
        }
    }
}

#endif
