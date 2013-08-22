#include "Tile.hpp"

using namespace std;

namespace rlns
{
    multimap<int, TilePtr> Tile::list;

    /*--------------------------------------------------------------------------------
        Function    : Tile::findTile
        Description : First checks to see if a tile with the given ID exists in the
                      Tile list, and if so, returns it.
        Inputs      : tile ID
        Outputs     : possible error message and abort
        Return      : TilePtr
    --------------------------------------------------------------------------------*/
    TilePtr Tile::findTile(const int i) 
    { 
        std::multimap<int, TilePtr>::iterator it = list.find(i);
        if(it == list.end()) // the tile doesn't exist, throw an error
        {
            char msg[30];
        #ifdef _WIN32
            sprintf_s(msg, "Tile ID %d doesn't exist!", i);
        #else
            sprintf(msg, "Tile ID %d doesn't exist!", i);
        #endif
            fatalError(msg);
        }
        return it->second;
    }



    /*--------------------------------------------------------------------------------
        Function    : Tile::signal
        Description : Interprets an action send to the tile, which may turn it into
                      another tile, or it may take some other action.
        Inputs      : type of action to take
        Outputs     : None
        Return      : int (the new tile that this one turns into)
    --------------------------------------------------------------------------------*/
    int Tile::signal(const TileActionType action)
    {
        int i = 0;

        try { i = actions.at(action); }
        catch(exception&) { return 0; }

        return i;
    }



    /*--------------------------------------------------------------------------------
        Function    : TileParser::TileListener::parserNewStruct
        Description : Called when the parser finds a new Tile struct.  This
                      function doesn't do any data reading; it just resets the flag
                      tracking variables.
        Inputs      : parser, parser struct, name of the struct
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool TileParser::TileListener::parserNewStruct(TCODParser* parser, const TCODParserStruct* str, const char* name)
    {
        // unused parameters
        (void) parser;
        (void) str;
        (void) name;
        
        // reset the tracking variables
        shortdesc = longdesc = "";
        fg = bg = TCODColor::fuchsia;  // fuchsia is used as the 'empty' color, since nothing will be that fabulous
        for(int i=0; i<NUM_TILE_ACTIONS; ++i)
        {
            actions[i] = 0; // 0 indicates no action
        }
        for(int i=0; i<NUM_TILE_FLAGS; ++i)
        {
            flags[i] = false;
        }

        return true; 
    }



    /*--------------------------------------------------------------------------------
        Function    : TileParser::TileListener::parserFlag
        Description : If the parser finds a flag defined in a struct, this assigns a 
                      true value to the appropriate tracking variable.
        Inputs      : parser, flag name
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool TileParser::TileListener::parserFlag(TCODParser* parser, const char* name)
    {
        if(strcmp(name, "blocksLight") == 0)
        {
            flags[BLOCKS_LIGHT] = true;
        }
        else if(strcmp(name, "blocksWalk") == 0)
        {
            flags[BLOCKS_WALK] = true;
        }
        else if(strcmp(name, "directionallyLinked") == 0)
        {
            flags[DIRECTIONALLY_LINKED] = true;
        }
        else if(strcmp(name, "multipleChars") == 0)
        {
            flags[MULTIPLE_CHARS] = true;
        }
        else if(strcmp(name, "notable") == 0)
        {
            flags[NOTABLE] = true;
        }
        else
        {
            parser->error("Unknown flag '%s' found.", name);
        }

        return true;
    }



    /*--------------------------------------------------------------------------------
        Function    : TileParser::TileListener::parserProperty
        Description : Called when the parser finds a property, this updates the
                      relevant tracking variable in TileListener.
        Inputs      : parser, property name, value type, value data
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool TileParser::TileListener::parserProperty
    (TCODParser* parser, const char* name, TCOD_value_type_t valtype, TCOD_value_t value)
    {
        // unused parameters
        (void) valtype;

        if(strcmp(name, "id") == 0)
        {
            id = value.i;
        }
        else if(strcmp(name, "charSet") == 0)
        {
            charSet = value.list;
        }
        else if(strcmp(name, "shortdesc") == 0)
        {
            shortdesc = string(value.s);
        }
        else if(strcmp(name, "longdesc") == 0)
        {
            longdesc = string(value.s);
        }
        else if(strcmp(name, "fgColor") == 0)
        {
            fg = value.col;
        }
        else if(strcmp(name, "bgColor") == 0)
        {
            bg = value.col;
        }
        else if(strcmp(name, "light") == 0)
        {
            light = string(value.s);
        }
        else if(strcmp(name, "onOpen") == 0)
        {
            actions[OPEN] = value.i;
        }
        else if(strcmp(name, "onToggle") == 0)
        {
            actions[TOGGLE] = value.i;
        }
        else
        {
            parser->error("Unknown property '%s' in struct '%s'", value.s, name);
            return false;
        }
        return true;
    }



    /*--------------------------------------------------------------------------------
        Function    : TileParser::TileListener::parserEndStruct
        Description : Called when the parser comes to the end of a feature structure,
                      this callback creates a new Tile object out of the previously
                      read data and adds it to the Tile list.  
        Inputs      : parser, tileset struct, name of the struct
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool TileParser::TileListener::parserEndStruct
    (TCODParser* parser, const TCODParserStruct* str, const char* name)
    {
        // unused parameters
        (void) parser;
        (void) str;
        (void) name;

        int count = 0;
        int* charSetEnd = charSet.end();
        for(int* it = charSet.begin(); it != charSetEnd; ++it, ++count)
        {
            int numChars= charSetEnd - charSet.begin();
            TilePtr newTile(new Tile(*it, numChars, fg, bg, shortdesc, longdesc, light, actions, flags));
            Tile::list.insert(pair<int, TilePtr>(id+count, newTile));
        }
        return true;
    }



    /*--------------------------------------------------------------------------------
        Function    : TileParser::TileListener::error
        Description : Called when the parser detects an error.  Prints the given error
                      message.  If this function is called, it will terminate the
                      program.
        Inputs      : error message
        Outputs     : error message
        Return      : void
    --------------------------------------------------------------------------------*/
    void TileParser::TileListener::error(const char* error)
    {
        cerr << "TILE PARSING ERROR: " << error << endl;
    }



    /*--------------------------------------------------------------------------------
        Function    : TileParser::verify
        Description : Testing function that prints out the contents of the Tile
                      list to verify that everything was read correctly.
        Inputs      : None
        Outputs     : contents of Tile list
        Return      : void
    --------------------------------------------------------------------------------*/
    void TileParser::verify() const
    {
        multimap<int, TilePtr>::const_iterator it, end;
        end = Tile::list.end();
        for(it=Tile::list.begin(); it!=end; ++it)
        {
            cout << "Tile ID: " << it->first << endl;
            cout << "char: " << it->second->getChar() << endl;
            cout << "FGColor: "; printColor(it->second->getFgColor());
            cout << "BGColor: "; printColor(it->second->getBgColor());
            cout << "shortdesc: " << it->second->shortDescription() << endl;
            cout << "longdesc: " << it->second->longDescription() << endl;
            cout << "dlinked?: " << it->second->isDirectionallyLinked() << endl;
            cout << "blocksLight?: " << it->second->blocksLight() << endl;
            cout << "blocksWalking?: " << it->second->blocksWalking() << endl;
            cout << endl;
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : TileParser::defineSyntax
        Description : defines the syntax for the tileset config file.  See 
                      datafiles/feature.txt for explanations of the syntax.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void TileParser::defineSyntax()
    {
        // define Tile structure
        TCODParserStruct TileStruct = *(parser.newStructure("Tile"));
        TileStruct.addProperty("id", TCOD_TYPE_INT, true);
        TileStruct.addListProperty("charSet", TCOD_TYPE_INT, true);
        TileStruct.addProperty("shortdesc", TCOD_TYPE_STRING, true);
        TileStruct.addProperty("longdesc", TCOD_TYPE_STRING, true);
        TileStruct.addProperty("bgColor", TCOD_TYPE_COLOR, false);
        TileStruct.addProperty("fgColor", TCOD_TYPE_COLOR, false);
        TileStruct.addFlag("blocksLight");
        TileStruct.addFlag("blocksWalk");
        TileStruct.addFlag("directionallyLinked");
        TileStruct.addFlag("multipleChars");
        TileStruct.addFlag("notable");

        // define feature attributes
        TileStruct.addProperty("light", TCOD_TYPE_STRING, false);


        // define feature actions
        TileStruct.addProperty("onOpen", TCOD_TYPE_INT, false);
        TileStruct.addProperty("onToggle", TCOD_TYPE_INT, false);
    }



    /*--------------------------------------------------------------------------------
        Function    : TileParser::run
        Description : Entry point into the Tile Parser.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void TileParser::run()
    {
        defineSyntax();
        parser.run(filename.c_str(), &listener);
        //verify();
    }
}
