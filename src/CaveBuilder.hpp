#ifndef RLNS_CAVEBUILDER_HPP
#define RLNS_CAVEBUILDER_HPP

#include <iostream>

#include "MapBuilder.hpp"
#include "Point.hpp"
#include "Tile.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Class       : CaveBuilder
        Description : Builds the Cave tileset map.
        Parents     : MapBuilder
        Children    : None
        Friends     : None
    --------------------------------------------------------------------------------*/
    class CaveBuilder: public MapBuilder
    {
        // Member Variables
        private:
            /*------------------------------------------------------------------------
                Class       : CaveCallback
                Description : This class is passed to the libtcod BSP traversal 
                              functions, where its visitNode function will be called
                              on each node in that tree.
                Parents     : TCODBspCallback
                Children    : None
                Friends     : None
            ------------------------------------------------------------------------*/
            class CaveCallback: public ITCODBspCallback
            {
                public:
                    bool visitNode(TCODBsp*, void*);
            };


        // Member Functions
        private:
            void ensureStairsAreReachable();
            void fillRandomly();
            bool isOpenArea(const TCODBsp&);
            void randomizeFloorTiles();
            void replaceBorderWalls();

        protected:
            virtual void buildBSPTree();
            virtual void constructAreas();
            virtual void connectAreas();
            virtual void addLights() {}
            virtual void finishMap();

        public:
            CaveBuilder(const MapPtr m)
            : MapBuilder(m) {}
    };
}

#endif
