#include "Level.hpp"

using namespace std;

namespace rlns
{
    vector<LevelPtr> Level::levels;
    unsigned int Level::currentLevel = 0;

    /*--------------------------------------------------------------------------------
        Function    : Level::Level
        Description : Constructor for the Level object.  Makes a map filled
                      with its filler tile.
        Inputs      : Name of a tileset
        Outputs     : None
        Return      : None (constructor)
    --------------------------------------------------------------------------------*/
    Level::Level(const string& tilesetName)
    : map(new Map(Tileset::findTileset(tilesetName)))
    {
        if(tilesetName == "Castle")
        {
            DungeonBuilder dungeonBuilder(map);
            dungeonBuilder.buildMap();
            areas = dungeonBuilder.getAreas();
        }
        else if(tilesetName == "Cavern")
        {
            CaveBuilder caveBuilder(map);
            caveBuilder.buildMap();
            areas = caveBuilder.getAreas();
        }

        // add items
        RoomFiller roomFiller(map, 1);
        vector<AreaPtr>::const_iterator it, end;
        it = areas.begin(); end = areas.end();
        for(; it!=end; ++it)
        {
            items.push_back(roomFiller.genItem(*it));
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : Level::Level(RLNSZip&)
        Description : Loads a previously saved level from the given save buffer.
        Inputs      : RLNSZip save buffer
        Outputs     : None
        Return      : None (constructor)
    --------------------------------------------------------------------------------*/
    Level::Level(RLNSZip& zip)
    : map(new Map(zip)) {}



    /*--------------------------------------------------------------------------------
        Function    : Level::getTileInfo
        Description : Returns a TileInfo struct containing the ascii character of a
                      tile as well as its foreground and background colors.
        Inputs      : Coordinates of the tile
        Outputs     : None
        Return      : TileInfo
    --------------------------------------------------------------------------------*/
    TileInfo Level::getTileInfo(const int x, const int y) const
    {
        // create a blank TileInfo
        TileInfo tileInfo = { TCODColor::fuchsia, TCODColor::fuchsia, 0 };

        // get the list of tile IDs at the given coordinates
        vector<int> tileIDs = map->at(x,y);

        // fetch the light info at the given coordinates
        //TCODColor light = getTileLightInfo(x,y);

        // create iterators
        vector<int>::const_reverse_iterator it, rend;
        it = tileIDs.rbegin();
        rend = tileIDs.rend();

        // load the character of the topmost terrain feature
        tileInfo.ascii = Tile::findTile(*it)->getChar();

        // iterate through the tiles list, finding the topmost foreground and background colors
        for(; it!=rend; ++it)
        {
            // the fuchsia color is treated as being blank for our purposes,
            // so once we find a non-fuchsia color, we have a color we want to display
            if(tileInfo.fgColor == TCODColor::fuchsia) 
            {
                tileInfo.fgColor = Tile::findTile(*it)->getFgColor();
            }

            if(tileInfo.bgColor == TCODColor::fuchsia) 
            {
                tileInfo.bgColor = Tile::findTile(*it)->getBgColor();
            }
        }

        /* finally, display the correctly colored tile
        if(DEBUG_MODE)
        {
            TCODColor debuglight = TCODColor::white * 0.5f;
            light.r = max(debuglight.r, light.r);
            light.g = max(debuglight.g, light.g);
            light.b = max(debuglight.b, light.b);
        }*/

        return tileInfo;
    }



    /*--------------------------------------------------------------------------------
        Function    : Level::fetchItemsAtLocation
        Description : Creates a vector of the items at a specified tile, REMOVES them
                      from the items vector, and returns this vector.  This is used
                      when characters pick up items from a location to add them to
                      their inventory.
        Inputs      : location to fetch items from
        Outputs     : None
        Return      : a vector of the items at the specified location
    --------------------------------------------------------------------------------*/
    vector<ItemPtr> Level::fetchItemsAtLocation(const Point& pt)
    {
        vector<ItemPtr> fetchedItems;
        vector<ItemPtr> otherItems;
        vector<ItemPtr>::iterator it, end;
        it = items.begin(); end = items.end();

        PositionEquals positionEquals(pt);
        for(; it!=end; ++it)
        {
            if(positionEquals(*it))
            {
                fetchedItems.push_back(*it);
            }
            else
            {
                otherItems.push_back(*it);
            }
        }

        items = otherItems;
        return fetchedItems;
    }



    /*--------------------------------------------------------------------------------
        Function    : Level::inspectTileContents
        Description : Returns a string revealing what creatures and items are in the
                      tile at the given point on the map.
        Inputs      : message string, Point to inspect
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Level::inspectTileContents(string& message, const Point& pt) const
    {
        // TODO: get a list of all of the creatures at the point

        // get a list of all of the items at the point
        vector<ItemPtr> itemsAtPt;
        vector<ItemPtr>::const_iterator it, end;
        it = items.begin(); end = items.end();

        for(; it!=end; ++it)
        {
            if((*it)->getPosition() == pt)
            {
                itemsAtPt.push_back(*it);
            }
        }

        if(itemsAtPt.empty()) return;

        // construct the info string
        message = "You see ";
        it = itemsAtPt.begin(); end = itemsAtPt.end();

        for(; it!=end; ++it)
        {
            string part = (*it)->shortDescription();
            if(it == end-1) part += ".";
            else part += ", ";
            message.append(part);
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : Level::saveToDisk
        Description : Saves the Level object to the given save buffer.
        Inputs      : RLNSZip save buffer
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Level::saveToDisk(RLNSZip& zip) const
    {
        map->saveToDisk(zip);
    }



    /*--------------------------------------------------------------------------------
        Function    : Level::addLevel
        Description : Adds a new level of the given tileset to the level list.
        Inputs      : Name of a tileset
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Level::addLevel(const string& tilesetName)
    {
        LevelPtr newLevel(new Level(tilesetName));
        Level::levels.push_back(newLevel);
    }



    /*--------------------------------------------------------------------------------
        Function    : Level::saveLevelsToDisk
        Description : Saves all of the levels to the given save buffer.
        Inputs      : RLNSZip save buffer
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Level::saveLevelsToDisk(RLNSZip& zip)
    {
        zip.putInt(Level::currentLevel);

        vector<LevelPtr>::const_iterator it, end;
        it = Level::levels.begin(); end = Level::levels.end();

        zip.putInt(Level::levels.size());
        for(; it!=end; ++it)
        {
            (*it)->saveToDisk(zip);
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : Level::loadLevelsFromDisk
        Description : Loads the level list from the given save buffer.
        Inputs      : RLNSZip save buffer
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Level::loadLevelsFromDisk(RLNSZip& zip)
    {
        currentLevel = zip.getInt();
        int numLevels = zip.getInt();

        while(numLevels --> 0)
        {
            LevelPtr level(new Level(zip));
            levels.push_back(level);
        }
    }
}
