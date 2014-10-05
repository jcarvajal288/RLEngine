#include "lcrl.hpp"

using namespace std;

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : LCRL::initialize
        Description : Run on game start, this starts InitData reading init.txt and
                      loads any saved games.
        Inputs      : None
        Outputs     : None
        Return      : bool (whether initialization is successful)
    --------------------------------------------------------------------------------*/
    bool LCRL::initialize()
    {
    #ifdef _WIN32
        initData.readInitFile(".\\init.txt");
    #else
        initData.readInitFile("./init.txt");
    #endif
        initData.initRoot();
        display.reset(new Display(initData));

        // run file parsers
    #ifdef _WIN32
        TileParser tileParser(".\\datafiles\\tiles.txt");
        TilesetParser tilesetParser(".\\datafiles\\tileset.txt");
    #else
        TileParser tileParser("./datafiles/tiles.txt");
        TilesetParser tilesetParser("./datafiles/tileset.txt");
        CreatureParser creatureParser("./datafiles/creatures.txt");
    #endif
        tileParser.run();
        tilesetParser.run();
        creatureParser.run();

        // create the first level
        Level::addLevel("Castle");

        // add the player to the party
        Point pos = Level::getCurrentLevel()->getUpStairLocation();
        ActorPtr player(new Actor(pos, '@', TCODColor::white));
        Party::getPlayerParty()->addMember(player);
        Level::getCurrentLevel()->addParty(Party::getPlayerParty());

        display->setFocalPoint(player->getPosition());
        return true;
    }



    /*--------------------------------------------------------------------------------
        Function    : LCRL::gameLoop
        Description : Starts the main game loop.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void LCRL::gameLoop()
    {
        EventHandler eventHandler;

        while(IS_RUNNING && !TCODConsole::isWindowClosed())
        {
            render(display);
            EventType event = eventHandler.getPlayerInput();
            mainEventContext(event, display);
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : LCRL::render
        Description : Starts the screen refresh process.
        Inputs      : pointer to the display object
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void LCRL::render(const DisplayPtr display) const
    {
        display->refresh();
        TCODConsole::flush();
    }



    /*--------------------------------------------------------------------------------
        Function    : LCRL::cleanup
        Description : Called once the player decides to exit the game.  Saves the
                      player's game and releases all game resources.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void LCRL::cleanup()
    {
        //cout << "cleanup" << endl;
    }



    /*--------------------------------------------------------------------------------
        Function    : LCRL::run
        Description : Starts the game process and calls all of the other game
                      functions.
        Inputs      : None
        Outputs     : None
        Return      : int (error code)
    --------------------------------------------------------------------------------*/
    int LCRL::run()
    {
        initialize();
        gameLoop();
        cleanup();
        return 0;
    }
}



int main()
{
    rlns::LCRL game;
    return game.run();
}
