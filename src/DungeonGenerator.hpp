#ifndef RLNS_DUNGEONGENERATOR_HPP
#define RLNS_DUNGEONGENERATOR_HPP

#include <iostream>
#include <vector>

#include "Area.hpp"
#include "MapGenerator.hpp"
#include "Point.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : DungeonGenerator
            Description : Used to generate Dungeons, which are characterized by large
                          square and circular rooms connected by corridors.
            Parents     : MapGenerator
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class DungeonGenerator: public MapGenerator
        {
            // Member Variables
            private:
                /*------------------------------------------------------------------------
                    Class       : DungeonCallBack
                    Description : This class is passed to the BSP traversal functions,
                                  where its visitNode function will be called on each node
                                  in that tree.
                    Parents     : TCODBspCallback
                    Children    : None
                ------------------------------------------------------------------------*/
                class DungeonCallback: public ITCODBspCallback
                {
                    public:
                        bool visitNode(TCODBsp*, void*);
                };


            // Member Functions
            private:
                void addLights();
                void chooseRoomType(const TCODBsp&); 
                void createRooms();
                void connectPoints(utl::Point&, const utl::Point&, bool);
                void connectRooms(const AreaPtr, const AreaPtr);
                void finish();
                void setWall(const utl::Point&);
                void checkForDoor(const utl::Point&);

            public:
                DungeonGenerator(const TilesetPtr t)
                : MapGenerator(t) {}

                virtual MapPtr generate(const bool);
        };
    }
}

#endif
