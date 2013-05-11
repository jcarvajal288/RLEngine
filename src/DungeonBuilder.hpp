#ifndef RLNS_DUNGEONBUILDER_HPP
#define RLNS_DUNGEONBUILDER_HPP

#include <iostream>

#include "Area.hpp"
#include "MapBuilder.hpp"
#include "Point.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : DungeonBuilder
            Description : Constructs a Dungeon tileset map.  Features large square,
                          circular, and cross-shaped rooms occasionally filled by pillars.
                          Rooms are connected by one tile wide corridors.
            Parents     : MapBuilder
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class DungeonBuilder: public MapBuilder
        {
            // Member Variables
            private:
                /*------------------------------------------------------------------------
                    Class       : DungeonCallback
                    Description : This class is passed to the libtcod BSP traversal 
                                  functions, where its visitNode function will be called
                                  on each node in that tree.
                    Parents     : TCODBspCallback
                    Children    : None
                    Friends     : None
                ------------------------------------------------------------------------*/
                class DungeonCallback: public ITCODBspCallback
                {
                    public:
                        bool visitNode(TCODBsp*, void*);
                };


            // Member Functions
            private:
                void buildRoomType(const TCODBsp&); 
                void createRooms();
                void connectPoints(utl::Point&, const utl::Point&, bool);
                void connectRooms(const AreaPtr, const AreaPtr);
                void setWall(const utl::Point&);
                void checkForDoor(const utl::Point&);

            protected:
                void buildBSPTree();
                void constructAreas();
                void connectAreas();
                void addLights();
                void finishMap();

            public:
                DungeonBuilder(const MapPtr m)
                : MapBuilder(m) {}

        };
    }
}

#endif
