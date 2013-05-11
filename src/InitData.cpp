#include "InitData.hpp"
using namespace std;

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : InitData::error
        Description : Reports if an value is not found in txt and exits the
                      program.
        Inputs      : value that wasn't found
        Outputs     : error message
        Return      : void
    --------------------------------------------------------------------------------*/
    void InitData::error(const string& value)
    {
        cerr << "INIT ERROR: Value " << value << " not found in init.txt" << endl;
        abort();
    }



    /*--------------------------------------------------------------------------------
        Function    : InitData::readNextValue
        Description : reads the next metadata value from txt
        Inputs      : None
        Outputs     : None
        Return      : a string containing the next metadata value
    --------------------------------------------------------------------------------*/
    string InitData::readNextValue()
    {
        if(!input.is_open())
        {
            cerr << "init.txt not found. Aborting." << endl;
            exit(1);
        }
        string line;
        while(!input.eof())
        {
            getline(input, line);
            if(line.empty() || line.at(0) != '[') // all data entries start with a [
               continue;
            else 
            {
                int i = 1; // loop index
                string data; // data value to return
                // skip over the data name
                while(line[i] != ':') i++;
                i++; // move off the ':'
                // read the data
                while(line[i] != ']')
                {
                    data += line[i++];
                }
                // data is read, so return the value
                return data;
            }
        }
        // if the end of the file has been reached without finding a data value,
        // then a metadata entry will be left unfilled. Return a "NOVALUE" string
        // for error handling in the alization function
        return "NOVALUE";
    }



    /*--------------------------------------------------------------------------------
        Function    : InitData::InitData
        Description : Default consructor for InitData.  Sets all member variables to 0
        Inputs      : None
        Outputs     : None
        Return      : None (constructor)
    --------------------------------------------------------------------------------*/
    InitData::InitData()
    : rootWidth(0), rootHeight(0), rootTileWidth(0), rootTileHeight(0),
      gwWidth(0), gwHeight(0), gwTileWidth(0), gwTileHeight(0),
      commandLineHeight(0), font(""), fontWidth(0), fontHeight(0), fontgs(false),
      fontLayout(""), mlgwRatio(0.0f), logSize(0) {}



    /*--------------------------------------------------------------------------------
        Function    : InitData::readInitFile
        Description : reads the metadata from txt and processes it into the
                      metadata variables in the Yarl class. 
        Inputs      : filename
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void InitData::readInitFile(const char* filename="./init.txt")
    {
        string value, value2;
        input.open(filename);

    // read WINDOWWIDTH
        value = readNextValue();
        if(value == "NOVALUE") error("WINDOWWIDTH");
        rootWidth = atoi(value.c_str());

    // read WINDOWHEIGHT
        value = readNextValue();
        if(value == "NOVALUE") error("WINDOWHEIGHT");
        rootHeight = atoi(value.c_str());

    // read GAMEFONT
        value = readNextValue();
        if(value == "NOVALUE") error("GAMEFONT");
    #ifdef _WIN32
        font = ".\\fonts\\" + value;
    #else
        font = "./fonts/" + value;
    #endif

    // read FONTWIDTH
        value = readNextValue();
        if(value == "NOVALUE") error("FONTWIDTH");
        fontWidth = atoi(value.c_str());

    // read FONTHEIGHT
        value = readNextValue();
        if(value == "NOVALUE") error("FONTHEIGHT");
        fontHeight = atoi(value.c_str());

    // read GREYSCALE
        value = readNextValue();
        if(value == "NOVALUE") error("GREYSCALE");
        fontgs = (value == "true") ? true : false;

    // read FONTLAYOUT
        value = readNextValue();
        if(value == "NOVALUE") error("FONTLAYOUT");
        fontLayout = value;

    // determine mlgwRatio
        value = readNextValue();
        if(value == "NOVALUE") error("MLHEIGHT");
        value2 = readNextValue();
        if(value2 == "NOVALUE") error("GWHEIGHT");
        float v1 = boost::lexical_cast<float>(value);
        float v2 = boost::lexical_cast<float>(value2);
        mlgwRatio = v2 / (v1 + v2);

    // determine pwgwRatio
        value = readNextValue();
        if(value == "NOVALUE") error("PWWIDTH");
        value2 = readNextValue();
        if(value2 == "NOVALUE") error("GWWIDTH");
        v1 = boost::lexical_cast<float>(value); 
        v2 = boost::lexical_cast<float>(value2);
        pwgwRatio = v2 / (v1 + v2);

    // read RENDERER
        value = readNextValue();
        if(value == "NOVALUE") error("RENDERER");
        if(value == "GLSL")
            renderer = TCOD_RENDERER_GLSL;
        else if(value == "OPENGL")
            renderer = TCOD_RENDERER_OPENGL;
        else if(value == "SDL")
            renderer = TCOD_RENDERER_SDL;
        else
        {
            cerr << "Renderer setting in txt not recognized. ";
            cerr << "Defaulting to SDL." << endl;
            renderer = TCOD_RENDERER_SDL;
        }

    // read LOGSIZE
        value = readNextValue();
        if(value == "NOVALUE") error("LOGSIZE");
        logSize = atoi(value.c_str());

    // read LIGHTFLICKER
        value = readNextValue();
        if(value == "NOVALUE") error("LIGHTFLICKER");
        LIGHT_FLICKER_ENABLED = (value == "true") ? true : false;

        input.close();

        // now that FONTSIZE has been read, we can set the window tile lengths

        // set the tile size of the root console
        rootTileWidth = rootWidth / fontWidth;
        rootTileHeight = rootHeight / fontHeight;
        
        // set Game Window size
        //gwWidth = 3 * rootWidth/4;
        //gwHeight = 2 * rootHeight/3;
        gwWidth = static_cast<int>(rootWidth * pwgwRatio);
        gwHeight = static_cast<int>(rootWidth * mlgwRatio);
        gwTileWidth = gwWidth / fontWidth;
        gwTileHeight = gwHeight / fontHeight;

        // set Command Line height
        commandLineHeight = gwTileHeight + 1;
    }



    /*--------------------------------------------------------------------------------
        Function    : InitData::initRoot
        Description : initializes the root console
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void InitData::initRoot()
    {
        TCODConsole::setKeyboardRepeat(500,50); 
        TCODSystem::setFps(32);

        if(fontLayout == "as")
        {
            if(fontgs)
            {
                TCODConsole::setCustomFont(font.c_str(), 
                                           TCOD_FONT_LAYOUT_ASCII_INCOL |
                                           TCOD_FONT_TYPE_GREYSCALE);
            }
            else TCODConsole::setCustomFont(font.c_str(), TCOD_FONT_LAYOUT_ASCII_INCOL);
        }
        else if(fontLayout == "ro")
        {
            if(fontgs)
            {
                TCODConsole::setCustomFont(font.c_str(), 
                                           TCOD_FONT_LAYOUT_ASCII_INROW |
                                           TCOD_FONT_TYPE_GREYSCALE);
            }
            else TCODConsole::setCustomFont(font.c_str(), TCOD_FONT_LAYOUT_ASCII_INROW);
        }
        else if(fontLayout == "tc")
        {
            if(fontgs)
            {
                TCODConsole::setCustomFont(font.c_str(), 
                                           TCOD_FONT_LAYOUT_TCOD |
                                           TCOD_FONT_TYPE_GREYSCALE);
            }
            else TCODConsole::setCustomFont(font.c_str(), TCOD_FONT_LAYOUT_TCOD);
        }
        else // error
        {
            cerr << "Invalid font layout in init.txt.  ";
            cerr << "Must be either 'as', 'ro', or 'tc'." << endl;
            abort();
        }

        TCODConsole::initRoot(rootTileWidth,
                              rootTileHeight,
                              "Yet Another RogueLike",
                              false, // no fullscreen (yet)
                              renderer);

        UIForeColor = TCODColor::lightestGrey;
        UIBackColor = TCODColor::black;
        TCODConsole::root->setDefaultBackground(UIBackColor);
        TCODConsole::root->setDefaultForeground(UIForeColor);
    }
}

