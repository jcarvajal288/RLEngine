#include "CaveBuilder.hpp"

using namespace std;

namespace rlns
{
    using namespace utl;

    namespace model
    {
        /*--------------------------------------------------------------------------------
            Function    : CaveBuilder::CaveCallback::visitNode
            Description : callback function for the CaveCallback class.  This function
                          is called during BSP traversals.
            Inputs      : TCODBsp pointer, void pointer (for user supplied data)
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool CaveBuilder::CaveCallback::visitNode(TCODBsp* node, void* userData)
        {
            if(node->isLeaf())
            {
                // convert the void pointer userData to the parent class
                CaveBuilder* parent = static_cast<CaveBuilder*>(userData);
                if(parent->isOpenArea(*node))
                {
                    AreaPtr newArea(new Area(Point(node->x, node->y),
                                             Point(node->w+node->x-1, node->h+node->y-1)));
                    parent->addArea(newArea);
                }
            }
            return true;
        }



        /*--------------------------------------------------------------------------------
            Function    : CaveBuilder::ensureStairsAreReachable
            Description : Checks that a path exists between the up and down stairs.  If
                          one does not, it will dig a striaght line path between them.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void CaveBuilder::ensureStairsAreReachable()
        {
            Point up = map->getUpStairLocation();
            Point down = map->getDownStairLocation();

            if(!isReachable(map, up, down))
            {
                vector<Point> path;
                getCurvyPathBetweenPoints(&path, map, &rand, up, down);
                makePath(map, path, map->tileset->getFloorTileID());
            }
        }



        /*-------------------------------------------------------------------------------- 
            Function    : CaveBuilder::fillRandomly
            Description : Fills the map randomly with floor and ceiling tiles.  This is
                          the first step in the map generation algorithm.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void CaveBuilder::fillRandomly()
        {
            size_t width = map->getWidth();
            size_t height = map->getHeight();

            for(size_t x=0; x<width; ++x)
            {
                for(size_t y=0; y<height; ++y)
                {
                    if(rand.getInt(0,1) == 1)
                    {
                        map->setBottomMostAt(x,y, map->tileset->getFloorTileID());
                    }
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : CaveBuilder::isOpenArea
            Description : Determines if an area defined by the TCODBsp Tree has enough
                          open tiles to be a suitable room.  The number of open tiles
                          required is ten.
            Inputs      : TCODBsp node
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool CaveBuilder::isOpenArea(const TCODBsp& node) 
        {
            int tlx = node.x, tly = node.y;             // topleft corner
            int brx = tlx+node.w-1, bry = tly+node.h-1; // bottomright corner

            int count = 0;

            for(int x=tlx; x<brx; ++x)
            {
                for(int y=tly; y<bry; ++y)
                {
                    if(!Tile::findTile(map->bottomMostAt(x,y))->blocksWalking()) ++count;
                }
            }

            if(count >= 10) return true;
            else return false;
        }



        /*--------------------------------------------------------------------------------
            Function    : CaveBuilder::randomizeFloorTiles
            Description : Iterates through each of the floor tiles and determines randomly
                          which character they will display.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void CaveBuilder::randomizeFloorTiles()
        {
            int numChars = Tile::findTile(map->tileset->getFloorTileID())->getNumChars();

            size_t width = map->getWidth();
            size_t height = map->getHeight();

            for(size_t x=0; x<width; ++x)
            {
                for(size_t y=0; y<height; ++y)
                {
                    if(map->bottomMostAt(x,y) == map->tileset->getFloorTileID())
                    {
                        int offset = rand.getInt(0, numChars-1);
                        map->setBottomMostAt(x, y, map->tileset->getFloorTileID(offset));
                    }
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : CaveBuilder::replaceBorderWalls
            Description : Ensures that the borders of the map are solid walls.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void CaveBuilder::replaceBorderWalls()
        {
            unsigned int width = map->getWidth();
            unsigned int height = map->getHeight();

            // first, the northern and southern borders
            for(unsigned int x=0; x<width; ++x)
            {
                map->setBottomMostAt(x, 0, map->tileset->getWallTileID());
                map->setBottomMostAt(x, height-1, map->tileset->getWallTileID());
            }

            // next, the eastern and western borders
            for(unsigned int y=0; y<height; ++y)
            {
                map->setBottomMostAt(0, y, map->tileset->getWallTileID());
                map->setBottomMostAt(width-1, y, map->tileset->getWallTileID());
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : CaveBuilder::buildBSPTree
            Description : Constructs the TCODBsp tree used for determining level areas.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void CaveBuilder::buildBSPTree()
        {
            // topleft corner of the root node
            int x = 1, y = 1;
            // bottomright corner of the root node              
            // 
            // For some reason, w needs to be set to Width-2, otherwise rooms can run
            // a tile over the eastern edge of the map. h is fine to be set at Height-1.
            int w = map->getWidth()-2, h = map->getHeight()-1; 
            bsp.reset(new TCODBsp(x,y,w,h));
            bsp->splitRecursive(NULL, 6, 15, 15, static_cast<float>(1.1), static_cast<float>(1.1)); // casts to float to avoid VSC++ warnings
        }



        /*--------------------------------------------------------------------------------
            Function    : CaveBuilder::constructAreas
            Description : Randomly fills the map with floor and wall tiles, and then
                          refines the map using the 4-5 rule: a tile becomes a wall if it
                          was a wall and 4 or more of its nine neighbors were walls or if it
                          was not a wall and 5 or more neighbors were.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void CaveBuilder::constructAreas()
        {
            fillRandomly();
            
            int numIterations = 5;
            int wallID = map->tileset->getWallTileID();
            int floorID = map->tileset->getFloorTileID();

            size_t width = map->getWidth()-1;
            size_t height = map->getHeight()-1;

            while(numIterations --> 0)
            {
                for(size_t x=1; x<width; ++x)
                {
                    for(size_t y=1; y<height; ++y)
                    {
                        int count = numNeighbors(map, Point(x,y), wallID);

                        if((map->bottomMostAt(x,y) == wallID && count >= 4)
                        || (map->bottomMostAt(x,y) != wallID && count >= 5)
                        || (map->isBorderTile(x,y)))
                        {
                            map->setBottomMostAt(x,y, wallID);
                        }
                        else
                        {
                            map->setBottomMostAt(x,y, floorID);
                        }
                    }
                }
            }

            // add Areas
            CaveCallback c;
            bsp->traverseInvertedLevelOrder(&c, this);
        }



        /*--------------------------------------------------------------------------------
            Function    : CaveBuilder::connectAreas
            Description : Makes sure that all of the areas on the map are reachable from
                          the starting area.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void CaveBuilder::connectAreas()
        {
            vector<AreaPtr>::iterator it, end;
            it = areas.begin()+1; end = areas.end();

            for(; it!=end; ++it)
            {
                Point origin, dest;
                do { origin = (*(it-1))->getRandomPoint(rand); } 
                while(!map->isWalkable(origin));
                
                do { dest = (*it)->getRandomPoint(rand); } 
                while(!map->isWalkable(dest));

                if(!isReachable(map, origin, dest))
                {
                    vector<Point> path;
                    getCurvyPathBetweenPoints(&path, map, &rand, origin, dest);
                    makePath(map, path, map->tileset->getFloorTileID());
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : CaveBuilder::finishMap
            Description : Adds finishing touches to the cave map.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void CaveBuilder::finishMap()
        {
            placeStairs();
            ensureStairsAreReachable();
            replaceBorderWalls();
            randomizeFloorTiles();
        }
    }
}
