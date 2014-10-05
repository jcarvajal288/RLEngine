#include "Creature.hpp"

using namespace std;

namespace rlns
{
    vector<CreaturePtr> Creature::list;

    /*--------------------------------------------------------------------------------
        Function    : Creature::findCreatureIndex
        Description : Searches the Creature list for a creature with the given name
                      and returns the integer index of that creature, or -1 if the name
                      is not found.  This is a private helper function for
                      getCreature().
        Inputs      : name of a creature
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    int Creature::findCreatureIndex(const string& creatureName)
    {
        int end = Creature::list.size();
        for(int i=0; i < end; ++i)
        {
            if(Creature::list[i]->name == creatureName)
                return i;
        }
        // creature name not found
        return -1;
    }

    /*--------------------------------------------------------------------------------
        Function    : Creature::findCreature
        Description : Finds the creature in Creature::list with the given name
        Inputs      : Name of the desired creature
        Outputs     : None
        Return      : CreaturePtr
    --------------------------------------------------------------------------------*/
    CreaturePtr Creature::findCreature(const string& creatureName)
    {
        int index = findCreatureIndex(creatureName);

        if(index < 0) // if the creature wasn't found, index will be -1
        {
            fatalError("Creature '" + creatureName + "' not found!");
        }

        return Creature::list.at(index);
    }

    /*--------------------------------------------------------------------------------
        Function    : CreatureParser::CreatureListener::parserNewStruct
        Description : Called when the parser finds a new Creature struct.  This
                      function doesn't do any data reading; it just resets the flag
                      tracking variables.
        Inputs      : parser, parser struct, name of the struct
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool CreatureParser::CreatureListener::parserNewStruct(TCODParser* parser, const TCODParserStruct* str, const char* name)
    {
        // unused parameters
        (void) parser;

//        string strName(str->getName());
//        if(strName == "Dungeon")
//        {
//            temp.type = MapType::DUNGEON;
//        }
//        else if(strName == "Cave")
//        {
//            temp.type = MapType::CAVE;
//        }
//        temp.n = name;
        return true; 
    }

    /*--------------------------------------------------------------------------------
        Function    : CreatureParser::CreatureListener::parserFlag
        Description : Creature doesn't use flags, so just returns true
        Inputs      : parser, flag name
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool CreatureParser::CreatureListener::parserFlag(TCODParser* parser, const char* name)
    {
        // unused parameters
        (void) parser;
        (void) name;

        return true;
    }

    /*--------------------------------------------------------------------------------
        Function    : CreatureParser::CreatureListener::parserProperty
        Description : Called when the parser finds a property, this updates the
                      relevant tracking variable in CreatureListener.
        Inputs      : parser, property name, value type, value data
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool CreatureParser::CreatureListener::parserProperty
    (TCODParser* parser, const char* name, TCOD_value_type_t valtype, TCOD_value_t value)
    {
        // unused parameters
        (void) valtype;

        if(strcmp(name, "character") == 0)
        {
            temp.character = value.i;
        }
        else if(strcmp(name, "fgColor") == 0)
        {
            temp.fgColor = value.col;
        }
        else if(strcmp(name, "hp") == 0)
        {
            temp.hp = value.i;
        }
        else if(strcmp(name, "shortdesc") == 0)
        {
            temp.shortdesc = value.s;
        }
        else if(strcmp(name, "longdesc") == 0)
        {
            temp.longdesc = value.s;
        }
        else
        {
            parser->error("Unknown property '%s' in struct '%s'", value.s, name);
            return false;
        }
        return true;
    }

    /*--------------------------------------------------------------------------------
        Function    : CreatureParser::CreatureListener::parserEndStruct
        Description : Called when the parser comes to the end of a tileset structure,
                      this callback creates a new Creature object out of the previously
                      read data and adds it to the Creature list.  
        Inputs      : parser, tileset struct, name of the struct
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool CreatureParser::CreatureListener::parserEndStruct
    (TCODParser* parser, const TCODParserStruct* str, const char* name)
    {
        // unused parameters
        (void) parser;
        (void) str;
        (void) name;

        Creature::list.push_back(CreaturePtr(new Creature(temp)));
        return true;
    }

    /*--------------------------------------------------------------------------------
        Function    : CreatureParser::CreatureListener::error
        Description : Called when the parser detects an error.  Prints the given error
                      message.  If this function is called, it will terminate the
                      program.
        Inputs      : error message
        Outputs     : error message
        Return      : void
    --------------------------------------------------------------------------------*/
    void CreatureParser::CreatureListener::error(const char* error)
    {
        cout << "CREATURE PARSING ERROR: " << error << endl;
    }

    /*--------------------------------------------------------------------------------
        Function    : CreatureParser::verify
        Description : Testing function that prints out the contents of the Creature
                      list to verify that everything was read correctly.
        Inputs      : None
        Outputs     : contents of Creature list
        Return      : void
    --------------------------------------------------------------------------------*/
    void CreatureParser::verify()
    {
        vector<CreaturePtr>::iterator it, end;
        end = Creature::list.end();
        for(it = Creature::list.begin(); it != end; ++it)
        {
            cout << "Creature: " << (*it)->getName() << endl;
            cout << "\tDisplay Character: " << (*it)->getChar() << endl;
            //cout << "\tFG Color: " << (*it)->getFgColor() << endl;
            cout << "\tHP: " << (*it)->getHP() << endl;
            cout << "\tshortdesc: " << (*it)->shortDescription() << endl;
            cout << "\tlongdesc: " << (*it)->longDescription() << endl;
        }
    }

    /*--------------------------------------------------------------------------------
        Function    : CreatureParser::defineSyntax
        Description : defines the syntax for the creature config file.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void CreatureParser::defineSyntax()
    {
        // define Creature structure
        TCODParserStruct Creature = *(parser.newStructure("Creature"));
        Creature.addProperty("character", TCOD_TYPE_INT, true);
        Creature.addProperty("fgColor", TCOD_TYPE_COLOR, true);
        Creature.addProperty("hp", TCOD_TYPE_INT, true);
        Creature.addProperty("shortdesc", TCOD_TYPE_STRING, true);
        Creature.addProperty("longdesc", TCOD_TYPE_STRING, true);
    }

    /*--------------------------------------------------------------------------------
        Function    : CreatureParser::run
        Description : Entry point into the Creature Parser.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void CreatureParser::run()
    {
        defineSyntax();
        parser.run(filename.c_str(), &listener);
        verify();
    }
}
