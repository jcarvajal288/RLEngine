#ifndef RLNS_CONSOLE_HPP
#define RLNS_CONSOLE_HPP

#include <list>
#include <string>

#include "DisplayWindow.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace view
    {
        /*--------------------------------------------------------------------------------
            Class       : Message
            Description : Wrapper for a text string and its foreground and background
                          colors for display in Console.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Message
        {
            // Member Variables
            private:
                std::string text;
                TCODColor foreColor;
                TCODColor backColor;

            // Member Functions
            public:
                Message(const std::string& t, 
                        const TCODColor& fc = TCODColor::lighterGrey,
                        const TCODColor& bc = TCODColor::black)
                : text(t), foreColor(fc), backColor(bc) {}

                std::string getText() const { return text; }
                TCODColor getForeColor() const { return foreColor; }
                TCODColor getBackColor() const { return backColor; }
        };



        /*--------------------------------------------------------------------------------  
            Class       : Console
            Description : Displays messages from the game to the player.  This class is a
                          member of the Singleton Display class, and so messages can be
                          put to the Console by calling the various printToConsole
                          methods.
            Parents     : DisplayWindow
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Console: public DisplayWindow
        {
            // Member Variables
            private:
                std::list<MessagePtr> messageLog;
                size_t logSize;                 // maximum size of the message log (determined in init.txt)
                int start;                      // determines where in the messageLog the game will start
                                                // printing to the screen.

            // Member Functions
            private:
                void addMessage(const std::string&, const TCODColor&, const TCODColor&);

            public:
                Console(const utl::Point& tl, const utl::Point& br, const int ls=100)
                : DisplayWindow(tl, br), logSize(ls), start(0) {}

                void printStdMessage(const std::string&);

                void draw() const;
        };


        // inline functions

        // puts a standard message into messagelog: lighter grey on black
        inline void Console::printStdMessage(const std::string& str)
        {
            addMessage(str, TCODColor::lighterGrey, TCODColor::black);
        }
    }
}

#endif
