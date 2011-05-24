#include "Level.hpp"

using namespace std;
using namespace rlns::utl;

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Function    : Level::addLights
            Description : After map generation, this function goes through the map, finds
                          all tiles that have a light, creates that light, and adds it to
                          the level's lights vector.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------/
        void Level::addLights()
        {
            lights = map->getLights(); 
            vector<int>::iterator it, end;
            end = lights.end();
            for(it=lights.begin(); it!=end; ++it)
            {
                model::LightPtr light(new model::Light(*model::Light::findLight("Red Glow")));
                model::GameData::get()->getCurrentLevel()->addLight(utl::Point(20, 20), otherLight);
            }
        }*/



        /*--------------------------------------------------------------------------------
            Function    : Level::generateMap
            Description : Creates a MapGenerator and constructs the Level's Map with it.
            Inputs      : name of the map's tileset, whether the map is approached from
                          below (determines what stairs the party starts on)
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Level::generateMap(const string& t, const bool ascending)
        {
            TilesetPtr tileset = Tileset::findTileset(t);

            if(tileset->getType() == MapType::DUNGEON)
            {
                DungeonGenerator gen(tileset);
                map = gen.generate(ascending);
                map->lightLights();
            }
            else
            {
                utl::fatalError("Unknown Map Type: " + tileset->getType());
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Level::Level(RLNSZip&)
            Description : Creates a level based on data found in the given RLNSZip buffer.
                          Used when loading the game state from saved games.
            Inputs      : RLNSZip buffer.
            Outputs     : None
            Return      : None (constructor)
        --------------------------------------------------------------------------------*/
        Level::Level(RLNSZip& zip)
        : map(new Map(zip)) {}



        /*--------------------------------------------------------------------------------
            Function    : Level::getStairs()
            Description : Returns a vector of the positions of all the stairs in the level.
            Inputs      : None
            Outputs     : None
            Return      : vector<Point>
        --------------------------------------------------------------------------------*/
        vector<Point> Level::getStairs()
        {
            std::vector<utl::Point> stairs = getUpStairs();
            std::vector<utl::Point> downstairs = getDownStairs();
            stairs.insert(stairs.end(), downstairs.begin(), downstairs.end());
            return stairs;
        }



        /*--------------------------------------------------------------------------------
            Function    : Level::checkForTileChar
            Description : Checks if the  map has a tile with the given foreground 
                          character at the given point.
            Inputs      : Point, character
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool Level::checkForTileChar(const Point& pt, const char ch) const
        {
            vector<TilePtr> tiles = map->getTiles(pt);
            vector<TilePtr>::const_iterator it, end = tiles.end();

            for(it=tiles.begin(); it!=end; ++it)
            {
                if((*it)->getChar() == ch)
                    return true;
            }
            return false;
        }



        /*--------------------------------------------------------------------------------
            Function    : Level::getLights
            Description : Returns an vector of all the light indices used by either the
                          level itself or its occupants.
            Inputs      : None
            Outputs     : None
            Return      : vector<int>
        --------------------------------------------------------------------------------*/
        vector<int> Level::getLights() const
        {
            return map->getLights();
        }



        /*--------------------------------------------------------------------------------
            Function    : Level::removeLight
            Description : Searches the lights vector for a light that has the same
                          location as the given point and removes it.
            Inputs      : Point
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------/
        void Level::removeLight(const Point& p)
        {
            map<int, LightPtr>::iterator it, end;
            end = lights.end();
            for(it=lights.begin(); it != end; ++it)
            {
                if((*it)->getPosition() == p)
                {
                    lights.erase(it);
                    break;
                }
            }
        }*/



        /*--------------------------------------------------------------------------------
            Function    : Level::saveToDisk
            Description : Saves the Level to the given save buffer.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Level::saveToDisk(RLNSZip& zip) const
        {
            map->saveToDisk(zip);

            // save features
            //saveSharedPtrContainer(features, zip);
        }
    }
}
