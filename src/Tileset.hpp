#ifndef RLE_TILESET_HPP
#define RLE_TILESET_HPP

// see warning disable rationale in lcrl.hpp
#ifdef _WIN32
#pragma warning( disable : 4482 )
#endif

#include <string>
#include <vector>

#include "CheckedSave.hpp"
#include "FileParser.hpp"
#include "Tile.hpp"
#include "Types.hpp"
#include "Utility.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        struct temp_tileset
        {
             const char* n;
             MapType type;
             int ft;
             int wt;
             int flt;
             int us;
             int ds;
             int nsd;
             int ewd;
             TCODColor al;
             int mw;
             int mh;
             int rl;
             int mhs;
             int mvs;
             float mhr;
             float mvr;
        };



       /*-------------------------------------------------------------------------------- 
            Class       : Tileset
            Description : Contains the tiles that go into a certain style of map.  This
                          is passed to a Map object during map generation so the
                          generator knows what kind of map it's making and what tiles
                          it's using.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Tileset
        {
            // Member Variables
            public:
                static std::vector<TilesetPtr> list;

            private:
                std::string name;
                MapType type;

                // Tiles
                int floorTiles;
                int wallTiles;
                int fillerTile; // this tile fills the map at the start of map generation
                int upStair;
                int downStair;
                int n_s_door;
                int e_w_door;

                TCODColor ambientLight;

                // Level generation specifications
                int mapWidth;
                int mapHeight;

                int recurseLevel;
                int minHSize;
                int minVSize;
                float maxHRatio;
                float maxVRatio;

            // Member Functions
            private:
                static int findTilesetIndex(const std::string&);

            public:
                Tileset(const temp_tileset& t)
                : name(t.n),
                  type(t.type),
                  floorTiles(t.ft),
                  wallTiles(t.wt),
                  fillerTile(t.flt),
                  upStair(t.us),
                  downStair(t.ds),
                  n_s_door(t.nsd),
                  e_w_door(t.ewd),
                  ambientLight(t.al),
                  mapWidth(t.mw),
                  mapHeight(t.mh),
                  recurseLevel(t.rl),
                  minHSize(t.mhs),
                  minVSize(t.mvs),
                  maxHRatio(t.mhr),
                  maxVRatio(t.mvr) {}

                std::string getName()   const    { return name; }
                MapType getType()       const    { return type; }

                int getFloorTileID(const int i=0)  const    { return floorTiles + i; }
                int getWallTileID(const int i=0)   const    { return wallTiles + i;  }
                int getFillerTileID(const int i=0) const    { return fillerTile + i; }
                int getUpStairTileID()             const    { return upStair;        }
                int getDownStairTileID()           const    { return downStair;      }
                int getN_S_DoorID()                const    { return n_s_door;       }
                int getE_W_DoorID()                const    { return e_w_door;       }


                TCODColor getAmbientLight() const    { return ambientLight;  }

                int getMapWidth()       const    { return mapWidth;      }
                int getMapHeight()      const    { return mapHeight;     }
                int getRecurseLevel()   const    { return recurseLevel;  }
                int getMinHSize()       const    { return minHSize;      }
                int getMinVSize()       const    { return minVSize;      }
                float getMaxHRatio()    const    { return maxHRatio;     }
                float getMaxVRatio()    const    { return maxVRatio;     }

                static TilesetPtr findTileset(const std::string&);

                void saveToDisk(utl::RLNSZip&) const;
        };



        /*--------------------------------------------------------------------------------
            Class       : TilesetParser
            Description : Reads in data from the datafile tileset.txt and stores it in
                          Tileset's list variable.
            Parents     : FileParser
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class TilesetParser: public FileParser
        {
            // Member Variables
            private:
                /*--------------------------------------------------------------------------------
                    Class       : TilesetListener
                    Description : Constructs and executes a parser to read the datafile 
                                  tileset.txt.
                    Parents     : ITCODParserListener (see libtcod file parser documentation)
                    Children    : None
                    Friends     : None
                --------------------------------------------------------------------------------*/
                class TilesetListener: public ITCODParserListener
                {
                    // Member Variables
                    private:
                        temp_tileset temp;

                    // Member Functions
                    public:
                        bool parserNewStruct(TCODParser*, const TCODParserStruct*, const char*);
                        bool parserFlag(TCODParser*, const char*);
                        bool parserProperty(TCODParser*, const char*, TCOD_value_type_t, TCOD_value_t);
                        bool parserEndStruct(TCODParser*, const TCODParserStruct*, const char*);
                        void error(const char*);
                } listener;

            // Member Functions
            private:
                void verify();

            protected:
                void defineSyntax();

            public:
                TilesetParser(const std::string& n): FileParser(n)
                {
                    Tile::list.clear();
                }

                void run();
        };
    }
}

#endif
