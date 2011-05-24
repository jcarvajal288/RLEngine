#ifndef RLNS_MAPGENERATOR_HPP
#define RLNS_MAPGENERATOR_HPP

#include <map>
#include <string>

#include "Area.hpp"
#include "Tile.hpp"
#include "Map.hpp"
#include "Point.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : MapGenerator
            Description : Handles generating a Map object for use in a Level.  This may
                          in the future be a base class for a family of MapGenerators,
                          with the type of generator read in from the struct title in
                          tileset.txt
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class MapGenerator
        {
            // Member Variables
            protected:
                MapPtr map;
                static TCODRandom rand; // libtcod's random number generator
                std::vector<AreaPtr> areas;
                
                // libtcod BSP tree and parameters
                TCODBspPtr bsp; // libtcod BSP tree
                const int recursionLevel;
                const int minHSize;
                const int minVSize;
                const float maxHRatio;
                const float maxVRatio;

            // Member Functions
            private:
            // no copying, assignment, or default constructing
                MapGenerator();
                MapGenerator(const MapGenerator&);
                MapGenerator& operator=(const MapGenerator&);

            protected:
            // Map Generation routines
                void createBSPTree();
                int getBorderCode(const utl::Point&, const int);
                //void digUpStairs();
                //void digDownStairs(const int);
                void digStairs(const bool, const int);
                void digStaircases();
                //void addDoors();

            public:
                MapGenerator(const TilesetPtr t)
                : map(new Map(t)),
                  bsp(new TCODBsp(0, 0, t->getMapWidth(), t->getMapHeight())),
                  recursionLevel(t->getRecurseLevel()),
                  minHSize(t->getMinHSize()),
                  minVSize(t->getMinVSize()),
                  maxHRatio(t->getMaxHRatio()),
                  maxVRatio(t->getMaxVRatio()) {}

                virtual MapPtr generate(const bool) = 0;
        };
    }
}

#endif
