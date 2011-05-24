#include "DungeonGenerator.hpp"

using namespace rlns::utl;
using namespace std;

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Function    : DungeonGenerator::DungeonCallback::visitNode
            Description : callback function for the DungeonCallback class.  This function
                          is called during BSP traversals.
            Inputs      : TCODBsp pointer, void pointer (for user supplied data)
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool DungeonGenerator::DungeonCallback::visitNode(TCODBsp* node, void* userData)
        {
            if(node->isLeaf())
            {
                // convert the void pointer userData to the parent class
                DungeonGenerator* parent = static_cast<DungeonGenerator*>(userData);
                parent->chooseRoomType(*node);
            }
            return true;
        }



        /*--------------------------------------------------------------------------------
            Function    : DungeonGenerator::addLights
            Description : Places a light in each room.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void DungeonGenerator::addLights()
        {
            Point pt;

            size_t numAreas = areas.size();
            for(size_t i=0; i<numAreas; ++i)
            {
                do { pt = areas.at(i)->getRandomPoint(rand); }
                while(map->getTileID(pt) != map->getFloorTileID());

                map->addFeature(pt, 500); // adds a brazier

                //LightPtr light(new Light(*Light::findLight("Lantern")));
                //GameData::get()->getCurrentLevel()->addLight(pt, light);
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : DungeonGenerator::chooseRoomType
            Description : Randomly chooses a room creation algorithm to apply to a given
                          BSP node.
            Inputs      : BSP node
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void DungeonGenerator::chooseRoomType(const TCODBsp& node)
        {
            Area newArea;
            switch(rand.getInt(0,3))
            {
                case 0:
                    newArea = squareRoom(map, node, rand, 10);
                    addColumnsToSquareRoom(map, rand, newArea);
                    break;
                case 1:
                    newArea =  squareHall(map, node);
                    addColumnsToSquareRoom(map, rand, newArea);
                    break;
                case 2:
                    newArea =  circularHall(map, node);
                    break;
                case 3:
                    newArea =  crossHall(map, node, rand, 10);
                    break;
                default:
                    newArea =  squareHall(map, node);
                    break;
            }

            // add the new Area to the areas vector
            AreaPtr newAreaPtr(new Area(newArea));
            areas.push_back(newAreaPtr);
        }



        /*--------------------------------------------------------------------------------
            Function    : MapGenerator::createRooms
            Description : iterates through the BSP tree and calls chooseRoom for each
                          node.  chooseRoom is responsible for choosing which room type
                          will be constructed in each node and calling the appropriate
                          function.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void DungeonGenerator::createRooms()
        {
            DungeonCallback c;
            bsp->traverseInvertedLevelOrder(&c, this);
        }



        /*--------------------------------------------------------------------------------
            Function    : DungeonGenerator::connectPoints
            Description : digs a corridor between two Points on the map
            Inputs      : two Points
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void DungeonGenerator::connectPoints(Point& p1, const Point& p2, bool vertfirst)
        {
            if(vertfirst)
            {
                while(p1.getY() != p2.getY())
                {
                    if(map->getTileID(p1) == map->getFillerTileID())
                        map->setTileID(p1, map->getFloorTileID());
                    if(p1.getY() > p2.getY())
                        p1.shift(NORTH);
                    else if(p1.getY() < p2.getY())
                        p1.shift(SOUTH);
                }
                while(p1.getX() != p2.getX())
                {
                    if(map->getTileID(p1) == map->getFillerTileID())
                        map->setTileID(p1, map->getFloorTileID());
                    if(p1.getX() > p2.getX())
                        p1.shift(WEST);
                    else if(p1.getX() < p2.getX())
                        p1.shift(EAST);
                }
            }
            else
            {
                while(p1.getX() != p2.getX())
                {
                    if(map->getTileID(p1) == map->getFillerTileID())
                        map->setTileID(p1, map->getFloorTileID());
                    if(p1.getX() > p2.getX())
                        p1.shift(WEST);
                    else if(p1.getX() < p2.getX())
                        p1.shift(EAST);
                }
                while(p1.getY() != p2.getY())
                {
                    if(map->getTileID(p1) == map->getFillerTileID())
                        map->setTileID(p1, map->getFloorTileID());
                    if(p1.getY() > p2.getY())
                        p1.shift(NORTH);
                    else if(p1.getY() < p2.getY())
                        p1.shift(SOUTH);
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : DungeonGenerator::connectRooms
            Description : connects two rooms to each other
            Inputs      : two room objects
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void DungeonGenerator::connectRooms(const AreaPtr r1, const AreaPtr r2)
        {
            // get a random floor tile in each room
            Point r1point;
            Point r2point;

            do { r1point = r1->getRandomPoint(rand); }
            while(map->getTileID(r1point) != map->getFloorTileID());

            do { r2point = r2->getRandomPoint(rand); }
            while(map->getTileID(r2point) != map->getFloorTileID());

            connectPoints(r1point, r2point, rand.getInt(0,1));
        }



        /*--------------------------------------------------------------------------------
            Function    : DungeonGenerator::finishWalls()
            Description : iterates through every tile in the dungeon and checks each wall
                          tile it finds to see if it should change to a double-barred wall
                          character, and each floor tile to see if it should be a door.  
                          For each tile it finds, this function and its children assemble 
                          a code that deterimines which tiles around it are not also 
                          walls.  The code is similar to Unix file permission codes, in 
                          that the code, in binary, is a series of flags.  The code 
                          variable is essentially a four-bit binary number where each bit
                          refers to a cardinal direction.  If the given direction is a 
                          wall tile, then that bit is 1.  In decimal form, these codes 
                          form a unique decimal number useful in a switch statement.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void DungeonGenerator::finish()
        {
            int width = map->getWidth();
            int height = map->getHeight();

            for(int x=0; x<width; x++)
            {
                for(int y=0; y<height; y++)
                {
                    if(map->getTileID(x,y) == map->getFillerTileID())
                    {
                        setWall(Point(x,y));
                    }
                    else if(map->getTileID(x,y) == map->getFloorTileID())
                    {
                        checkForDoor(Point(x,y));
                    }
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : DungeonGenerator::setWall
            Description : Checks if the given coordinate needs a double-barred wall
                          character or not.
            Inputs      : point to check
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void DungeonGenerator::setWall(const Point& pt)
        {
            int code = getBorderCode(pt, map->getFloorTileID());
            switch(code)
            {
                case 1:
                case 3:
                case 16:
                case 17:
                case 24:
                case 27:
                case 31:
                case 38:
                case 48:
                case 51:
                case 56:
                case 59:
                case 63:
                case 117:
                case 129:
                case 131:
                case 153:
                case 155:
                case 157:
                case 159:
                case 177:
                case 179:
                case 183:
                case 185:
                case 187:
                case 191:
                case 241:
                case 243:
                case 249:
                case 251:
                    map->setTileID(pt, map->getWallTileID(1)); // horizontal line
                    break;
                case 4:
                case 6:
                case 12:
                case 14:
                case 64:
                case 57:
                case 68:
                case 96:
                case 102:
                case 108:
                case 110:
                case 111:
                case 124:
                case 126:
                case 192:
                case 198:
                case 199:
                case 204:
                case 205:
                case 206:
                case 207:
                case 220:
                case 224:
                case 230:
                case 231:
                case 236:
                case 239:
                case 238:
                case 252:
                case 254:
                    map->setTileID(pt, map->getWallTileID(2)); // vertical line
                    break;
                case 7:
                case 15:
                case 32:
                case 47:
                case 114:
                case 135:
                case 143:
                case 175:
                    map->setTileID(pt, map->getWallTileID(3)); // NE corner
                    break;
                case 8:
                case 52:
                case 141:
                case 193:
                case 195:
                case 225:
                case 227:
                case 235:
                    map->setTileID(pt, map->getWallTileID(4)); // NW corner
                    break;
                case 28:
                case 30:
                case 60:
                case 62:
                case 67:
                case 128:
                case 190:
                    map->setTileID(pt, map->getWallTileID(5)); // SE corner
                    break;
                case 2:
                case 112:
                case 120:
                case 216:
                case 240:
                case 248:
                case 250:
                    map->setTileID(pt, map->getWallTileID(6)); // SW corner
                    break;
                case 13:
                case 44:
                case 46:
                case 80:
                case 134:
                case 140:
                case 142:
                case 160:
                case 166:
                case 174:
                case 217:
                    map->setTileID(pt, map->getWallTileID(7)); // T West
                    break;
                case 10:
                case 53:
                case 98:
                case 104:
                case 106:
                case 122:
                case 181:
                case 194:
                case 200:
                case 202:
                case 226:
                case 232:
                case 234:
                    map->setTileID(pt, map->getWallTileID(8)); // T East
                    break;
                case 26:
                case 50:
                case 58:
                case 130:
                case 152:
                case 154:
                case 176:
                case 178:
                case 184:
                case 186:
                    map->setTileID(pt, map->getWallTileID(9)); // T North
                    break;
                case 11:
                case 35:
                case 40:
                case 43:
                case 137:
                case 139:
                case 161:
                case 163:
                case 169:
                case 171:
                    map->setTileID(pt, map->getWallTileID(10)); // T South
                    break;
                case 34:
                case 42:
                case 136:
                case 138:
                case 162:
                case 168:
                case 170:
                case 255:
                    map->setTileID(pt, map->getWallTileID(11)); // center cross
                    break;
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : DungeonGenerator::checkForDoor
            Description : Checks if the given floor tile should have a door in it or not.
            Inputs      : point to check
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void DungeonGenerator::checkForDoor(const Point& pt)
        {
            int code = getBorderCode(pt, map->getFloorTileID());
            switch(code)
            {
                case 78:
                case 196:
                case 204:
                case 228:
                case 230:
                case 238:
                    map->addFeature(pt, map->getTileset()->getE_W_DoorID()); // east/west door 
                    break;
                case 57:
                case 147:
                case 163:
                    map->addFeature(pt, map->getTileset()->getN_S_DoorID()); // north/south door
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : DungeonGenerator::generate
            Description : Starts the map generation process and returns a pointer to a
                          finished Map object.
            Inputs      : Whether the level is being approached from above. (determines
                          what kind of stairway attaches this level to the previous one)
            Outputs     : None
            Return      : MapPtr
        --------------------------------------------------------------------------------*/
        MapPtr DungeonGenerator::generate(const bool descending)
        {
            (void) descending; // TODO: find something to do with this parameter
            createBSPTree();
            createRooms();

            // connect the rooms with corridors
            size_t numAreas = areas.size();
            for(size_t i=1; i<numAreas; ++i)
            {
                connectRooms(areas[i], areas[i-1]);
            }

            digStaircases();
            addLights(); 
            finish();

            return map;
        }
    }
}
