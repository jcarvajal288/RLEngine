#include "Area.hpp"

using namespace std;
using namespace rlns::utl;

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Function    : Area::saveToDisk
            Description : saves the Area to the given save buffer
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------/
        void Area::saveToDisk(RLNSZip& zip) const
        {
            topLeft.saveToDisk(zip);
            bottomRight.saveToDisk(zip);
        }*/



        /*--------------------------------------------------------------------------------
            Function    : Area:loadFromDisk
            Description : loads a Area from the given save buffer
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------/
        void Area::loadFromDisk(RLNSZip& zip)
        {
            topLeft.loadFromDisk(zip);
            bottomRight.loadFromDisk(zip);
            dimensions.loadFromDisk(zip);
        }*/



        /*--------------------------------------------------------------------------------
            Function    : Area::getRandomPoint
            Description : returns a random Point contained in the room.
            Inputs      : TCODRandom generator
            Outputs     : None
            Return      : Point
        --------------------------------------------------------------------------------*/
        Point Area::getRandomPoint(TCODRandom& rand) const
        {
            int x = rand.getInt(topLeft.X(), bottomRight.X());
            int y = rand.getInt(topLeft.Y(), bottomRight.Y());
            return Point(x,y);
        }



        /*--------------------------------------------------------------------------------
            Function    : Area::getPossibleCorridorTo
            Description : returns a pair<Area,DirectionType> containing the area of a
                          possible corridor between this room and another, and the
                          direction in which that corridor would lie.
            Inputs      : Area object
            Outputs     : None
            Return      : pair<Area,DirectionType>
        --------------------------------------------------------------------------------*/
        pair<Area,DirectionType> Area::getPossibleCorridorTo(const Area& room) const
        {
            Point roomTL = room.getTL();
            Point roomBR = room.getBR();
            DirectionType direction; // which direction the other room lies

            if(bottomRight.Y() >= roomTL.Y())
                direction = SOUTH;
            else if(bottomRight.X() <= roomTL.X())
                direction = EAST;
            else if(topLeft.Y() <= roomBR.Y())
                direction = NORTH;
            else //(topLeft.X() >= roomBR.X())
                direction = WEST;

            if(direction == NORTH)
            {
                return pair<Area, DirectionType>(Area(greatestX(topLeft, room.getBL()),
                                                      leastX(getTR(), roomBR)), 
                                                      direction);
            }
            else if(direction == EAST)
            {
                return pair<Area, DirectionType>(Area(greatestY(getTR(), roomTL),
                                                      leastY(bottomRight, room.getBL())),
                                                      direction);  
            }
            else if(direction == SOUTH)
            {
                return pair<Area, DirectionType>(Area(greatestX(getBL(), roomTL),
                                                      leastX(bottomRight, room.getTR())),
                                                      direction);
            }
            else if(direction == WEST)
            {
                return pair<Area, DirectionType>(Area(greatestY(topLeft, room.getTR()),
                                                      leastY(getBL(), roomBR)),
                                                      direction);
            }
            else return pair<Area, DirectionType>(Area(Point(0,0), Point(0,0)), NORTH); //error
        }



     /* XXX Area creation helper functions XXX */



        /*-------------------------------------------------------------------------------- 
            Function    : addColumnsToSquareArea
            Description : adds a series of columns to any square room.
            Inputs      : Map object, random number generator, topleft and 
                          bottomright corners of a room
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void addColumnsToSquareRoom(MapPtr map, TCODRandom& rand, const Area& room)
        {
            Point columnOffset(2,2);
            Point TL = room.getTL() + columnOffset;
            Point BR = room.getBR() - columnOffset;
            int width = BR.X() - TL.X();
            int height = BR.Y() - TL.Y();
            if((width%2 && height%2))
                return;
            int density = rand.getInt(3,4);
            //int heightDensity = rand.getInt(2,4);

            for(int x=TL.X(); x<=BR.X(); x++)
            {
                for(int y=TL.Y(); y<=BR.Y(); y++)
                {
                    // Only on checkerboard square
                    if((x + y) & 1) continue;
                    if(x % density) continue;
                    if(y % density) continue;
                    map->setBottomMostAt(x,y, map->tileset->getWallTileID());
                }
            }
        }



    /* XXX Area Creation functions XXX */



        /*--------------------------------------------------------------------------------
            Function    : squareHall
            Description : generates an empty square room that fills an entire node.
            Inputs      : Level object, BSP node
            Outputs     : None
            Return      : a Area object to add to the Level's roomList
        --------------------------------------------------------------------------------*/
        Area squareHall(MapPtr map, const TCODBsp& node)
        {
            // get node dimensions, offset one to account for room walls
            int x = node.x, y = node.y;         // topleft corner
            int w = x+node.w-1, h = y+node.h-1; // bottomright corner

            // dig the room
            for(int a=x; a<w; a++)
            {
                for(int b=y; b<h; b++)
                {
                    map->setBottomMostAt(a,b, map->tileset->getFloorTileID());
                }
            }

            return Area(Point(x-1,y-1), Point(w,h));
        }



        /*--------------------------------------------------------------------------------
            Function    : squareArea
            Description : similar to squareHall(), except that it only digs out a
                          subsection of the node.
            Inputs      : Level object, BSP node, random number generator, minimum room
                          dimension
            Outputs     : None
            Return      : a Area object to add to the Level's roomList
        --------------------------------------------------------------------------------*/
        Area squareRoom(MapPtr map, const TCODBsp& node, TCODRandom& rand, const int minDim)
        {
            // get the node dimensions, they're the room limits
            int xlim = node.x, ylim = node.y;
            int wlim = xlim+node.w-1, hlim = ylim+node.h-1;

            // get the new, smaller corners
            int x = rand.getInt(xlim, wlim-minDim);
            int y = rand.getInt(ylim, hlim-minDim);
            int w = rand.getInt(x+minDim, wlim);
            int h = rand.getInt(y+minDim, hlim);

            // dig the room
            for(int a=x; a<w; a++)
            {
                for(int b=y; b<h; b++)
                {
                    map->setBottomMostAt(a,b, map->tileset->getFloorTileID());
                }
            }

            return Area(Point(x-1,y-1), Point(w,h));
        }



        /*--------------------------------------------------------------------------------
            Function    : crossHall
            Description : forms an irregular shaped room by making two smaller,
                          intersecting rooms in the same node.  Since this is a hall, and
                          therefore as big as the node, one of the smaller rooms will
                          stretch across the full width of the node, and the other
                          across the height.
            Inputs      : Level object, BSP node, random number generator, minimum room
                          dimension
            Outputs     : None
            Return      : a Area object to add to the Level's roomList
        --------------------------------------------------------------------------------*/
        // TODO: Consider breaking this function up into
        // T shaped rooms, L shaped rooms, and + shaped rooms.
        Area crossHall(MapPtr map, const TCODBsp& node, TCODRandom& rand, const int minDim)
        {
            // get the node dimensions, they're the room limits
            int llim = node.x, tlim = node.y;
            int rlim = llim+node.w-1, blim = tlim+node.h-1;
            
            // get the new corners of the two smaller rooms
            // to promote symmetry, the room stretching across the largest
            // of the two dimensions will be fixed in the middle of the room,
            // while the smaller room can slide across the larger room, thus
            // making a room similar to a cathedral.

            // determine whether the vertical or horizontal dimension will be fixed
            bool vert;
            if(node.w < node.h) vert = true;
            else vert = false;

            // calculate room corner points
            unsigned int tlx1, tlx2;
            unsigned int tly1, tly2;
            unsigned int brx1, brx2;
            unsigned int bry1, bry2;

            unsigned int width  = rand.getInt(minDim, 3*node.w/4, 0);
            unsigned int height = rand.getInt(minDim, 3*node.h/4, 0);

            if(vert)
            {
                // calculate first room
                tlx1 = llim + (node.w-width)/2; // centers the width
                tly1 = tlim;
                brx1 = tlx1 + width;
                bry1 = blim;

                // calculate second room
                tlx2 = llim;
                tly2 = tlim + rand.getInt(0, node.h-height);
                brx2 = rlim;
                bry2 = tly2 + height;
            }
            else
            {
                // calculate first room
                tlx1 = llim + rand.getInt(0, node.w-width);
                tly1 = tlim;
                brx1 = tlx1 + width;
                bry1 = blim;

                // calculate second room
                tlx2 = llim;
                tly2 = tlim + (node.h-height)/2; // centers the height;
                brx2 = rlim;
                bry2 = tly2 + height;
            }

            // cross rooms occasionally dig all the way up to the map edge.
            // These next few lines are a hack to prevent this.
            if(brx1 == map->getWidth()) --brx1;
            if(bry1 == map->getHeight()) --bry1;
            if(brx2 == map->getWidth()) --brx2;
            if(bry2 == map->getHeight()) --bry2;

            // dig the rooms
            for(unsigned int a=tlx1; a<brx1; a++)
            {
                for(unsigned int b=tly1; b<bry1; b++)
                {
                    map->setBottomMostAt(a,b, map->tileset->getFloorTileID());
                }
            }
            for(unsigned int a=tlx2; a<brx2; a++)
            {
                for(unsigned int b=tly2; b<bry2; b++)
                {
                    map->setBottomMostAt(a,b, map->tileset->getFloorTileID());
                }
            }
            return Area(Point(llim-1, tlim-1), Point(rlim, blim));
        }



        /*--------------------------------------------------------------------------------
            Function    : circularHall
            Description : makes a circular room that stretches across the entire node.
                          Formed by picking the center of the node as the center of the
                          circle, and then digging out each tile that falls within the
                          circle's radius, which is half of the smallest dimension of the
                          node.  This means, of course, that a circularHall can only be
                          as big across as the smallest node dimension; if the node is not
                          square, this may make a circular hall much smaller than its 
                          parent node.
            Inputs      : Level object, TCODBsp node
            Outputs     : None
            Return      : a Area object to add to the Level's roomList 
        --------------------------------------------------------------------------------*/
        Area circularHall(MapPtr map, const TCODBsp& node)
        {
            // get node dimensions, offset one if a dimension is even to allow
            // for a center point.
            int x = node.x, y = node.y;         // topleft corner
            int w = (node.w%2) ? x+node.w : x+node.w-1;
            int h = (node.h%2) ? y+node.h : y+node.h-1; // bottomright corner

            // determine if node is horizontally or vertically biased
            //if(w<h) bool vert = true;
            //else bool vert = false;

            // calculate the center point and the radius
            // TODO: make the center point able to slide along the larger dimension
            Point center(x+(w-x)/2, y+(h-y)/2);
            int radius = min(node.w, node.h) / 2;

            for(int a=x; a<w; a++)
            {
                for(int b=y; b<h; b++)
                {
                    // compare distance to radius squared, since that works the same
                    // and avoids the square root function in distance
                    if(relDistance(center, Point(a,b)) < radius*radius)
                    {
                        map->setBottomMostAt(a,b, map->tileset->getFloorTileID());
                    }
                }
            }

            // return a room object shrunk from the size of the node to the new size
            // of the room
            // XXX: this is currently bugged, but may be useful for making columns
            //Point offset(radius/2, radius/2);
            return Area(Point(center.X()-radius, center.Y()-radius),
                        Point(center.X()+radius, center.Y()+radius));
        }
    }
}
