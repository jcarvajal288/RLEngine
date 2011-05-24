#include "EventHandler.hpp"

using namespace std;
using namespace rlns::utl;

namespace rlns
{
    namespace control
    {
        EventHandlerPtr EventHandler::_instance(new EventHandler());

        /*--------------------------------------------------------------------------------
            Function    : EventHandler::useStairs
            Description : Checks if there's a stair on the current player's tile, and
                          if so, calls the necessary functions to create a new level and
                          attach it to the current one.
            Inputs      : stair character (either '>' for downstairs or '<' for upstairs)
            Outputs     : None
            Return      : EventType
        --------------------------------------------------------------------------------*/
        EventType EventHandler::useStairs(const char stairChar) const
        {
            Point pt = model::GameData::get()->getActivePlayer()->getPosition();
            if(model::GameData::get()->getCurrentLevel()->checkForTileChar(pt, stairChar))
            {
                if(stairChar == '>')
                {
                    view::Display::get()->setCommandPrompt("Going downstairs.");
                    model::GameData::get()->useStairs(pt, false);
                }
                else 
                {
                    view::Display::get()->setCommandPrompt("Going upstairs.");
                    model::GameData::get()->useStairs(pt, true);
                }
                return MOVE;
            }
            else
            {
                if(stairChar == '>')
                    view::Display::get()->setCommandPrompt("There is no way down here.");
                else
                    view::Display::get()->setCommandPrompt("There is no way up here.");
                return NO_EVENT;
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::movePlayer
            Description : Tells GameData to move the current player in the given direction.
            Inputs      : DirectionType
            Outputs     : None
            Return      : EventType (indicates success or failure)
        --------------------------------------------------------------------------------*/
        EventType EventHandler::movePlayer(const DirectionType dir) const
        {
            // get the active player's current position and shift it in the given direction
            Point destination = model::GameData::get()->getActivePlayer()->getPosition();
            destination.shift(dir);

            // check if the destination tile is walkable.  If so, move the player there.
            // If it is not, try to open a door in the destination tile.  If a door opens, 
            // indicate a MOVE; if not, NO_EVENT.
            if(model::GameData::get()->getCurrentLevel()->isWalkable(destination))
            {
                // walk successful
                model::GameData::get()->moveActivePlayer(dir);
                return MOVE;
            }
            else
            {
                // check for a door
                if(model::GameData::get()->getCurrentLevel()->signalTile(destination, OPEN))
                {
                    return MOVE;
                }
                else return NO_EVENT;
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::openCommand
            Description : Prompts the player to enter a direction and sends an OPEN signal
                          to the tile in that direction.
            Inputs      : None
            Outputs     : Prompts the player for a direction
            Return      : EventType
        --------------------------------------------------------------------------------*/
        EventType EventHandler::openCommand() const
        {
            DirectionType dir = promptDirection();
            if(dir == NO_DIRECTION)
            {
                view::Display::get()->setCommandPrompt("Okay, then.");
                view::Renderer::get()->refresh();
                return NO_EVENT;
            }
            else 
            {
                Point pt = model::GameData::get()->getActivePlayer()->getPosition();
                if(model::GameData::get()->getCurrentLevel()->signalTile(pt.neighbor(dir), OPEN))
                {
                    return MOVE;
                }
                else
                {
                    view::Display::get()->setCommandPrompt("There's nothing openable there.");
                    view::Renderer::get()->refresh();
                    return NO_EVENT;
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::promptDirection
            Description : prompts the player to press a direction key.
            Inputs      : None
            Outputs     : prompt to press a direction key.
            Return      : DirectionType
        --------------------------------------------------------------------------------*/
        DirectionType EventHandler::promptDirection() const
        {
            view::Display::get()->setCommandPrompt("Which direction? (Space to cancel)");
            view::Renderer::get()->refresh();

            TCOD_key_t key;
            do
            {
                key = getKeypress();
                if(key.vk == TCODK_SPACE) 
                {
                    return NO_DIRECTION;
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
            Return      : DirectionType
        --------------------------------------------------------------------------------*/
        DirectionType EventHandler::getDirection(const TCOD_key_t key) const
        {
            switch(key.vk)
            {
                case TCODK_CHAR:
                {
                    switch(key.c)
                    {
                        // Letter Movement Commands (uiojlm,.)
                        case 'i':
                            return NORTH;
                        case 'o':
                            return NORTHEAST;
                        case 'l':
                            return EAST;
                        case '.':
                            return SOUTHEAST;
                        case ',':
                            return SOUTH;
                        case 'm':
                            return SOUTHWEST;
                        case 'j':
                            return WEST;
                        case 'u':
                            return NORTHWEST;
                        case 'k':
                            return CENTER;
                        default:
                            return NO_DIRECTION;
                    }
                }
                case TCODK_UP:
                case TCODK_KP8:
                    return NORTH;

                case TCODK_KP9:
                    return NORTHEAST;

                case TCODK_RIGHT:
                case TCODK_KP6:
                    return EAST;

                case TCODK_KP3:
                    return SOUTHEAST;

                case TCODK_DOWN:
                case TCODK_KP2:
                    return SOUTH;

                case TCODK_KP1:
                    return SOUTHWEST; 

                case TCODK_LEFT:
                case TCODK_KP4:
                    return WEST;

                case TCODK_KP7:
                    return NORTHWEST;

                case TCODK_KP5:
                    return CENTER; 

                default:
                    return NO_DIRECTION;
            }

        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::getKeypress
            Description : Depending on various flags, this function either waits for a key
                          to be pressed or it returns a TCODK_NONE indicating no key has
                          been pressed.
            Inputs      : None
            Outputs     : None
            Return      : TCOD_key_t
        --------------------------------------------------------------------------------*/
        TCOD_key_t EventHandler::getKeypress() const
        {
            if(LIGHT_FLICKER_ENABLED)
            {
                // this allows the screen to update while waiting for the player
                // to press a key.
                return TCODConsole::checkForKeypress();
            }
            else
            {
                return TCODConsole::waitForKeypress(true);
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::getPlayerInput
            Description : Waits until the player presses a key, and then interprets that
                          keystroke and starts the proper events in the game.
            Inputs      : None
            Outputs     : None
            Return      : EventType
        --------------------------------------------------------------------------------*/
        EventType EventHandler::getPlayerInput() const
        {
            TCOD_key_t key;
            EventType event;
            key = getKeypress();

            if(key.vk == TCODK_NONE) return EventType::NO_EVENT; // no key pressed

            event = systemCommands(key);
            if(event != NO_EVENT) return event;
            
            DirectionType dir = getDirection(key);
            if(dir != NO_DIRECTION) return movePlayer(dir);

            event = letterCommands(key.c);
            if(event != NO_EVENT) return event;

            // if control gets here, no valid key was pressed
            return NO_EVENT; // Possibly this could prompt for the help command?
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
            switch(key.vk)
            {
                case TCODK_ESCAPE:
                    return EXIT;
                default:
                    return NO_EVENT;
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::letterCommands
            Description : Checks whether the input key is a printable character.
            Inputs      : key code
            Outputs     : None
            Return      : EventType
        --------------------------------------------------------------------------------*/
        EventType EventHandler::letterCommands(const char c) const
        {
            switch(c)
            {
                case 'c':
                   return openCommand(); 
                case '<':
                   return useStairs('<');
                case '>': 
                   return useStairs('>');
                default:
                   return NO_EVENT;
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::gameLoop
            Description : Main event loop of the game.  Once this terminates, the game
                          exits.
            Inputs      : None
            Outputs     : Most of the game.
            Return      : void
        --------------------------------------------------------------------------------*/
        void EventHandler::gameLoop() const
        {
            EventType event;
            do
            {
                // refresh the screen
                view::Renderer::get()->refresh(); 

                // poll the player until they enter a valid action
                do { event = getPlayerInput(); }
                while(event == NO_EVENT);
            }
            while(event != EXIT);
        }



        /*--------------------------------------------------------------------------------
            Function    : EventHandler::get
            Description : Public access function for the private EventHandler instance.
            Inputs      : None
            Outputs     : None
            Return      : EventHandlerPtr
        --------------------------------------------------------------------------------*/
        EventHandlerPtr EventHandler::get()
        {
            if(_instance == 0)
            {
                _instance.reset(new EventHandler());
            }
            return _instance;
        }
    }
}
