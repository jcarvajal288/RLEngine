#ifndef RLNS_CONSOLE_HPP
#define RLNS_CONSOLE_HPP

#include <list>
#include <string>

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
            Class       : MessageTracker
            Description : Contains a list of formatted Message objects to be displayed on 
                          the Console.  Also provides an interface for easily adding
                          formatted messages.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class MessageTracker
        {
            // Member Variables
            private:
                std::string prompt;

                std::list<MessagePtr> messageLog;

                // Maximum size of the message log
                size_t logSize;     

                // Determines where in the message log the game will start printing
                //unsigned int start;

            // Member Functions
            public:
                MessageTracker(const int ls=100)
                : logSize(ls), start(0) {}

                void setPrompt(const std::string& p) 
                { prompt = p; }

                std::string getPrompt() const 
                { return prompt; }

                std::list<MessagePtr>::const_iterator begin() const
                { return messageLog.begin(); }

                std::list<MessagePtr>::const_iterator end() const
                { return messageLog.end(); }

                void addMessage(const std::string&, const TCODColor&, const TCODColor&);

                void addStdMessage(const std::string&);
        };


        // Inline Functions

        // adds a message formatted to the default light grey foreground and black background
        inline void MessageTracker::addStdMessage(const std::string& str)
        {
            addMessage(str, TCODColor::lighterGrey, TCODColor::black);
        }
    }
}

#endif
