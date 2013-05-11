#include "Tileset.hpp"

using namespace std;
using namespace rlns::utl;

namespace rlns
{
    namespace model
    {
        vector<TilesetPtr> Tileset::list;

        /*--------------------------------------------------------------------------------
            Function    : Tileset::findTilesetIndex
            Description : Searches the Tileset list for a tileset with the given name
                          and returns the integer index of that tileset, or -1 if the name
                          is not found.  This is a private helper function for
                          getTileset().
            Inputs      : name of a tileset
            Outputs     : None
            Return      : int
        --------------------------------------------------------------------------------*/
        int Tileset::findTilesetIndex(const string& tilesetName)
        {
            int end = Tileset::list.size();
            for(int i=0; i < end; ++i)
            {
                if(Tileset::list[i]->name == tilesetName)
                    return i;
            }
            // tileset name not found
            return -1;
        }



        /*--------------------------------------------------------------------------------
            Function    : Tileset::findTileset
            Description : Finds the tileset in Tileset::list with the given name
            Inputs      : Name of the desired tileset
            Outputs     : None
            Return      : TilesetPtr
        --------------------------------------------------------------------------------*/
        TilesetPtr Tileset::findTileset(const string& tilesetName)
        {
            int index = findTilesetIndex(tilesetName);

            if(index < 0) // if the tileset wasn't found, index will be -1
            {
                utl::fatalError("Tileset '" + tilesetName + "' not found!");
            }

            return Tileset::list.at(index);
        }



        /*--------------------------------------------------------------------------------
            Function    : Tileset::saveToDisk
            Description : Saves the tileset to the given RLNSZip save buffer.  Since
                          tilesets are initially read from a datafile, we only need to
                          save the Tileset's name.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Tileset::saveToDisk(RLNSZip& zip) const
        {
            zip.putString(name.c_str());
        }



        /*--------------------------------------------------------------------------------
            Function    : TilesetParser::TilesetListener::parserNewStruct
            Description : Called when the parser finds a new Tileset struct.  This
                          function doesn't do any data reading; it just resets the flag
                          tracking variables.
            Inputs      : parser, parser struct, name of the struct
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool TilesetParser::TilesetListener::parserNewStruct(TCODParser* parser, const TCODParserStruct* str, const char* name)
        {
            // unused parameters
            (void) parser;

            string strName(str->getName());
            if(strName == "Dungeon")
            {
                temp.type = MapType::DUNGEON;
            }
            else if(strName == "Cave")
            {
                temp.type = MapType::CAVE;
            }
            temp.n = name;
            return true; 
        }



        /*--------------------------------------------------------------------------------
            Function    : TilesetParser::TilesetListener::parserFlag
            Description : Only returns true, since the Tileset struct doesn't use flags
            Inputs      : parser, flag name
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool TilesetParser::TilesetListener::parserFlag(TCODParser* parser, const char* name)
        {
            // unused parameters
            (void) parser;
            (void) name;

            return true;
        }



        /*--------------------------------------------------------------------------------
            Function    : TilesetParser::TilesetListener::parserProperty
            Description : Called when the parser finds a property, this updates the
                          relevant tracking variable in TilesetListener.
            Inputs      : parser, property name, value type, value data
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool TilesetParser::TilesetListener::parserProperty
        (TCODParser* parser, const char* name, TCOD_value_type_t valtype, TCOD_value_t value)
        {
            // unused parameters
            (void) valtype;

            if(strcmp(name, "floorTile") == 0)
            {
                temp.ft = value.i;
                if(!Tile::tileExists(temp.ft))
                {
                    parser->error("tile ID '%d' not found in Tile list", value.i);
                }
            }
            else if(strcmp(name, "wallTile") == 0)
            {
                temp.wt = value.i;
                if(!Tile::tileExists(temp.wt))
                {
                    parser->error("tile ID '%d' not found in Tile list", value.i);
                }
            }
            else if(strcmp(name, "fillerTile") == 0)
            {
                temp.flt = value.i;
                if(!Tile::tileExists(temp.flt))
                {
                    parser->error("tile ID '%d' not found in Tile list", value.i);
                }
            }
            else if(strcmp(name, "upStair") == 0)
            {
                temp.us = value.i;
                if(!Tile::tileExists(temp.us))
                {
                    parser->error("tile ID '%d' not found in Tile list", value.i);
                }
            }
            else if(strcmp(name, "downStair") == 0)
            {
                temp.ds = value.i;
                if(!Tile::tileExists(temp.ds))
                {
                    parser->error("tile ID '%d' not found in Tile list", value.i);
                }
            }
            else if(strcmp(name, "n_s_Door") == 0)
            {
                temp.nsd = value.i;
                if(!Tile::tileExists(temp.nsd))
                {
                    parser->error("tile ID '%d' not found in Tile list", value.i);
                }
            }
            else if(strcmp(name, "e_w_Door") == 0)
            {
                temp.ewd = value.i;
                if(!Tile::tileExists(temp.ewd))
                {
                    parser->error("tile ID '%d' not found in Tile list", value.i);
                }
            }
            else if(strcmp(name, "ambientLight") == 0)
            {
                temp.al = value.col;
            }
            else if(strcmp(name, "mapWidth") == 0)
            {
                temp.mw = value.i;
            }
            else if(strcmp(name, "mapHeight") == 0)
            {
                temp.mh = value.i;
            }
            else if(strcmp(name, "recurseLevel") == 0)
            {
                temp.rl = value.i;
            }
            else if(strcmp(name, "minHSize") == 0)
            {
                temp.mhs = value.i;
            }
            else if(strcmp(name, "minVSize") == 0)
            {
                temp.mvs = value.i;
            }
            else if(strcmp(name, "maxHRatio") == 0)
            {
                temp.mhr = value.f;
            }
            else if(strcmp(name, "maxVRatio") == 0)
            {
                temp.mvr = value.f;
            }
            else
            {
                parser->error("Unknown property '%s' in struct '%s'", value.s, name);
                return false;
            }
            return true;
        }



        /*--------------------------------------------------------------------------------
            Function    : TilesetParser::TilesetListener::parserEndStruct
            Description : Called when the parser comes to the end of a tileset structure,
                          this callback creates a new Tileset object out of the previously
                          read data and adds it to the Tileset list.  
            Inputs      : parser, tileset struct, name of the struct
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool TilesetParser::TilesetListener::parserEndStruct
        (TCODParser* parser, const TCODParserStruct* str, const char* name)
        {
            // unused parameters
            (void) parser;
            (void) str;
            (void) name;

            Tileset::list.push_back(TilesetPtr(new Tileset(temp)));
            return true;
        }



        /*--------------------------------------------------------------------------------
            Function    : TilesetParser::TilesetListener::error
            Description : Called when the parser detects an error.  Prints the given error
                          message.  If this function is called, it will terminate the
                          program.
            Inputs      : error message
            Outputs     : error message
            Return      : void
        --------------------------------------------------------------------------------*/
        void TilesetParser::TilesetListener::error(const char* error)
        {
            cout << "TILESET PARSING ERROR: " << error << endl;
        }



        /*--------------------------------------------------------------------------------
            Function    : TilesetParser::verify
            Description : Testing function that prints out the contents of the Tileset
                          list to verify that everything was read correctly.
            Inputs      : None
            Outputs     : contents of Tileset list
            Return      : void
        --------------------------------------------------------------------------------*/
        void TilesetParser::verify()
        {
            vector<TilesetPtr>::iterator it, end;
            end = Tileset::list.end();
            for(it = Tileset::list.begin(); it != end; ++it)
            {
                cout << "Tileset: " << (*it)->getName() << endl;
                cout << "\ttype: " << (*it)->getType() << endl;
                cout << "\tfloorTile: " << (*it)->getFloorTileID() << endl;
                cout << "\twallTile: " << (*it)->getWallTileID() << endl;
                cout << "\tfillerTile: " << (*it)->getFillerTileID() << endl;
                cout << "\tupStair: " << (*it)->getUpStairTileID() << endl;
                cout << "\tdownStair: " << (*it)->getDownStairTileID() << endl;
                cout << "\tn-s_Door: " << (*it)->getN_S_DoorID() << endl;
                cout << "\te-w_Door: " << (*it)->getE_W_DoorID() << endl;
                //cout << "\tambientLight: " << (*it)->getAmbientLight() << endl;
                cout << "\tmapWidth: " << (*it)->getMapWidth() << endl;
                cout << "\tmapHeight: " << (*it)->getMapHeight() << endl;
                cout << "\trecurseLevel: " << (*it)->getRecurseLevel() << endl;
                cout << "\tminHSize: " << (*it)->getMinHSize() << endl;
                cout << "\tminVSize: " << (*it)->getMinVSize() << endl;
                cout << "\tmaxHRatio: " << (*it)->getMaxHRatio() << endl;
                cout << "\tmaxVRatio: " << (*it)->getMaxVRatio() << endl;
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : TilesetParser::defineSyntax
            Description : defines the syntax for the tileset config file.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void TilesetParser::defineSyntax()
        {
            // define Dungeon structure
            TCODParserStruct Dungeon = *(parser.newStructure("Dungeon"));
            Dungeon.addProperty("floorTile", TCOD_TYPE_INT, true);
            Dungeon.addProperty("wallTile", TCOD_TYPE_INT, true);
            Dungeon.addProperty("fillerTile", TCOD_TYPE_INT, true);
            Dungeon.addProperty("upStair", TCOD_TYPE_INT, true);
            Dungeon.addProperty("downStair", TCOD_TYPE_INT, true);
            Dungeon.addProperty("n_s_Door", TCOD_TYPE_INT, true);
            Dungeon.addProperty("e_w_Door", TCOD_TYPE_INT, true);
            Dungeon.addProperty("ambientLight", TCOD_TYPE_COLOR, true);
            Dungeon.addProperty("mapWidth", TCOD_TYPE_INT, true);
            Dungeon.addProperty("mapHeight", TCOD_TYPE_INT, true);
            Dungeon.addProperty("recurseLevel", TCOD_TYPE_INT, true);
            Dungeon.addProperty("minHSize", TCOD_TYPE_INT, true);
            Dungeon.addProperty("minVSize", TCOD_TYPE_INT, true);
            Dungeon.addProperty("maxHRatio", TCOD_TYPE_FLOAT, true);
            Dungeon.addProperty("maxVRatio", TCOD_TYPE_FLOAT, true);

            // define Cave structure
            TCODParserStruct Cave = *(parser.newStructure("Cave"));
            Cave.addProperty("floorTile", TCOD_TYPE_INT, true);
            Cave.addProperty("wallTile", TCOD_TYPE_INT, true);
            Cave.addProperty("fillerTile", TCOD_TYPE_INT, true);
            Cave.addProperty("upStair", TCOD_TYPE_INT, true);
            Cave.addProperty("downStair", TCOD_TYPE_INT, true);
            Cave.addProperty("n_s_Door", TCOD_TYPE_INT, true);
            Cave.addProperty("e_w_Door", TCOD_TYPE_INT, true);
            Cave.addProperty("ambientLight", TCOD_TYPE_COLOR, true);
            Cave.addProperty("mapWidth", TCOD_TYPE_INT, true);
            Cave.addProperty("mapHeight", TCOD_TYPE_INT, true);
            Cave.addProperty("recurseLevel", TCOD_TYPE_INT, true);
            Cave.addProperty("minHSize", TCOD_TYPE_INT, true);
            Cave.addProperty("minVSize", TCOD_TYPE_INT, true);
            Cave.addProperty("maxHRatio", TCOD_TYPE_FLOAT, true);
            Cave.addProperty("maxVRatio", TCOD_TYPE_FLOAT, true);
        }



        /*--------------------------------------------------------------------------------
            Function    : TilesetParser::run
            Description : Entry point into the Tileset Parser.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void TilesetParser::run()
        {
            defineSyntax();
            parser.run(filename.c_str(), &listener);
            //verify();
        }
    }
}
