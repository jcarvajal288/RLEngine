#include <utility>

#include "Actor.hpp"
#include "CheckedSave.hpp"
#include "EventHandler.hpp"
#include "Display.hpp"
#include "GameData.hpp"
#include "InitData.hpp"
#include "Light.hpp"
#include "Point.hpp"
#include "Renderer.hpp"
#include "Tile.hpp"
#include "Tileset.hpp"

#include "libtcod.hpp"

using namespace rlns;
using namespace rlns::utl;
using namespace std;

/*--------------------------------------------------------------------------------
    Function    : readDataFiles
    Description : Creates and runs the parsers for the datafiles.
    Inputs      : None
    Outputs     : None
    Return      : void
--------------------------------------------------------------------------------*/
void readDataFiles()
{
    model::TileParser tileParser("datafiles/tiles.txt");
    model::TilesetParser tilesetParser("datafiles/tileset.txt");
    model::LightParser lightParser("datafiles/lights.txt");
    tileParser.run();
    tilesetParser.run();
    lightParser.run();
}



/*--------------------------------------------------------------------------------
    Function    : initializeDisplay
    Description : Reads init.txt and constructs the Display Window
    Inputs      : None
    Outputs     : None
    Return      : void
--------------------------------------------------------------------------------*/
void initializeDisplay()
{
    InitData::get()->readInitFile();
    InitData::get()->initRoot();
}



/*--------------------------------------------------------------------------------
    Function    : loadGame
    Description : Loads the game state from a save file.
    Inputs      : name of the saved game
    Outputs     : None
    Return      : void
--------------------------------------------------------------------------------*/
void loadGame(const string& saveGame)
{
    RLNSZip zip;
    zip.loadFromFile(saveGame.c_str());
    model::GameData::get()->loadFromDisk(zip);
}



/*--------------------------------------------------------------------------------
    Function    : checkSave
    Description : Checks if the given save file exists.  If so, it loads the game
                  state from that save file.  If not, it creates a new game state
                  from scratch.
    Inputs      : None
    Outputs     : None
    Return      : void
--------------------------------------------------------------------------------*/
void checkSave()
{
    string saveDir = "./save/";
    if(!TCODSystem::isDirectory(saveDir.c_str()))
        TCODSystem::createDirectory(saveDir.c_str());

    string saveFile = saveDir + "gameName.sav";
    // load saved game if save file exists.  Create new one otherwise.
    FILE* fileExists = fopen(saveFile.c_str(), "r");
    if(fileExists)
    {
        fclose(fileExists);
        loadGame(saveFile);
    }
    else
    {
        model::GameData::get()->createNewGameState();

        // add a torch to the current focal point
        int playerLight = model::Light::createLight("Torch");
        model::GameData::get()->getActivePlayer()->attachLight(playerLight);
    }
}



/*--------------------------------------------------------------------------------
    Function    : saveAndQuit
    Description : saves the game to an external file and exits the game.
    Inputs      : None
    Outputs     : a save file in the ./save folder
    Return      : void
--------------------------------------------------------------------------------*/
void saveAndQuit()
{
    RLNSZip zip;
    model::GameData::get()->saveToDisk(zip);
    string filename = "./save/gameName.sav"; // TODO: replace this with name of character
    zip.saveToFile(filename.c_str());
    exit(0);
}



int main()
{
    readDataFiles();
    initializeDisplay();
    checkSave();
    control::EventHandler::get()->gameLoop();
    saveAndQuit();
}
