#include "Events.hpp"

using namespace std;

namespace rlns
{
    using namespace model;
    using namespace utl;
    using namespace view;

    namespace control
    {

        /*--------------------------------------------------------------------------------
            Function    : interpretCommandString
            Description : Executes a command specified on the command line
            Inputs      : DisplayPtr
            Outputs     : command prompt
            Return      : string
        --------------------------------------------------------------------------------*/
		static void interpretCommandString(const string& command, const DisplayPtr display)
		{
			if(command.empty()) return;

			// Placeholder 'interpret' code that just prints the command to the message log.
            display->messageTracker->addStdMessage(command);
            display->messageTracker->setPrompt("");
		}



        /*--------------------------------------------------------------------------------
            Function    : getTextInput
            Description : Prints a colon prompt at the command line and lets the user
                          input a command, then returns the command as a text string.
            Inputs      : DisplayPtr
            Outputs     : command prompt
            Return      : string
        --------------------------------------------------------------------------------*/
        static string getTextInput(const DisplayPtr display)
        {
            TCOD_key_t key;
            string prompt(":");
            string commandString("");

            while(true)
            {
                display->messageTracker->setPrompt(prompt + commandString);
                display->refresh();
                TCODConsole::flush();

                key = getKeypress();

                // pressing enter finishes the command
                if(key.vk == TCODK_ENTER) 
                    break;
                else if(key.vk == TCODK_BACKSPACE)
                {
                    if(commandString.empty()) break;
					else
                    {
                        commandString.erase(commandString.length()-2, commandString.length());
                        continue;
                    }
                }

                commandString.append(&key.c);
            } 
            
            return commandString;
        }



        /*--------------------------------------------------------------------------------
            Function    : movePlayer
            Description : Moves the current player in the given direction.
            Inputs      : DirectionType
            Outputs     : None
            Return      : bool (indicates successful or failed move)
        --------------------------------------------------------------------------------*/
        static bool movePlayer(const DirectionType dir, const view::DisplayPtr display) 
        {
            model::LevelPtr currentLevel = model::Level::getCurrentLevel();
            model::ActorPtr player = model::Party::getPlayerParty()->getLeader();

            Point destination = player->getPosition();
            destination.shift(dir);

            if(currentLevel->moveLegal(destination, player->getMovementType()))
            {
                player->setPosition(destination);
                display->setFocalPoint(destination);
                display->inspectTile(currentLevel, destination);
                return true;
            }
            else
            {
                return currentLevel->signalTile(destination, OPEN) != 0;
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : pickUpItem()
            Description : Queries the current level for any items in the active
                          character's tile.  If it finds any, it adds them to the
                          character's inventory and passes the turn.  If not, it prints
                          a message to that effect.
            Inputs      : Pointer to the display
            Outputs     : Message on whether items were picked up.
            Return      : bool (whether items were picked up)
        --------------------------------------------------------------------------------*/
        static bool pickUpItem(const view::DisplayPtr display) 
        {
            LevelPtr currentLevel = model::Level::getCurrentLevel();
            ActorPtr player = model::Party::getPlayerParty()->getLeader();
            Point location = player->getPosition();

            vector<ItemPtr> itemsAtLocation = currentLevel->fetchItemsAtLocation(location);

            if(itemsAtLocation.empty())
            {
                display->messageTracker->setPrompt("There are no items here.");
                return false;
            }

            // TODO: in the future, if there are more than one items at the location,
            // prompt the player whether he wants to add each item.
            vector<ItemPtr>::const_iterator it, end;
            it = itemsAtLocation.begin(); end = itemsAtLocation.end();
            for(; it!=end; ++it)
            {
                player->giveItem(*it);
                display->messageTracker->addStdMessage("Picked up " + (*it)->shortDescription() + ".");
            }

            return true;
        }



        /*--------------------------------------------------------------------------------
            Function    : showInventory
            Description : Creates an InventoryScreen object and starts its event loop.
            Inputs      : DisplayPtr
            Outputs     : inventory screen
            Return      : bool (whether the action performed in the inventory screen costs
                          a turn)
        --------------------------------------------------------------------------------*/
        bool showInventory(const view::DisplayPtr display)
        {
            InventoryScreen invScreen(display, Party::getPlayerParty()->getLeader());
            return invScreen.eventLoop();
        }



        /*--------------------------------------------------------------------------------
            Function    : mainEventContext
            Description : Handles events for the main mode of the game: moving the party
                          around the playfield.
            Inputs      : EventType
            Outputs     : Changes to the game state
            Return      : bool (whether the action costs a turn)
        --------------------------------------------------------------------------------*/
        bool mainEventContext(const EventType event, const view::DisplayPtr display)
        {
            // clear the command prompt, since it should only display during the turn
            // it is activated
            display->messageTracker->setPrompt("");

            switch(event)
            {
                case COMMAND:
                {
                    string command = getTextInput(display);
					interpretCommandString(command, display);
                    return false;
                }
                case EXIT:
                {
                    IS_RUNNING = false;
                    return true;
                }
                case MOVE_NORTH:
                {
                    return movePlayer(DirectionType::NORTH, display);
                }
                case MOVE_NORTHEAST:
                {
                    return movePlayer(DirectionType::NORTHEAST, display);
                }
                case MOVE_EAST:
                {
                    return movePlayer(DirectionType::EAST, display);
                }
                case MOVE_SOUTHEAST:
                {
                    return movePlayer(DirectionType::SOUTHEAST, display);
                }
                case MOVE_SOUTH:
                {
                    return movePlayer(DirectionType::SOUTH, display);
                }
                case MOVE_SOUTHWEST:
                {
                    return movePlayer(DirectionType::SOUTHWEST, display);
                }
                case MOVE_WEST:
                {
                    return movePlayer(DirectionType::WEST, display);
                }
                case MOVE_NORTHWEST:
                {
                    return movePlayer(DirectionType::NORTHWEST, display);
                }
                case ITEM:
                {
                    return pickUpItem(display);
                }
                case EQUIPMENT:
                {
                    return showInventory(display);
                }
                default: return false;
            }
        }
    }
}
