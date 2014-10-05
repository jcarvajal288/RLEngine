#ifndef RLE_CREATURE_HPP
#define RLE_CREATURE_HPP

#include <string>
#include <vector>

#include "FileParser.hpp"
#include "MapObject.hpp"
#include "Types.hpp"
#include "Utility.hpp"

#include "libtcod.hpp"

namespace rlns
{
    struct temp_creature // used for the parser
    {
        std::string name;
        int character;
        TCODColor fgColor;
        int hp;
        std::string shortdesc;
        std::string longdesc;
    };

    /*--------------------------------------------------------------------------------
        Class       : Creature
        Description : defines all NPCs in the game
        Parents     :
        Children    :
        Friends     :
    --------------------------------------------------------------------------------*/
    class Creature: public MapObject
    {
        // Member Variables
        public:
            // this is a list of all creature types in the game, not all
            // unique creatures.
            static std::vector<CreaturePtr> list;

        private:
            std::string name;
            int hp;

        // Member Functions
        public:
            Creature(const temp_creature& t)
            : MapObject(t.character, t.fgColor, TCODColor::fuchsia, t.shortdesc, t.longdesc, Point(0,0)), name(t.name) {}

            std::string getName() const { return name; }

            int getHP() const { return hp; }
            void setHP(const int _hp) { hp = _hp; }

            static CreaturePtr findCreature(const std::string&);

        private:
            static int findCreatureIndex(const std::string&);
    };


    /*--------------------------------------------------------------------------------
        Class       : CreatureParser
        Description : Reads in data from the datafile creature.txt and stores it in
                      Creature's list variable.
        Parents     : FileParser
        Children    : None
        Friends     : None
    --------------------------------------------------------------------------------*/
    class CreatureParser: public FileParser
    {
        // Member Variables
        private:
            /*--------------------------------------------------------------------------------
                Class       : CreatureListener
                Description : Constructs and executes a parser to read the datafile 
                              creature.txt.
                Parents     : ITCODParserListener (see libtcod file parser documentation)
                Children    : None
                Friends     : None
            --------------------------------------------------------------------------------*/
            class CreatureListener: public ITCODParserListener
            {
                // Member Variables
                private:
                    temp_creature temp;

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
            CreatureParser(const std::string& n): FileParser(n)
            {
                Creature::list.clear();
            }

            void run();
    };
}

#endif
