#include "EventHandler.hpp"

using namespace std;
using namespace rlns::utl;
using namespace rlns::model;

namespace rlns
{
    namespace control
    {
        /*--------------------------------------------------------------------------------
            Function    : EventHandler::promptDirection
            Description : prompts the player to press a direction key.
            Inputs      : None
            Outputs     : prompt to press a direction key.
            Return      : EventType
        --------------------------------------------------------------------------------*/
        EventType EventHandler::promptDirection() const
        {
            //display->setCommandPrompt("Which direction? (Space to cancel)");
            //view::Renderer::get()->refresh();

            TCOD_key_t key;
            do
            {
                key = getKeypress();
                if(key.vk == TCODK_SPACE) 
                {
                    return CANCEL;
                }
            }
            while(key.vk == TCODK_NONE);

            return getDirection(key);
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::getDirection
            Description : Checks if the player input belongs to one of the directional
                          commands.
                          There are three different movement options for players.  The
                          recommended one is using the keypad numbers to indicate 
                          direction. The arrow keys can be used, although they cannot
                          represent directonal or no movement. The roguelike keys
                          ('uiojklm,.') can be used similarly to the numeric keypad.
            Inputs      : key pressed
            Outputs     : None
            Return      : EventType
        --------------------------------------------------------------------------------*/
        EventType EventHandler::getDirection(const TCOD_key_t key) const
        {
            if(!key.lalt && !key.lctrl && !key.shift)
            {
                switch(key.vk)
                {
                    case TCODK_UP:
                    case TCODK_KP8:
                        return MOVE_NORTH;

                    case TCODK_KP9:
                        return MOVE_NORTHEAST;

                    case TCODK_RIGHT:
                    case TCODK_KP6:
                        return MOVE_EAST;

                    case TCODK_KP3:
                        return MOVE_SOUTHEAST;

                    case TCODK_DOWN:
                    case TCODK_KP2:
                        return MOVE_SOUTH;

                    case TCODK_KP1:
                        return MOVE_SOUTHWEST; 

                    case TCODK_LEFT:
                    case TCODK_KP4:
                        return MOVE_WEST;

                    case TCODK_KP7:
                        return MOVE_NORTHWEST;

                    case TCODK_KP5:
                        return MOVE_CENTER; 

                    default:
                        return NO_EVENT;
                }
            }
            else return NO_EVENT;
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::systemCommands
            Description : Checks if the player input belongs to one of the system commands.
            Inputs      : key pressed
            Outputs     : None
            Return      : EventType
        --------------------------------------------------------------------------------*/
        EventType EventHandler::systemCommands(const TCOD_key_t key) const
        {
            if(!key.lalt && !key.lctrl && !key.shift)
            {
                switch(key.vk)
                {
                    case TCODK_ESCAPE:
                        return EXIT;
                    case TCODK_SPACE:
                    case TCODK_DELETE:
                        return CANCEL;
                    default:
                        return NO_EVENT;
                }
            }
            else return NO_EVENT;
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::letterCommands
            Description : Checks whether the input key is a printable character.
            Inputs      : key code
            Outputs     : None
            Return      : EventType
        --------------------------------------------------------------------------------*/
        EventType EventHandler::letterCommands(const TCOD_key_t key) const
        {
            if(!key.lalt && !key.lctrl)
            {
                switch(key.c)
                {
                // Letter Movement Commands (uiojlm,.)
                    case 'i':
                        return MOVE_NORTH;
                    case 'o':
                        return MOVE_NORTHEAST;
                    case 'l':
                        return MOVE_EAST;
                    case '.':
                        return MOVE_SOUTHEAST;
                    case ',':
                        return MOVE_SOUTH;
                    case 'm':
                        return MOVE_SOUTHWEST;
                    case 'j':
                        return MOVE_WEST;
                    case 'u':
                        return MOVE_NORTHWEST;
                    case 'k':
                        return MOVE_CENTER;
                // Equipment Commands
                    case 'd':
                        return ITEM;
                    case 'e':
                        return EQUIPMENT;
                    default:
                        return NO_EVENT;
                // Command Prompt
                    case ':':
                        return COMMAND;
                }
            }
            else return NO_EVENT;
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::keyToEvent
            Description : Translates a pressed key into an appropriate event.
            Inputs      : keycode
            Outputs     : None
            Return      : EventType
        --------------------------------------------------------------------------------*/
        EventType EventHandler::keyToEvent(const TCOD_key_t key) const
        {
            EventType event;

            if(key.vk == TCODK_NONE) return EventType::NO_EVENT; // no key pressed

            event = systemCommands(key);
            if(event != NO_EVENT) return event;
            
            // check for directional movement
            event = getDirection(key);
            if(event != NO_EVENT) return event;

            event = letterCommands(key);
            if(event != NO_EVENT) return event;

            // if control gets here, no valid key was pressed
            return NO_EVENT; // Possibly this could prompt for the help command?
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::getPlayerInput
            Description : Updates the game state in response to the player pressing a key.
            Inputs      : None
            Outputs     : updated game state
            Return      : EventType
        --------------------------------------------------------------------------------*/
        EventType EventHandler::getPlayerInput() const
        {
            TCOD_key_t key = getKeypress();
            return keyToEvent(key);
        }
    }
}
