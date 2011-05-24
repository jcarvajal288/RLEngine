#ifndef RLNS_AREA_HPP
#define RLNS_AREA_HPP

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

#include "Map.hpp"
#include "Point.hpp"
#include "Utility.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : Area
            Description : Manages a specific area in a level where monsters and treasure
                          can spawn.  Contains information about the area's dimensions and 
                          location in the level.  If inside, areas may represent rooms or 
                          stretches of corridors.  If outside, an area could be an 
                          arbitrary stretch of land.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Area
        {
            // Member Variables
            private:
                utl::Point topLeft;
                utl::Point bottomRight;
                utl::Point dimensions;

            // Member Functions
            public:
                Area() {}

                Area(const utl::Point& tl, const utl::Point& br)
                : topLeft(tl), bottomRight(br), dimensions(br-tl) {}

                //Area(utl::RLNSZip& zip) // used for loading saved games
                //: topLeft(zip), bottomRight(zip), dimensions(bottomRight - topLeft) {}

                //void saveToDisk(RLNSZip&) const;
                //void loadFromDisk(RLNSZip&);

                utl::Point getTL()  const { return topLeft; }
                utl::Point getTR()  const { return utl::Point(bottomRight.getX(), topLeft.getY()); }
                utl::Point getBL()  const { return utl::Point(topLeft.getX(), bottomRight.getY()); }
                utl::Point getBR()  const { return bottomRight; }
                utl::Point getDim() const { return dimensions; }

                utl::Point getRandomPoint(TCODRandom&) const;
                std::pair<Area, DirectionType> getPossibleCorridorTo(const Area&) const;
        };

        // Area creation helper functions
        void addColumnsToSquareRoom(MapPtr, TCODRandom&, const Area&);

        // Area creation functions (Halls fill the whole node, rooms take a subset)
        Area squareHall(MapPtr, const TCODBsp&);
        Area squareRoom(MapPtr, const TCODBsp&, TCODRandom&, const int);
        Area crossHall(MapPtr, const TCODBsp&, TCODRandom&, const int);
        Area circularHall(MapPtr, const TCODBsp&);
    }
}

#endif
