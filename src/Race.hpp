#ifndef RLNS_RACE_HPP
#define RLNS_RACE_HPP

#include <string>
#include <vector>

#include "FileParser.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
	namespace model
	{
        /*--------------------------------------------------------------------------------
            Class       : Race
            Description : Contains all of the game information for a race of creatures.
						  All of this information is static; it is the characteristics of
						  a race and will not change.  Mutable information (such as hit
						  points, etc.) are stored in the Actor class.
            Parents     : None
            Children    : None
            Friends     : RaceParser
        --------------------------------------------------------------------------------*/
		class Race
		{
			// Member Variables
			private:
				std::string name;
				int ascii;
				TCODColor color;

				// More to come...

                static std::vector<RacePtr> list;

			// Member Functions
			public:
				Race(const std::string& n, const int a, const TCODColor& col)
				: name(n), ascii(a), color(col) {}

				std::string getName() const
				{ return name; }

				int getChar() const
				{ return ascii; }

				TCODColor getColor() const
				{ return color; }

                friend class RaceParser;
		};



        /*--------------------------------------------------------------------------------
            Class       : RaceParser
            Description : Reads in the race entries from the creatures.txt datafile.
            Parents     : FileParser
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class RaceParser: public FileParser
        {
            // Member Variables
            private:
                /*--------------------------------------------------------------------------------
                    Class       : RaceListener
                    Description : Constructs and executes a parser to read the datafile
                                  creatures.txt
                    Parents     : ITCODParserListener (see libtcod file parser documentation)
                    Children    : None
                    Friends     : None
                --------------------------------------------------------------------------------*/
                class RaceListener: public ITCODParserListener
                {
                    // Member Variables
                    private:
                        std::string name;
                        int ascii;
                        TCODColor color;

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
                RaceParser(const std::string& n)
				: FileParser(n)
                {
                    Race::list.clear();
                }

                void run();
        };
	}
}

#endif
