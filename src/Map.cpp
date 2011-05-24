#include "Map.hpp"

using namespace rlns::utl;
using namespace std;

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Function    : Map::Map(RLNSZip&)
            Description : Creates a Map based on the data in a RLNSZip buffer.  Used when
                          loading saved games.
            Inputs      : RLNSZip buffer.
            Outputs     : None
            Return      : None (constructor)
        --------------------------------------------------------------------------------*/
        Map::Map(RLNSZip& zip)
        : tileset(Tileset::findTileset(zip.getString())),
          tileMap(tileset->getMapWidth(), 
                  vector< vector<int> >(tileset->getMapHeight(), 
                                                 vector<int>())),
          lightMap(tileset->getMapWidth(), 
                   vector<TCODColor>(tileset->getMapHeight(), 
                                     TCODColor::white * tileset->getAmbientLight())),
          fovMap(new TCODMap(tileset->getMapWidth(), tileset->getMapHeight()))
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
                        addFeature(x,y,zip.getInt());
                    }
                }
            }

            // load lights
            int numLights = zip.getInt();
            for(int i=0; i<numLights; ++i)
            {
                lights.push_back(zip.getInt());
            }
        }


        /*--------------------------------------------------------------------------------
            Function    : Map::getTiles
            Description : Fetches a vector of all the Tiles referred to in a particular
                          map coordinate.
            Inputs      : x coordinate, y coordinate
            Outputs     : None
            Return      : vector<TilePtr>
        --------------------------------------------------------------------------------*/
        vector<TilePtr> Map::getTiles(const int x, const int y) const
        {
            vector<int> tileIDs = tileMap.at(x).at(y);
            vector<TilePtr> tiles;
            vector<int>::const_iterator it, end;
            end = tileIDs.end();
            for(it=tileIDs.begin(); it!=end; ++it)
            {
                tiles.push_back(Tile::findTile(*it));
            }
            return tiles;
        }



        /*--------------------------------------------------------------------------------
            Function    : Map::signalTile
            Description : Sends the given signal to the tiles at the given map location.
            Inputs      : Point on the map to signal, the signal ID
            Outputs     : None
            Return      : bool (indicates whether the signal had any effect)
        --------------------------------------------------------------------------------*/
        bool Map::signalTile(const Point& pt, const TileActionType action)
        {
            int x = pt.getX(), y = pt.getY();
            bool result = false;

            vector<int>::iterator it, begin, end;
            begin = tileMap.at(x).at(y).begin();
            end = tileMap.at(x).at(y).end();

            for(it=begin; it!=end; ++it)
            {
                int n = Tile::findTile(*it)->signal(action);
                if(n > 0) // the signal had an effect
                {
                    // replace the IDs of the old tile with the new
                    replace(begin, end, *it, n);
                    result = true;
                }
            }
            updateFOVMap(x,y);
            return result;
        }



        /*--------------------------------------------------------------------------------
            Function    : Map::examineTile
            Description : Iterates through all the tiles at this location, and describes
                          any notable ones.
            Inputs      : position of the tile to examine
            Outputs     : descriptions of notable tiles
            Return      : void
        --------------------------------------------------------------------------------*/
        void Map::examineTile(const Point& pt) const
        {
            string desc;
            stringstream descStream;

            const vector<TilePtr>& tiles = getTiles(pt);
            vector<TilePtr>::const_iterator it, end = tiles.end();

            for(it=tiles.begin(); it!=end; ++it)
            {
                if((*it)->isNotable())
                {
                    descStream.str(desc);
                    descStream << "You see " << (*it)->getShortDesc() << " here.";
                    view::Display::get()->printToConsole(descStream.str());
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Map::clearLightMap
            Description : Resets every element of lightMap to the tileset's ambient light.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Map::clearLightMap()
        {
            vector< vector<TCODColor> >::iterator it, end;
            end = lightMap.end();
            for(it=lightMap.begin(); it!=end; ++it)
            {
                fill(it->begin(), it->end(), TCODColor::white * tileset->getAmbientLight());
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Map::lightLights
            Description : Finds each tile in the map with a native light, creates that
                          light, puts it in a vector, and then returns it.  This function
                          is called by Level after map generation.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Map::lightLights()
        {
            // TODO: delete the old lights from Light::lights
            lights.clear();

            int lightID;

            int xEnd = getWidth(), yEnd = getHeight();
            for(int x=0; x<xEnd; ++x)
            {
                for(int y=0; y<yEnd; ++y)
                {
                    vector<int> tiles = tileMap.at(x).at(y);
                    vector<int>::const_iterator it, end;
                    end = tiles.end();
                    for(it=tiles.begin(); it!=end; ++it)
                    {
                        TilePtr tile = Tile::findTile(*it);
                        string lightName = tile->getLight();
                        if(!lightName.empty())
                        {
                            lightID = Light::createLight(lightName);
                            Light::moveLight(lightID, Point(x,y));
                            lights.push_back(lightID);
                        }
                    }
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Map::updateFOVMap
            Description : Whenever the contents of tileMap are changed, fovMap needs to be
                          made aware of the changes.  This function tells fovMap to
                          reevaluate the walkability and transparency of the given tile.
            Inputs      : x coordinate, y coordinate
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Map::updateFOVMap(const int x, const int y)
        {
            bool transparent = true, walkable = true;

            vector<int>::iterator it, end;
            it = tileMap.at(x).at(y).begin();
            end = tileMap.at(x).at(y).end();

            for(; it!=end; ++it)
            {
                if(Tile::findTile(*it)->blocksLight())
                    transparent = false;
                if(Tile::findTile(*it)->blocksWalking())
                    walkable = false;
            }

            fovMap->setProperties(x,y, transparent, walkable);
        }



        /*--------------------------------------------------------------------------------
            Function    : Map::calculateFOV
            Description : updates the fovMap to reflect the new state of the map.  Uses
                          libtcod's TCODMap::computeFOV()
            Inputs      : center point, fov radius
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Map::calculateFOV(const Point& center, const int radius)
        {
            fovMap->computeFov(center.getX(), center.getY(), radius, true, FOV_SHADOW);
        }



        /*--------------------------------------------------------------------------------
            Function    : Map::calculatePartyFOV
            Description : Calculates the fov for each party member, updating a collective
                          FOV map as it goes.
            Inputs      : collective TCODMap, vector of party member positions
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Map::calculatePartyFOV(vector< vector<bool> >& partyFOV, const vector<Point>& positions)
        {
            vector<Point>::const_iterator it, end;
            end = positions.end();
            for(it=positions.begin(); it!=end; ++it)
            {
                calculateFOV(*it, 0); // 10 is a magic number that will probably be replaced by
                                       // a character's visual radius or somesuch.
                for(int x=0; x<getWidth(); ++x)
                {
                    for(int y=0; y<getHeight(); ++y)
                    {
                        if(fovMap->isInFov(x,y))
                        {
                            partyFOV.at(x).at(y) = true;
                        }
                    }
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Map::drawTile
            Description : draws a tile to the root console
            Inputs      : Pair of integers representing the tile's position in the root
                          console, and another representing the tile's position in the Map.
            Outputs     : tile to the root console
            Return      : void
        --------------------------------------------------------------------------------*/
        void Map::drawTile(const int a, const int b, const int x, const int y) const
        {
            // get the list of tiles at the given coordinates
            vector<TilePtr> tiles = getTiles(x,y);

            // create two variables loaded with 'programmer's purple' to track the colors
            TCODColor fg = TCODColor::fuchsia, bg = TCODColor::fuchsia;

            // fetch the light info at the given coordinates
            TCODColor light = getTileLightInfo(x,y);

            // create iterators
            vector<TilePtr>::const_reverse_iterator it, rbegin, rend;
            rbegin = tiles.rbegin();
            rend = tiles.rend();
            
            // load the character of the topmost terrain feature
            int character = (*rbegin)->getChar();

            // iterate through the tiles list, finding the topmost foreground and background colors
            for(it=tiles.rbegin(); it!=rend; ++it)
            {
                // the fuchsia color is treated as being blank for our purposes,
                // so once we find a non-fuchsia color, we have a color we want to display
                if(fg == TCODColor::fuchsia) fg = (*it)->getFgColor();
                if(bg == TCODColor::fuchsia) bg = (*it)->getBgColor();
            }

            // finally, display the correctly colored tile
            if(DEBUG_MODE)
            {
                TCODColor debuglight = TCODColor::white * 0.5f;
                light.r = max(debuglight.r, light.r);
                light.g = max(debuglight.g, light.g);
                light.b = max(debuglight.b, light.b);
            }
            TCODConsole::root->putCharEx(a,b, character, (fg * light), (bg * light));
        }



        /*--------------------------------------------------------------------------------
            Function    : Map::saveToDisk
            Description : Saves the Map to the given save buffer.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Map::saveToDisk(RLNSZip& zip) const
        {
            // save the name of the tileset
            zip.putString(tileset->getName().c_str());

            // save tileMap
            int xEnd = getWidth(), yEnd = getHeight();
            for(int x=0; x<xEnd; ++x)
            {
                for(int y=0; y<yEnd; ++y)
                {
                    vector<int> tiles = tileMap.at(x).at(y);
                    vector<int>::const_iterator it, end;
                    end = tiles.end();
                    zip.putInt(tiles.size());
                    for(it=tiles.begin(); it!=end; ++it)
                    {
                        zip.putInt(*it);
                    }
                }
            }

            // save lights vector
            zip.putInt(lights.size());
            vector<int>::const_iterator it, end;
            end = lights.end();
            for(it=lights.begin(); it!=end; ++it)
            {
                zip.putInt(*it);
            }
        }
    }
}
