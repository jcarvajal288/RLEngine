#ifndef RLNS_TYPES_HPP
#define RLNS_TYPES_HPP

#include <boost/shared_ptr.hpp>

#include "CheckedSave.hpp"

#include "libtcod.hpp"

namespace rlns
{
    // meta-variables
    extern bool LIGHT_FLICKER_ENABLED;
    extern bool IS_RUNNING;
    extern TCODColor UI_FORE_COLOR;
    extern TCODColor UI_BACK_COLOR;


    // forward declarations and shared_ptr typedefs
    class LCRL;
    class InitData;

    typedef boost::shared_ptr<LCRL> LCRLPtr;
    typedef boost::shared_ptr<InitData> InitDataPtr;
    typedef boost::shared_ptr<TCODConsole> TCODConsolePtr;
    typedef boost::shared_ptr<TCODNoise> TCODNoisePtr;

    typedef boost::shared_ptr<TCODMap> TCODMapPtr;

    enum EventType
    {
        CANCEL,
        COMMAND,
        EQUIPMENT,
        EXIT,
        MOVE_NORTH,
        MOVE_NORTHEAST,
        MOVE_EAST,
        MOVE_SOUTHEAST,
        MOVE_SOUTH,
        MOVE_SOUTHWEST,
        MOVE_WEST,
        MOVE_NORTHWEST,
        MOVE_CENTER,
        NO_EVENT,
        ITEM
    };


    class EventHandler;
    typedef boost::shared_ptr<EventHandler> EventHandlerPtr;

    enum AlignmentType
    {
        LAWFUL,
        NEUTRAL,
        CHAOTIC
    };

    enum CharacterClassType
    {
        CLERIC,
        THIEF,
        WARRIOR,
        WIZARD,
        DWARF,
        ELF,
        HALFLING
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

    enum MapType
    {
        DUNGEON,
        CAVE
    };

    enum MovementType
    {
        WALKING,
        SWIMMING,
        FLYING,
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
        MULTIPLE_CHARS = 3,
        NOTABLE = 4,
        NUM_TILE_FLAGS = 5
    };

    class AbstractTile;
    class Actor;
    class Area;
    class Feature;
    class GameData;
    class Level;
    class LevelNode;
    class LevelTree;
    class Light;
    class Inventory;
    class Item;
    class Map;
    class MapBuilder;
    class MapObject;
    class Party;
    class Race;
    class Tile;
    class Tileset;

    typedef boost::shared_ptr<AbstractTile> AbstractTilePtr;
    typedef boost::shared_ptr<Actor> ActorPtr;
    typedef boost::shared_ptr<Area> AreaPtr;
    typedef boost::shared_ptr<Feature> FeaturePtr;
    typedef boost::shared_ptr<GameData> GameDataPtr;
    typedef boost::shared_ptr<Level> LevelPtr;
    typedef boost::shared_ptr<LevelNode> LevelNodePtr;
    typedef boost::shared_ptr<LevelTree> LevelTreePtr;
    typedef boost::shared_ptr<Light> LightPtr;
    typedef boost::shared_ptr<Inventory> InventoryPtr;
    typedef boost::shared_ptr<Item> ItemPtr;
    typedef boost::shared_ptr<Map> MapPtr;
    typedef boost::shared_ptr<MapBuilder> MapBuilderPtr;
    typedef boost::shared_ptr<MapObject> MapObjectPtr;
    typedef boost::shared_ptr<TCODBsp> TCODBspPtr;
    typedef boost::shared_ptr<Party> PartyPtr;
    typedef boost::shared_ptr<Race> RacePtr;
    typedef boost::shared_ptr<Tile> TilePtr;
    typedef boost::shared_ptr<Tileset> TilesetPtr;

    // if this is the debug build, we want to save games using the CheckedZip class which
    // checks the consistency of the save files but inflates the size of the save.
    // Otherwise, use the smaller default TCODZip.
#ifdef _DEBUG
    typedef CheckedZip RLNSZip;
#else
    typedef TCODZip RLNSZip;
#endif
    class Display;
    class Message;
    class MessageTracker;
    typedef boost::shared_ptr<Display> DisplayPtr;
    typedef boost::shared_ptr<Message> MessagePtr;
    typedef boost::shared_ptr<MessageTracker> MessageTrackerPtr;
}

#endif
