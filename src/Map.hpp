#ifndef RLNS_MAP_HPP
#define RLNS_MAP_HPP

#include <algorithm>
#include <iostream>
#include <vector>

#include "AbstractTile.hpp"
#include "CheckedSave.hpp"
#include "Tileset.hpp"

namespace rlns
{
    typedef std::vector< std::vector< std::vector<int> > > IntVector3D;

    /*--------------------------------------------------------------------------------
        Class       : Map
        Description : Contains all the data needed for a level map.  Includes the tile
                      map, pathing map, and lighting map.
        Parents     : None
        Children    : None
        Friends     : None
    --------------------------------------------------------------------------------*/
    class Map
    {
        // Member Variables
        public:
            TilesetPtr tileset;

            // This TCODImage will have twice the width and height of the
            // tileMap in order to have four data entries per tile. This
            // allows the map to be shaded using subpixels.
            TCODImage lightMap;

            // This map is the same width and height of the tile map and is
            // used for pathfinding and field of view.
            TCODMap pathingMap;

        private:
            // two dimensions for width and height, and the third dimension
            // for stacking tiles on top of each other, such as a door tile
            // sitting on top of a floor tile
            IntVector3D tileMap;
            Point upStairLocation, downStairLocation;

        // Member Functions
        private:
            void updateTileCoordinate(const int, const int);

        public:
            Map(const TilesetPtr);
            Map(RLNSZip&);

            TilesetPtr getTileset() const 
            { return tileset; }

            size_t getWidth() const 
            { return tileMap.size(); }

            size_t getHeight() const
            { return tileMap.at(0).size(); }

            Point getUpStairLocation() const
            { return upStairLocation; }
            void setUpStairLocation(const Point& up)
            { upStairLocation = up; }

            Point getDownStairLocation() const
            { return downStairLocation; }
            void setDownStairLocation(const Point& down)
            { downStairLocation = down; }

            std::vector<int> at(const int, const int) const;
            std::vector<int> at(const Point&) const;

            // Returns the tile ID at the end of the vector at
            // the given coordinates. This is the most visible
            // tile at those coordinates.
            int topMostAt(const int, const int) const;
            int topMostAt(const Point&) const;

            // Returns the tile ID at the beginning of the vector
            // at the given coordinates.  This is the least visible
            // tile at those coordinates.
            int bottomMostAt(const int, const int) const;
            int bottomMostAt(const Point&) const;
            void setBottomMostAt(const int, const int, const int);
            void setBottomMostAt(const Point&, const int);

            void addFeature(const int, const int, const int);
            void addFeature(const Point&, const int);

            bool isBorderTile(const unsigned int, const unsigned int) const;
            bool isInBoard(const int, const int) const;
            bool isInBoard(const Point&) const;

            bool isWalkable(const int, const int) const;
            bool isWalkable(const Point&) const;

            bool moveLegal(const Point&, const MovementType) const;

            bool signalTile(const Point&, const TileActionType);

            void listTileFeatures(std::vector<AbstractTilePtr>&, const Point&) const;

            void saveToDisk(RLNSZip&) const;
    };


    // Inline Functions

    inline std::vector<int> Map::at(const Point& pt) const
    {
        return at(pt.X(), pt.Y());
    }

    inline int Map::topMostAt(const Point& pt) const
    {
        return topMostAt(pt.X(), pt.Y());
    }

    inline int Map::bottomMostAt(const Point& pt) const
    {
        return bottomMostAt(pt.X(), pt.Y());
    }

    inline void Map::setBottomMostAt(const Point& pt, const int id) 
    {
        setBottomMostAt(pt.X(), pt.Y(), id);
    }

    inline void Map::addFeature(const Point& pt, const int id)
    {
        addFeature(pt.X(), pt.Y(), id);
    }

    inline bool Map::isBorderTile(const unsigned int x, const unsigned int y) const
    {
        return (x == 0 || x == getWidth()-1) || (y == 0 || y == getHeight()-1);
    }

    // Checks if a given point is within the map boundaries
    inline bool Map::isInBoard(const int x, const int y) const
    {
        if((x < 0) || (x >= static_cast<int>(getWidth())-1)
        || (y < 0) || (y >= static_cast<int>(getHeight())-1))
        {
            return false;
        }
        return true;
    }

    inline bool Map::isInBoard(const Point& pt) const
    {
        return isInBoard(pt.X(), pt.Y());
    }

    inline bool Map::isWalkable(const Point& pt) const
    {
        return isWalkable(pt.X(), pt.Y());
    }


    // Non-member Functions

    bool hasDoorAdjacentTo(const MapPtr, const Point&);
}

#endif
