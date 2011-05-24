#ifndef FILEPARSER_HPP
#define FILEPARSER_HPP

#include "libtcod.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Class       : FileParser
        Description : Defines an interface for all file parsers
        Parents     : None
        Children    : CreatureParser, ItemParser 
        Friends     : None
    --------------------------------------------------------------------------------*/
    class FileParser
    {
    // Member Variables
        protected:
            std::string filename;
            TCODParser parser;
    
    // Member Functions'
        protected:
            virtual void defineSyntax() = 0;

        public:
            FileParser(const std::string& n): filename(n) {}

            virtual ~FileParser() {}

            virtual void run() = 0;
    };
}

#endif
