#ifndef RLNS_CHECKEDSAVE_HPP
#define RLNS_CHECKEDSAVE_HPP

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "libtcod.hpp"

namespace rlns
{
    namespace utl
    {
        /*--------------------------------------------------------------------------------
            Class       : RLNSZip
            Description : Wrapper class for the libtcod RLNSZip class.  RLNSZip doesn't
                          check whether what it put in and what it gets out is consistent,
                          leading to difficulty debugging save/load functions. RLNSZip
                          tags each input with a type enum, so a load function will know
                          if it gets an input of an unexpected type.
            Parents     : TCODZip (for functions that don't require checking)
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class RLNSZip: public TCODZip
        {
            // Member Variables
            private:
                enum SaveType
                {
                    CHAR,
                    INT,
                    FLOAT,
                    STRING,
                    COLOR,
                    IMAGE,
                    CONSOLE,
                    DATA
                };

            // Member Functions
            public:
                RLNSZip(): TCODZip() {}

                // Input Functions
                char getChar();
                int  getInt();
                float getFloat();
                const char* getString();
                TCODColor getColor();
                TCODImage* getImage();
                TCODConsole* getConsole();
                int getData(int, void*);

                // Output Functions
                void putChar(char);
                void putInt(int);
                void putFloat(float);
                void putString(const char*);
                void putColor(const TCODColor*);
                void putImage(const TCODImage*);
                void putConsole(const TCODConsole*);
                void putData(int, const void*);
        };
    }
}

#endif
