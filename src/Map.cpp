#include "Map.hpp"

using namespace std;

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : Map::updateTileCoordinate
        Description : Updates the pathing and lighting information at the given
                      coordinate
        Inputs      : coordinate to update
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Map::updateTileCoordinate(const int x, const int y)
    {
        bool isTransparent = true;
        bool isWalkable = true;

        vector<int> tiles = at(x,y);
        vector<int>::const_iterator it, end;
        it = tiles.begin(); end = tiles.end();

        for(; it!=end; ++it)
        {
            TilePtr tile = Tile::findTile(*it);
            isTransparent = isTransparent && !tile->blocksLight();
            isWalkable = isWalkable && !tile->blocksWalking();
        }

        pathingMap.setProperties(x,y, isTransparent, isWalkable);
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::Map(TilesetPtr)
        Description : Constructs a blank map given a specific tileset.
        Inputs      : TilesetPtr
        Outputs     : None
        Return      : None (constructor)
    --------------------------------------------------------------------------------*/
    Map::Map(const TilesetPtr t)
    : tileset(t),
      lightMap(tileset->getMapWidth()*2, tileset->getMapHeight()*2),
      pathingMap(tileset->getMapWidth(), tileset->getMapHeight()),
      tileMap(tileset->getMapWidth(), 
              vector< vector<int> >(tileset->getMapHeight(), 
                                    vector<int>(1, t->getFillerTileID())))
    {
        lightMap.clear(tileset->getAmbientLight());
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::Map(RLNSZip&)
        Description : Creates a Map object from the given save buffer.
        Inputs      : RLNSZip save buffer
        Outputs     : None
        Return      : None (constructor)
    --------------------------------------------------------------------------------*/
    Map::Map(RLNSZip& zip)
    : tileset(Tileset::findTileset(zip.getString())),
      lightMap(tileset->getMapWidth(), tileset->getMapHeight()),
      pathingMap(tileset->getMapWidth(), tileset->getMapHeight()),
      tileMap(tileset->getMapWidth(), 
              vector< vector<int> >(tileset->getMapHeight(), vector<int>()))
    {
        // load tileMap
        int width = tileset->getMapWidth();
        int height = tileset->getMapHeight();
        for(int x=0; x<width; ++x)
        {
            for(int y=0; y<height; ++y)
            {
                int numTiles = zip.getInt();
                for(int z=0; z<numTiles; ++z)
                {
                    addFeature(x,y, zip.getInt());
                }
            }
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::at
        Description : Returns the tile vector at the given coordinates in the tileMap.
        Inputs      : x coordinate, y coordinate
        Outputs     : None
        Return      : vector<int>
    --------------------------------------------------------------------------------*/
    vector<int> Map::at(const int x, const int y) const
    {
        return tileMap.at(x).at(y);
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::topMostAt
        Description : Returns the tile ID at the end of the vector at the given
                      coordinates.  This is the most visible tile at that point.
        Inputs      : x coordinate, y coordinate
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    int Map::topMostAt(const int x, const int y) const
    {
        std::vector<int> tiles = tileMap.at(x).at(y);
        return tiles.at(tiles.size()-1);
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::bottomMostAt
        Description : Returns the tile ID at the begining of the vector at the given
                      coordinates.  This is the most visible tile at that point.
        Inputs      : x coordinate, y coordinate
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    int Map::bottomMostAt(const int x, const int y) const
    {
        std::vector<int> tiles = tileMap.at(x).at(y);
        return tiles.at(0);
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::setBottomMostAt
        Description : Sets the tile ID at the beginning of the vector at the given
                      coordinates.  This is the most visible tile at that point.
        Inputs      : x coordinate, y coordinate, new tile ID
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Map::setBottomMostAt(const int x, const int y, const int id) 
    {
        tileMap.at(x).at(y).at(0) = id;
        updateTileCoordinate(x,y);
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::addFeature
        Description : Adds the given tile id to the top of the tile vector at the 
                      given position and updates the pathingMap.
        Inputs      : coordinates where the new feature will be added, the feature id
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Map::addFeature(const int x, const int y, const int id)
    {
        tileMap.at(x).at(y).push_back(id);
        updateTileCoordinate(x,y);
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::isWalkable
        Description : Returns true if the tile at the given point is walkable, false
                      if not.
        Inputs      : coordinates of point to check
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool Map::isWalkable(const int x, const int y) const
    {
        return pathingMap.isWalkable(x,y);
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::moveLegal
        Description : Asks the map if movement using the given movement type is legal
                      at the given point.
        Inputs      : position of prospective move, type of movement
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool Map::moveLegal(const Point& pt, const MovementType moveType) const
    {
        switch(moveType)
        {
            case WALKING:
            case SWIMMING:
            case FLYING:
            default: // default to walking until more types are implemented
                return pathingMap.isWalkable(pt.X(), pt.Y());
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::signalTile
        Description : Signals all of the tiles at the given coordinate.
        Inputs      : coordinate, signal
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    bool Map::signalTile(const Point& pt, const TileActionType sig)
    {
        bool result = false;
        int x = pt.X(), y = pt.Y();

        vector<int>::iterator it, begin, end;
        begin = tileMap.at(x).at(y).begin(); 
        end = tileMap.at(x).at(y).end(); 

        for(it=begin; it!=end; ++it)
        {
            int i = Tile::findTile(*it)->signal(sig);
            if(i > 0) // the signaling had an effect
            {
                // replace the IDs of the old tile with the new
                replace(begin, end, *it, i);
                updateTileCoordinate(x,y);
                result = true;
            }
        }

        return result;
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::listTileFeatures
        Description : Adds any notable terrain features at the given point to the
                      vector parameter.  Terrain features are considered notable if
                      their 'notable' flag is set
        Inputs      : vector to add the notable features to, coordinate to inspect
        Outputs     : None
        Return      : None (information is passed out through the vector parameter)
    --------------------------------------------------------------------------------*/
    void Map::listTileFeatures(vector<AbstractTilePtr>& objectsAtPt, const Point& pt) const
    {
        vector<int> tiles = at(pt);
        vector<int>::const_iterator it, end;
        it = tiles.begin(); end = tiles.end();

        for(; it!=end; ++it)
        {
            TilePtr tile = Tile::findTile(*it);
            if(tile->isNotable())
            {
                objectsAtPt.push_back(tile);
            }
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : Map::saveToDisk
        Description : Saves the Map to the given save buffer.
        Inputs      : RLNSZip save buffer
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Map::saveToDisk(RLNSZip& zip) const
    {
        zip.putString(tileset->getName().c_str());

        int width = getWidth();
        int height = getHeight();

        vector<int> currentTile;
        vector<int>::const_iterator it, end;

        for(int x=0; x<width; ++x)
        {
            for(int y=0; y<height; ++y)
            {
                currentTile = at(x,y);
                it = currentTile.begin(); end = currentTile.end();

                zip.putInt(currentTile.size());
                for(; it!=end; ++it)
                {
                    zip.putInt(*it);
                }
            }
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : hasDoorAdjacentTo
        Description : Checks if there is a door one tile north, south east, or west
                      of the given point.  This is used to prevent doors being spawned
                      next to each other.
        Inputs      : MapPtr, position where a prospective door is to be placed
        Outputs     : None
        Return      : True if there is a door within one tile, false otherwise
    --------------------------------------------------------------------------------*/
    bool hasDoorAdjacentTo(const MapPtr map, const Point& pt)
    {
        int x = pt.X(), y = pt.Y();

        // check north
        if(map->topMostAt(x, y-1) == map->tileset->getN_S_DoorID())
            return true;

        // check south
        if(map->topMostAt(x, y+1) == map->tileset->getN_S_DoorID())
            return true;

        // check east
        if(map->topMostAt(x+1, y) == map->tileset->getE_W_DoorID())
            return true;

        // check west
        if(map->topMostAt(x-1, y) == map->tileset->getE_W_DoorID())
            return true;

        // if we get here, check fails for all directions, so return false
        return false;
    }
}
