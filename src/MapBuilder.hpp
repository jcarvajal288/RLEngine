#ifndef RLNS_MAPBUILDER_HPP
#define RLNS_MAPBUILDER_HPP

#include <algorithm>
#include <stdexcept>
#include <vector>

#include "Area.hpp"
#include "Map.hpp"
#include "Point.hpp"
#include "Types.hpp"
#include "Utility.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : MapBuilder
            Description : Abstract class that builds the various map types in the game.
            Parents     : None
            Children    : DungeonBuilder, CaveBuilder
            Friends     : None
        --------------------------------------------------------------------------------*/
        class MapBuilder
        {
            // Member Variables
            protected:
                TCODBspPtr bsp; // libtcod BSP tree
                TCODRandom rand;
                MapPtr map;
                std::vector<AreaPtr> areas;

                //const int recursionLevel;
                //const int minHSize;
                //const int minVSize;
                //const float maxHRatio;
                //const float maxVRatio;

            // Member Functions
            protected:
                MapBuilder(const MapPtr m) 
                : map(m) {}

                void placeStairs();

                virtual void buildBSPTree() {}
                virtual void constructAreas() {}
                virtual void connectAreas() {}
                virtual void addLights() {}
                virtual void finishMap() {}

            public:
                virtual ~MapBuilder() {}

                void addArea(const AreaPtr a)
                { areas.push_back(a); }
                std::vector<AreaPtr> getAreas() const
                { return areas; }

                void buildMap();
        };


        // Non-member Functions

        int getBorderCode(const MapPtr, const utl::Point&, const int);
        int getBorderCode(const MapPtr, const utl::Point&, const std::vector<int>&);

        int numNeighbors(const MapPtr, const utl::Point&, const int);

        bool isReachable(const MapPtr, const utl::Point&, const utl::Point&);

        void makePath(const MapPtr, const std::vector<utl::Point>&, const int);

        void getCurvyPathBetweenPoints(std::vector<utl::Point>*, const MapPtr, TCODRandom*, const utl::Point&, const utl::Point&);
    }
}

#endif 
