#include "DungeonBuilder.hpp"

using namespace std;

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::DungeonCallback::visitNode
        Description : callback function for the DungeonCallback class.  This function
                      is called during BSP traversals.
        Inputs      : TCODBsp pointer, void pointer (for user supplied data)
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool DungeonBuilder::DungeonCallback::visitNode(TCODBsp* node, void* userData)
    {
        if(node->isLeaf())
        {
            // convert the void pointer userData to the parent class
            DungeonBuilder* parent = static_cast<DungeonBuilder*>(userData);
            parent->buildRoomType(*node);
        }
        return true;
    }



    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::addLights
        Description : Places a light in each room.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void DungeonBuilder::addLights()
    {/*
        Point pt;

        size_t numAreas = areas.size();
        for(size_t i=0; i<numAreas; ++i)
        {
            do { pt = areas.at(i)->getRandomPoint(rand); }
            while(map->at(pt) != map->tileset->getFloorTileID());

            map->addFeature(pt, 500); // adds a brazier

            //LightPtr light(new Light(*Light::findLight("Lantern")));
            //GameData::get()->getCurrentLevel()->addLight(pt, light);
        }
        */
    }



    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::buildRoomType
        Description : Randomly chooses a room creation algorithm to apply to a given
                      BSP node.
        Inputs      : BSP node
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void DungeonBuilder::buildRoomType(const TCODBsp& node)
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
        Function    : MapBuilder::createRooms
        Description : iterates through the BSP tree and calls chooseRoom for each
                      node.  chooseRoom is responsible for choosing which room type
                      will be constructed in each node and calling the appropriate
                      function.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void DungeonBuilder::createRooms()
    {
        DungeonCallback c;
        bsp->traverseInvertedLevelOrder(&c, this);
    }



    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::connectPoints
        Description : digs a corridor between two Points on the map
        Inputs      : two Points
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void DungeonBuilder::connectPoints(Point& p1, const Point& p2, bool vertfirst)
    {
        if(vertfirst)
        {
            while(p1.Y() != p2.Y())
            {
                if(map->bottomMostAt(p1) == map->tileset->getFillerTileID())
                    map->setBottomMostAt(p1, map->tileset->getFloorTileID());
                if(p1.Y() > p2.Y())
                    p1.shift(NORTH);
                else if(p1.Y() < p2.Y())
                    p1.shift(SOUTH);
            }
            while(p1.X() != p2.X())
            {
                if(map->bottomMostAt(p1) == map->tileset->getFillerTileID())
                    map->setBottomMostAt(p1, map->tileset->getFloorTileID());
                if(p1.X() > p2.X())
                    p1.shift(WEST);
                else if(p1.X() < p2.X())
                    p1.shift(EAST);
            }
        }
        else
        {
            while(p1.X() != p2.X())
            {
                if(map->bottomMostAt(p1) == map->tileset->getFillerTileID())
                    map->setBottomMostAt(p1, map->tileset->getFloorTileID());
                if(p1.X() > p2.X())
                    p1.shift(WEST);
                else if(p1.X() < p2.X())
                    p1.shift(EAST);
            }
            while(p1.Y() != p2.Y())
            {
                if(map->bottomMostAt(p1) == map->tileset->getFillerTileID())
                    map->setBottomMostAt(p1, map->tileset->getFloorTileID());
                if(p1.Y() > p2.Y())
                    p1.shift(NORTH);
                else if(p1.Y() < p2.Y())
                    p1.shift(SOUTH);
            }
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::connectRooms
        Description : connects two rooms to each other
        Inputs      : two room objects
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void DungeonBuilder::connectRooms(const AreaPtr r1, const AreaPtr r2)
    {
        // get a random floor tile in each room
        Point r1point;
        Point r2point;

        do { r1point = r1->getRandomPoint(rand); }
        while(map->bottomMostAt(r1point) != map->tileset->getFloorTileID());

        do { r2point = r2->getRandomPoint(rand); }
        while(map->bottomMostAt(r2point) != map->tileset->getFloorTileID());

        bool vertfirst = rand.getInt(0,1) != 0;
        connectPoints(r1point, r2point, vertfirst);
    }



    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::finishMap()
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
    void DungeonBuilder::finishMap()
    {
        int width = map->getWidth();
        int height = map->getHeight();

        for(int x=0; x<width; x++)
        {
            for(int y=0; y<height; y++)
            {
                if(map->bottomMostAt(x,y) == map->tileset->getFillerTileID())
                {
                    setWall(Point(x,y));
                }
                else if(map->bottomMostAt(x,y) == map->tileset->getFloorTileID())
                {
                    checkForDoor(Point(x,y));
                }
            }
        }

        placeStairs();
    }



    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::setWall
        Description : Checks if the given coordinate needs a double-barred wall
                      character or not.
        Inputs      : point to check
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void DungeonBuilder::setWall(const Point& pt)
    {
        int code = getBorderCode(map, pt, map->tileset->getFloorTileID());
        switch(code)
        {
            case 1:
            case 3:
            case 16:
            case 17:
            case 24:
            case 27:
            case 31:
            case 48:
            case 51:
            case 55:
            case 56:
            case 59:
            case 63:
            case 115:
            case 117:
            case 123:
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
            case 189:
            case 191:
            case 219:
            case 241:
            case 243:
            case 249:
            case 251:
                map->setBottomMostAt(pt, map->tileset->getWallTileID(1)); // horizontal line
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
            case 103:
            case 108:
            case 110:
            case 111:
            case 118:
            case 124:
            case 126:
            case 167:
            case 192:
            case 198:
            case 199:
            case 204:
            case 205:
            case 206:
            case 207:
            case 220:
            case 222:
            case 224:
            case 230:
            case 231:
            case 236:
            case 237:
            case 239:
            case 238:
            case 246:
            case 252:
            case 254:
                map->setBottomMostAt(pt, map->tileset->getWallTileID(2)); // vertical line
                break;
            case 7:
            case 15:
            case 32:
            case 39:
            case 47:
            case 114:
            case 135:
            case 143:
            case 173:
            case 175:
                map->setBottomMostAt(pt, map->tileset->getWallTileID(3)); // NE corner
                break;
            case 8:
            case 52:
            case 99:
            case 141:
            case 193:
            case 195:
            case 201:
            case 203:
            case 225:
            case 227:
            case 233:
            case 235:
                map->setBottomMostAt(pt, map->tileset->getWallTileID(4)); // NW corner
                break;
            case 28:
            case 30:
            case 54:
            case 60:
            case 62:
            case 67:
            case 128:
            case 156:
            case 158:
            case 182:
            case 188:
            case 190:
                map->setBottomMostAt(pt, map->tileset->getWallTileID(5)); // SE corner
                break;
            case 2:
            case 112:
            case 120:
            case 216:
            case 218:
            case 240:
            case 242:
            case 248:
            case 250:
                map->setBottomMostAt(pt, map->tileset->getWallTileID(6)); // SW corner
                break;
            case 13:
            case 38:
            case 44:
            case 46:
            case 80:
            case 83:
            case 134:
            case 140:
            case 142:
            case 160:
            case 166:
            case 172:
            case 174:
            case 217:
                map->setBottomMostAt(pt, map->tileset->getWallTileID(7)); // T West
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
                map->setBottomMostAt(pt, map->tileset->getWallTileID(8)); // T East
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
                map->setBottomMostAt(pt, map->tileset->getWallTileID(9)); // T North
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
                map->setBottomMostAt(pt, map->tileset->getWallTileID(10)); // T South
                break;
            case 34:
            case 42:
            case 127:
            case 136:
            case 138:
            case 162:
            case 168:
            case 170:
            case 223:
            case 253:
            case 255:
                map->setBottomMostAt(pt, map->tileset->getWallTileID(11)); // center cross
                break;
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::checkForDoor
        Description : Checks if the given floor tile should have a door in it or not.
        Inputs      : point to check
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void DungeonBuilder::checkForDoor(const Point& pt)
    {
        // Create a vector of all the possible wall tile IDs EXCEPT for the first
        // one, as that is the pillar tile, and doors should not be attached to
        // pillars.  Since the tiles after the current one won't be finished, the
        // filler tile should be included as well. There are eleven wall tiles in total.
        vector<int> tileIDs;
        tileIDs.push_back(map->tileset->getFillerTileID());
        for(int i=1; i<12; ++i)
        {
            tileIDs.push_back(map->tileset->getWallTileID()+i);
        }

        int code = getBorderCode(map, pt, tileIDs);
        switch(code)
        {
            case 17:
            case 19:
            case 25:
            case 27:
            //case 59:
            case 145:
            case 147:
            case 177:
            //case 185:
            {
                if(!hasDoorAdjacentTo(map, pt))
                {
                    map->addFeature(pt, map->tileset->getE_W_DoorID()); // east/west door 
                    break;
                }
            }
            case 68:
            case 76:
            case 78:
            case 100:
            case 108:
            //case 110:
            case 196:
            case 198:
            //case 206:
            //case 230:
            {
                if(!hasDoorAdjacentTo(map, pt))
                {
                    map->addFeature(pt, map->tileset->getN_S_DoorID()); // north/south door
                }
            }
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::buildBSPTree
        Description : Creates and recursively splits the libtcod BSP tree.  The root 
                      node covers all of the internal tiles of the map.  All of the
                      edge tiles are not considered, since we must make sure that no 
                      room opens onto the edge of the map.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void DungeonBuilder::buildBSPTree()
    {
        // topleft corner of the root node
        int x = 1, y = 1;
        // bottomright corner of the root node              
        // 
        // For some reason, w needs to be set to Width-2, otherwise rooms can run
        // a tile over the eastern edge of the map. h is fine to be set at Height-1.
        int w = map->getWidth()-2, h = map->getHeight()-1; 
        bsp.reset(new TCODBsp(x,y,w,h));
        bsp->splitRecursive(NULL, 6, 15, 15, 1.5, 1.5);
    }



    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::constructAreas
        Description : Takes the BSP tree made in MapBuilder::buildBSPTree and digs 
                      rooms in the areas it defined.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void DungeonBuilder::constructAreas()
    {
        createRooms();
    }



    /*--------------------------------------------------------------------------------
        Function    : DungeonBuilder::connectAreas
        Description : Digs corridors between the rooms built in constructAreas().
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void DungeonBuilder::connectAreas()
    {
        vector<AreaPtr>::const_iterator it, end;
        it = areas.begin() + 1; end = areas.end();

        for(; it!=end; ++it)
        {
            connectRooms(*(it-1), *it);
        }
    }
}
