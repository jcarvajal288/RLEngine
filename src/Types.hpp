#ifndef RLNS_TYPES_HPP
#define RLNS_TYPES_HPP

#include <boost/shared_ptr.hpp>

#include "libtcod.hpp"

namespace rlns
{
    // meta-variables
    extern bool LIGHT_FLICKER_ENABLED;
    extern bool DEBUG_MODE;

    // forward declarations and shared_ptr typedefs
    class InitData;
    typedef boost::shared_ptr<InitData> InitDataPtr;
    typedef boost::shared_ptr<TCODNoise> TCODNoisePtr;

    typedef boost::shared_ptr<TCODMap> TCODMapPtr;

    namespace control
    {
        class EventHandler;
        typedef boost::shared_ptr<EventHandler> EventHandlerPtr;
    }

    namespace model
    {
        class Actor;
        class Area;
        class Feature;
        class GameData;
        class Level;
        class LevelNode;
        class LevelTree;
        class Light;
        class Map;
        class MapGenerator;
        class Tile;
        class Tileset;
        typedef boost::shared_ptr<Actor> ActorPtr;
        typedef boost::shared_ptr<Area> AreaPtr;
        typedef boost::shared_ptr<Feature> FeaturePtr;
        typedef boost::shared_ptr<GameData> GameDataPtr;
        typedef boost::shared_ptr<Level> LevelPtr;
        typedef boost::shared_ptr<LevelNode> LevelNodePtr;
        typedef boost::shared_ptr<LevelTree> LevelTreePtr;
        typedef boost::shared_ptr<Light> LightPtr;
        typedef boost::shared_ptr<Map> MapPtr;
        typedef boost::shared_ptr<MapGenerator> MapGeneratorPtr;
        typedef boost::shared_ptr<TCODBsp> TCODBspPtr;
        typedef boost::shared_ptr<Tile> TilePtr;
        typedef boost::shared_ptr<Tileset> TilesetPtr;
    }

    namespace view
    {
        class Display;
        class Message;
        class Renderer;
        typedef boost::shared_ptr<Display> DisplayPtr;
        typedef boost::shared_ptr<Message> MessagePtr;
        typedef boost::shared_ptr<Renderer> RendererPtr;
    }


    enum EventType
    {
        MOVE,
        NO_EVENT,
        EXIT
    };

    enum DirectionType
    {
        NORTH,
        NORTHEAST,
        EAST,
        SOUTHEAST,
        SOUTH,
        SOUTHWEST,
        WEST,
        NORTHWEST,
        CENTER,
        NO_DIRECTION
    };

    enum TileActionType
    {
        OPEN = 0,
        TOGGLE = 1,
        NUM_TILE_ACTIONS = 2
    };

    enum TileFlagType
    {
        BLOCKS_LIGHT = 0,
        BLOCKS_WALK = 1,
        DIRECTIONALLY_LINKED = 2,
        NOTABLE = 3,
        NUM_TILE_FLAGS = 4
    };

    enum MapType
    {
        DUNGEON
    };
}

#endif
