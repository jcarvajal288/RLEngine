#ifndef RLNS_CHECKEDSAVE_HPP
#define RLNS_CHECKEDSAVE_HPP

// see warning disable rationale in lcrl.hpp
#ifdef _WIN32
#pragma warning( disable : 4482 )
#endif

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include "libtcod.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Class       : CheckedZip
        Description : Wrapper class for the libtcod TCODZip class.  CheckedZip doesn't
                      check whether what it put in and what it gets out is consistent,
                      leading to difficulty debugging save/load functions. CheckedZip
                      tags each input with a type enum, so a load function will know
                      if it gets an input of an unexpected type.
        Parents     : TCODZip (for functions that don't require checking)
        Children    : None
        Friends     : None
    --------------------------------------------------------------------------------*/
    class CheckedZip: public TCODZip
    {
        // Member Variables
        private:
            enum SaveType
            {
                CHAR = 0,
                INT = 1,
                FLOAT = 2,
                STRING = 3,
                COLOR = 4,
                IMAGE = 5,
                CONSOLE = 6,
                DATA = 7
            };

        // Member Functions
        private:
            std::string interpretTypeCode(const int) const;
            void error(const int, const int) const;

        public:
            CheckedZip(): TCODZip() {}

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

#endif
