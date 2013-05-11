#ifndef RLNS_TILE_HPP
#define RLNS_TILE_HPP

#include <array>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#include "FileParser.hpp"
#include "AbstractTile.hpp"
#include "Point.hpp"
#include "Types.hpp"
#include "Utility.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : Tile
            Description : Class representing various map features, such as terrain, 
                          stairways and doors.  Tiles sit in one place and some may be 
                          interacted with.
            Parents     : AbstractTile
            Children    : UpStair, DownStair, Door
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Tile: public AbstractTile
        {
            // Member Variables
            private:
                int numChars;
                std::string light;
                std::array<int, NUM_TILE_ACTIONS> actions;
                std::array<bool, NUM_TILE_FLAGS> flags;

            public:
                static std::multimap<int, TilePtr> list;
            
            // Member Functions   
            public:
                Tile(const int ch, 
                     const int nc,
                     const TCODColor fg, 
                     const TCODColor bg,
                     const std::string& sd,
                     const std::string& ld,
                     const std::string& lt,
                     const std::array<int, NUM_TILE_ACTIONS> a,
                     const std::array<bool,NUM_TILE_FLAGS> f)
                : AbstractTile(ch, fg, bg, sd, ld), numChars(nc),
                  light(lt), actions(a), flags(f) {}

                int getNumChars()            const { return numChars; }
                bool blocksLight()           const { return flags[BLOCKS_LIGHT]; }
                bool blocksWalking()         const { return flags[BLOCKS_WALK]; }
                bool isDirectionallyLinked() const { return flags[DIRECTIONALLY_LINKED]; }
                bool hasMultipleChars()      const { return flags[MULTIPLE_CHARS]; }
                bool isNotable()             const { return flags[NOTABLE]; }
                std::string getLight()       const { return light; }

                static bool tileExists(const int i);
                static TilePtr findTile(const int i);

                int signal(const TileActionType);
        };


        // inline functions
        inline bool Tile::tileExists(const int i) 
        { 
            return list.find(i) != list.end(); 
        }



        /*--------------------------------------------------------------------------------
            Class       : TileParser
            Description : Reads in data from the datafile feature.txt and stores it in
                          Tile's list variable.
            Parents     : FileParser
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class TileParser: public FileParser
        {
            // Member Variables
            private:
                /*--------------------------------------------------------------------------------
                    Class       : TileListener
                    Description : Constructs and executes a parser to read the data file
                                  tiles.txt.
                    Parents     : ITCODParserListener (see libtcod file parser documentation)
                    Children    : None
                    Friends     : None
                --------------------------------------------------------------------------------*/
                class TileListener: public ITCODParserListener
                {
                    // Member Variables
                    private:
                        int id;
                        TCODList<int> charSet;
                        std::string shortdesc, longdesc, light;
                        TCODColor fg, bg;
                        std::array<int, NUM_TILE_ACTIONS> actions;
                        std::array<bool, NUM_TILE_FLAGS> flags;

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
                void verify() const;

            protected:
                void defineSyntax();

            public:
                TileParser(const std::string& n): FileParser(n)
                {
                    Tile::list.clear();
                }

                void run();
        };
    }
}

#endif
