#include "MapGenerator.hpp"

using namespace std;
using namespace rlns::utl;

namespace rlns
{
    namespace model
    {
        TCODRandom MapGenerator::rand;

        /*--------------------------------------------------------------------------------
            Function    : MapGenerator::createBSPTree
            Description : creates and recursively splits the libtcod BSP tree.  The root 
                          node covers all of the internal tiles of the map.  All of the
                          edge tiles are not considered, since we must make sure that no 
                          room opens onto the edge of the map.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void MapGenerator::createBSPTree()
        {
            // topleft corner of the root node
            int x = 1, y = 1;
            // bottomright corner of the root node              
            // 
            // For some reason, w needs to be set to Width-2, otherwise rooms can run
            // a tile over the eastern edge of the map. h is fine to be set at Height-1.
            int w = map->getWidth()-2, h = map->getHeight()-1; 
            bsp.reset(new TCODBsp(x,y,w,h));
            bsp->splitRecursive(NULL,
                                recursionLevel,
                                minHSize,
                                minVSize,
                                maxHRatio,
                                maxVRatio);
        }



        /*--------------------------------------------------------------------------------
            Function    : getBorderCode
            Description : assembles an integer code that tells which of the tiles
                          surrounding another are of a given character code. The code is
                          similar to how Unix file permissions work; each unique code is,
                          when converted to binary, a series of boolean flags.
            Inputs      : a Point and a character constant.
            Outputs     : None
            Return      : int
        --------------------------------------------------------------------------------*/
        int MapGenerator::getBorderCode(const utl::Point& center, const int ch)
        {
            // get center Point coordinates
            int x = center.getX();
            int y = center.getY();
            int code = 0;

            // assemble code.  The try/catch's are in case the Point is at the edge of the
            // map, in which case, the code is not incremented, since a non-existant tile
            // is certainly not equal to ch.
            //
            // try each compass direction and trip a flag if the character is found
            // from North -> NorthEast -> ... -> NorthWest
            try { if(map->getTileID(x,y-1) == ch) code += 1; }
            catch(out_of_range&) {}

            try { if(map->getTileID(x+1,y-1) == ch) code += 2; }
            catch(out_of_range&) {}

            try { if(map->getTileID(x+1,y) == ch) code += 4; }
            catch(out_of_range&) {}

            try { if(map->getTileID(x+1,y+1) == ch) code += 8; }
            catch(out_of_range&) {}

            try { if(map->getTileID(x,y+1) == ch) code += 16; }
            catch(out_of_range&) {}

            try { if(map->getTileID(x-1,y+1) == ch) code += 32; }
            catch(out_of_range&) {}

            try { if(map->getTileID(x-1,y) == ch) code += 64; }
            catch(out_of_range&) {}

            try { if(map->getTileID(x-1,y-1) == ch) code += 128; }
            catch(out_of_range&) {}

            return code;
        }



        /*--------------------------------------------------------------------------------
            Function    : MapGenerator::digDownStairs
            Description : Puts between one and three downStairs at various spots of the
                          map.  TODO: In the future, the number of stairs might be 
                          dependent on information read from tileset.txt.
            Inputs      : whether the stairs are up or down, number of stairs to generate
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void MapGenerator::digStairs(const bool up, const int n)
        {
            vector<Point> stairs(n, Point()); // create a Point vector of length n
            vector<int> selectedAreas;
            for(int i=0; i<n; ++i)
            {
                // select an area that hasn't already been selected
                int x;
                do { x = rand.getInt(0, areas.size()-1); }
                while(count(selectedAreas.begin(), selectedAreas.end(), x) > 0); 
                AreaPtr area = areas.at(x);

                // find a random floor tile in the selected Area
                do { stairs[i] = area->getRandomPoint(rand); } 
                while(!map->isWalkable(stairs.at(i)));

                // update the appropriate stairs vector
                if(up) map->setUpStairs(stairs);
                else   map->setDownStairs(stairs);

                // add the downStairs tiles to tileMap
                vector<Point>::const_iterator it, end;
                end = stairs.end();
                for(it=stairs.begin(); it!=end; ++it)
                {
                    if(up) map->addFeature(*it, map->getTileset()->getUpStairTileID());
                    else   map->addFeature(*it, map->getTileset()->getDownStairTileID());
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : MapGenerator::digStaircases
            Description : Puts one up and one down staircase on the map.  Other Map
                          Generators might put multiples of each.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void MapGenerator::digStaircases()
        {
            digStairs(true, 1);
            digStairs(false, 1);
        }
    }
}
