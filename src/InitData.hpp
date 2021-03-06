#ifndef RLNS_INITDATA_HPP
#define RLNS_INITDATA_HPP

#include <boost/lexical_cast.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "Point.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Class       : InitData
        Description : Wrapper for an input stream that reads the metadata from init.txt.
                      Keeps track of the data read from that file. 
        Parents     : None
        Children    : None
        Friends     : None
    --------------------------------------------------------------------------------*/
    class InitData
    {
    // Member Variables
        private:
            std::ifstream input;

            int rootWidth;      // width of the main window in pixels
            int rootHeight;     // height of the main window in pixels
            int rootTileWidth;  // width of the main window in tiles
            int rootTileHeight; // height of the main window in tiles

            int gwWidth;        // width of the game window in pixels
            int gwHeight;       // height of the game window in pixels
            int gwTileWidth;    // width of the game window in tiles
            int gwTileHeight;   // height of the game window in tiles

            int commandLineHeight;    // height of the command line

            std::string font;   // which font family is being used for the game window.
            int fontWidth;      // width of the game font
            int fontHeight;     // height of the game font
            bool fontgs;        // whether the font is greyscale or not
            std::string fontLayout; // one of three values: 'as', 'ro', or 'tc'

            float mlgwRatio;    // size of the message log relative to the game window
            float pwgwRatio;    // size of the party window relative to the game window

            TCODColor UIForeColor;
            TCODColor UIBackColor;

            int logSize;        // how big the game's message log is

            TCOD_renderer_t renderer; // which of the three libtcod renderers are used.


    // Member Functions
        private:
            void error(const std::string&);
            std::string readNextValue();

        public:
            InitData();

            int getRootWidth()          const   { return rootWidth;      }
            int getRootHeight()         const   { return rootHeight;     }
            int getRootTileWidth()      const   { return rootTileWidth;  }
            int getRootTileHeight()     const   { return rootTileHeight; }

            int getGwWidth()            const   { return gwWidth;        }
            int getGwHeight()           const   { return gwHeight;       }
            int getGwTileWidth()        const   { return gwTileWidth;    }
            int getGwTileHeight()       const   { return gwTileHeight;   }

            int getCommandLineHeight()  const   { return commandLineHeight;    }

            std::string getFont()       const   { return font;           }
            int getFontWidth()          const   { return fontWidth;      }
            int getFontHeight()         const   { return fontHeight;     }
            bool getFontGS()            const   { return fontgs;         }
            std::string getFontLayout() const   { return fontLayout;     }

            float getMLGWRatio()        const   { return mlgwRatio;      }
            float getPWGWRatio()        const   { return pwgwRatio;      }

            int getLogSize()            const   { return logSize;        }

            void readInitFile(const char*);
            void initRoot();
    };
}

#endif
