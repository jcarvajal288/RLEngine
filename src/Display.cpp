#include "Display.hpp"

using namespace std;

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : Display::drawLevelItems
        Description : Draws the Items found in the given level.  This function is
                      called after the Level's map has been drawn in the 
                      drawPlayfield() function.
        Inputs      : Level object
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::drawLevelItems(const LevelPtr level)
    {
        vector<ItemPtr> items = level->getItems();
        vector<ItemPtr>::const_iterator it, end;
        it = items.begin(); end = items.end();

        for(; it!=end; ++it)
        {
            drawObject(*it);
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : Display::drawLevelOccupants
        Description : Draws all of the characters and monsters in the level, including
                      the player's party.  This function is called after the Level's
                      map and the Level's items have been drawn in the drawPlayfield()
                      function.
        Inputs      : Level Object
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::drawLevelOccupants(const LevelPtr level)
    {
        vector<PartyPtr> parties = level->getParties();
        vector<PartyPtr>::const_iterator it, end;
        it = parties.begin(); end = parties.end();

        for(; it!=end; ++it)
        {
            vector<ActorPtr> members = (*it)->getMembers();
            vector<ActorPtr>::const_iterator current, finish;
            current = members.begin(); finish = members.end();

            for(; current!=finish; ++current)
            {
                drawObject(*current);
            }
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : Display::Display
        Description : Default constructor for Display.  Refers to data read in by
                      InitData to construct its consoles.
        Inputs      : InitData object
        Outputs     : None
        Return      : None (constructor)
    --------------------------------------------------------------------------------*/
    Display::Display(const InitData& initData)
    : focalPt(0,0),
      _playfieldTL(0,0),
      _playfield(new TCODConsole(initData.getGwTileWidth(),
                                 initData.getGwTileHeight())),
      _partyBarTL(_playfield->getWidth(), 0),
      _partyBar(new TCODConsole(initData.getRootTileWidth() - _playfield->getWidth(),
                                initData.getRootTileHeight())),
      _consoleTL(0, _playfield->getHeight()),
      _console(new TCODConsole(initData.getRootTileWidth() - _partyBar->getWidth(),
                               initData.getRootTileHeight() - _playfield->getHeight())),
      messageTracker(new MessageTracker(initData.getLogSize())) {}



    /*--------------------------------------------------------------------------------
        Function    : Display::shiftFocalPoint
        Description : Moves the focal point one tile in the given direction
        Inputs      : DirectionType
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::shiftFocalPoint(const DirectionType dir)
    {
        focalPt.shift(dir);
    }



    /*--------------------------------------------------------------------------------
        Function    : Display::setDisplayValues
        Description : This function calculates the dimensions of the display area and
                      what in the game the camera is looking at.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::setDisplayValues()
    {
        // get dimensions of the display area
        dispTL = Point(0,0);
        dispBR = Point(_playfield->getWidth(), _playfield->getHeight());
        dispCenter = Point((dispTL.X() + dispBR.X())/2, 
                                 (dispTL.Y() + dispBR.Y())/2); 

        // get the difference of the focal point and the center of the display area
        Point diff(dispCenter.X(), dispCenter.Y());
        camTL = focalPt - diff;
        camBR = focalPt + diff;
    }



    /*--------------------------------------------------------------------------------
        Function    : Display::drawObject
        Description : Checks if the object's position is within the viewable area, and
                      if so, draws it on the playfield.
        Inputs      : MapObjectPtr
        Outputs     : the actor on the playfield
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::drawObject(const MapObjectPtr object)
    {
        Point objectPosition = object->getPosition();

        // adjust the camera width and height by one coordinate
        // because the playfield dimensions start at 0 and go to (dimension - 1)
        Point adjustedCamBR = camBR - Point(1,1);
        
        if(objectPosition.withinBounds(camTL, adjustedCamBR))
        {
            Point relativePosition = objectPosition - camTL;
            int x = relativePosition.X(), y = relativePosition.Y();
            TCODColor backgroundColor = _playfield->getCharBackground(x,y);
            _playfield->putCharEx(x, y, object->getChar(), object->getFgColor(), backgroundColor);
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : Display::inspectTile
        Description : Queries a level to see if there is anything interesting at the
                      given coordinate.  If so, it prints a message to the console.
        Inputs      : Level object, coordinate to inspect
        Outputs     : Notable contents of the tile.
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::inspectTile(const LevelPtr level, const Point& pt)
    {
        string message;
        level->inspectTileContents(message, pt);
        if(message.empty()) return;
        messageTracker->addStdMessage(message);
    }



    /*--------------------------------------------------------------------------------
        Function    : Display::drawPlayfield
        Description : Draws the level Map.  Before drawing anything, this function
                      firsts calculates how much of the map is actually displayed on
                      the screen and does not attempt to draw outside the map bounds.
                      (Sounds obvious, but the first implementation simply caught
                      std::out_of_range exceptions and ignored them.  Turned out to be
                      inefficient to catch a few thousand exceptions every refresh.  
                      Who knew.)
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::drawPlayfield() 
    {
        // erase the old contents of the playfield
        _playfield->clear();

        LevelPtr level = Level::getCurrentLevel();

        // determine if part of the camera view is off the map
        bool camTLX_gt_0 = camTL.X() >= 0;
        bool camTLY_gt_0 = camTL.Y() >= 0;
        bool camBRX_lt_w = camBR.X() < level->getMapWidth();
        bool camBRY_lt_h = camBR.Y() < level->getMapHeight();

        // adjust display and camera coordinates accordingly
        int dispTL_X = camTLX_gt_0 ? dispTL.X() : dispTL.X() - camTL.X();
        int dispTL_Y = camTLY_gt_0 ? dispTL.Y() : dispTL.Y() - camTL.Y();
        int dispBR_X = camBRX_lt_w ? dispBR.X() : dispBR.X() + (level->getMapWidth() - camBR.X());    // '-1' not necessary here, for some reason.
        int dispBR_Y = camBRY_lt_h ? dispBR.Y() : dispBR.Y() + (level->getMapHeight() - camBR.Y()-1); // '-1' necessary to avoid drawing tiles off the edge of the map 
        int camTL_X  = (camTL.X() >= 0) ? camTL.X() : 0;
        int camTL_Y  = (camTL.Y() >= 0) ? camTL.Y() : 0;

        int a,b,x,y; // loop variables. (a,b) - console location; (x,y) map location
        TileInfo tileInfo;
        for(a=dispTL_X, x=camTL_X; a<dispBR_X; ++a, ++x)
        {
            for(b=dispTL_Y, y=camTL_Y; b<dispBR_Y; ++b, ++y)
            {
                try 
                {
                    tileInfo = level->getTileInfo(x,y);
                    _playfield->putCharEx(a,b, tileInfo.ascii, tileInfo.fgColor, tileInfo.bgColor);
                }
                catch(std::out_of_range&)
                {
                    cerr << "map exception caught" << endl;
                }
            }
        }

        drawLevelItems(level);
        drawLevelOccupants(level);
    }



    /*--------------------------------------------------------------------------------
        Function    : Display::drawPartyBar
        Description : Draws the party bar to the screen
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::drawPartyBar() 
    {
        _partyBar->setDefaultBackground(TCODColor::green);
        _partyBar->clear();
    }



    /*--------------------------------------------------------------------------------
        Function    : Display::drawConsole
        Description : Draws the console to the screen
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::drawConsole()
    {
        _console->clear();

        _console->printEx(0,1, TCOD_BKGND_NONE, TCOD_LEFT, messageTracker->getPrompt().c_str());

        int x = 0, y = 3;
        int width = _console->getWidth();
        int height = _console->getHeight()-y;

        // draw the Console frame
        _console->printFrame(x, y, width, height, false);
        _console->printEx(x+(width/2), y, 
                             TCOD_BKGND_NONE, 
                             TCOD_CENTER, 
                             " MESSAGE LOG ");

        // change the dimensions to fit the text inside the frame
        ++x; ++y; width -= 2; --height;

        // print the Console's messages
        int count = 0;
        int textHeight;
        list<MessagePtr>::const_iterator it, end;
        it = messageTracker->begin(); end = messageTracker->end();

        for(; it!=end; ++it)
        {
            string message = "%c" + (*it)->getText() + "%c";
            TCODConsole::setColorControl(TCOD_COLCTRL_1, 
                                         (*it)->getForeColor(),
                                         (*it)->getBackColor());
            textHeight = _console->getHeightRect(x, y, width, height,
                                                          message.c_str(),
                                                          TCOD_COLCTRL_1,
                                                          TCOD_COLCTRL_STOP);
            if(count+textHeight > height-1) 
                break;
            _console->printRectEx(x, y, width, height,
                                           TCOD_BKGND_SET,
                                           TCOD_LEFT,
                                           message.c_str(),
                                           TCOD_COLCTRL_1,
                                           TCOD_COLCTRL_STOP);
            y += textHeight;
            count += textHeight;
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : Display::draw
        Description : Blits all of Display's consoles onto the root console.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::draw() const
    {
        Point tl = _playfieldTL;
        TCODConsole::blit(_playfield.get(), 0,0, _playfield->getWidth(), _playfield->getHeight(),
                          TCODConsole::root, tl.X(), tl.Y());
        tl = _partyBarTL;
        TCODConsole::blit(_partyBar.get(), 0,0, _partyBar->getWidth(), _partyBar->getHeight(),
                          TCODConsole::root, tl.X(), tl.Y());
        tl = _consoleTL;
        TCODConsole::blit(_console.get(), 0,0, _console->getWidth(), _console->getHeight(),
                          TCODConsole::root, tl.X(), tl.Y());
    }



    /*--------------------------------------------------------------------------------
        Function    : Display::refresh
        Description : Calls all of the various console drawing functions and then the
                      Display blit function which puts each console onto the root
                      console.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Display::refresh()
    {
        setDisplayValues();
        drawPlayfield();
        //drawPartyBar();
        drawConsole();
        draw();
    }
}
