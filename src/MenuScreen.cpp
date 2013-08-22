#include "MenuScreen.hpp"

using namespace std;

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : MenuScreen::inc/decSelectedLine
        Description : increments or decrements selectedLine by one.  If this causes
                      selectedLine to be less than 0 or greater than the limit integer
                      specified, it will wrap around.
        Inputs      : limit integer
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void MenuScreen::incSelectedLine(const int limit)
    {
        if(limit <= 0) return;

        selectedLine = (selectedLine+1) % limit;
    }

    void MenuScreen::decSelectedLine(const int limit)
    {
        if(limit <= 0) return;

        --selectedLine;
        if(selectedLine < 0) 
            selectedLine = limit-1;
    }



    /*--------------------------------------------------------------------------------
        Function    : MenuScreen::inc/decSelectedTab
        Description : Increments or decrements selectedTab by one.  Works the same
                      as inc/decSelectedLine above.
        Inputs      : limit integer
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void MenuScreen::incSelectedTab(const int limit)
    {
        if(limit <= 0) return;

        selectedTab = (selectedTab+1) % limit;
    }

    void MenuScreen::decSelectedTab(const int limit)
    {
        if(limit <= 0) return;

        --selectedTab;
        if(selectedTab < 0) 
            selectedTab = limit-1;
    }



    /*--------------------------------------------------------------------------------
        Function    : MenuScreen::pageUp/Down
        Description : increments or decrements selectedLine by linesPerPage.  If this 
                      causes selectedLine to be less than 0 or greater than the limit 
                      integer specified, it will wrap around.
        Inputs      : limit integer
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void MenuScreen::pageUp(const int limit)
    {
        if(limit <= 0) return;
        selectedLine -= (linesPerPage-1);
        if(selectedLine < 0) selectedLine = limit-linesPerPage;
    }
    void MenuScreen::pageDown(const int limit)
    {
        if(limit <= 0) return;
        selectedLine = (selectedLine+linesPerPage-1) % limit;
    }



    /*--------------------------------------------------------------------------------
        Function    : MenuScreen::drawFolders
        Description : draws tabbed folders in a MenuScreen
        Inputs      : two ints representing the coodinates of the topleft of the
                      folder area, the dimensions of the game window, and a vector
                      of the characters to put in the tabs.
        Outputs     : None
        Return      : Point representing the topleft corner of the writable area
    --------------------------------------------------------------------------------*/
    Point MenuScreen::drawFolders(int& x, int& y, const Point& dim, 
                                  const vector< pair<int, TCODColor> >& tabChars) 
    {
        const int xstart = x;
        const int ystart = y;
        const int width = dim.X();
        const int height = dim.Y();
        const int NUMTABS = static_cast<int>(tabChars.size());
        drawTabs(x,y, tabChars);
        
        // draw the top of the folder, leaving the current tab open at the bottom
        for(; x<TABWIDTH*NUMTABS; ++x)
        {
            // if this is the selected tab
            if(x/TABWIDTH == selectedTab)
            {
                // if this is the left border
                if(x % TABWIDTH == 0) 
                {
                    if(selectedTab == 0) 
                        screen.setChar(x,y, TCOD_CHAR_VLINE);
                    else 
                        screen.setChar(x,y, TCOD_CHAR_SE);
                }
                // else if this is the right tab border
                else if(x % TABWIDTH == TABWIDTH-1) 
                {
                    if(selectedTab == NUMTABS) 
                        screen.setChar(x,y, TCOD_CHAR_VLINE);
                    else 
                        screen.setChar(x,y, TCOD_CHAR_SW);
                }
            }
            else
            {
                // if this is the left border
                if(x % TABWIDTH == 0) 
                {
                    if(x == xstart) 
                        screen.setChar(x,y, TCOD_CHAR_TEEE);
                    else 
                        screen.setChar(x,y, TCOD_CHAR_TEEN);
                }
                // else if this is the right tab border
                else if(x % TABWIDTH == TABWIDTH-1)
                {
                    screen.setChar(x,y, TCOD_CHAR_TEEN);
                }
                else screen.setChar(x,y, TCOD_CHAR_HLINE);
            }
        }

        // draw the rest of the top line
        for(; x<width-1; ++x) 
        {
            screen.setChar(x,y, TCOD_CHAR_HLINE);
        }
        screen.setChar(x,y++, TCOD_CHAR_NE); 
        x=xstart;

        // draw the rest of the folder box
        for(; y<height-1; ++y)
        {
            screen.setChar(xstart,y, TCOD_CHAR_VLINE);
            screen.setChar(width-1,y, TCOD_CHAR_VLINE);
        }

        screen.setChar(xstart,y, TCOD_CHAR_SW);
        screen.setChar(width-1,y, TCOD_CHAR_SE);

        for(x=xstart+1; x<width-1; ++x) 
        {
            screen.setChar(x,y, TCOD_CHAR_HLINE);
        }

        return Point(xstart+1, ystart+TABHEIGHT);
    }



    /*--------------------------------------------------------------------------------
        Function    : MenuScreen::drawScrollBar
        Description : draws a scroll bar at the designated Point, extending the given
                      length either horizontally or vertically.  The scroll bar is 
                      divided into a section for each page, and one of these sections
                      is highlighted to indicate the current page.
        Inputs      : Point, length, direction, current page, number of pages 
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void MenuScreen::drawScrollBar(const Point& origin, const int length, 
                                   const int curPage, const int numPages, const bool vert)
    {
        int x = origin.X(), y = origin.Y();

        // draw top/left arrow
        if(vert) screen.setChar(x, y++, TCOD_CHAR_ARROW2_N);
        else screen.setChar(x++, y, TCOD_CHAR_ARROW2_W);

        // draw the body of the scroll bar, highlighting the current page
        // if numPages doesn't divide cleanly into length, there will be
        // some extra tiles left over.  Add them to the last section.
        const int bodyLength = length+origin.Y()-2; // the '-2' allows for the arrows
        const int pageLength = (length-2)/numPages;
        const int extraTiles = (length-2)%numPages; 

        if(vert)
        {
            for(; y<bodyLength; ++y)
            {
                if(((y-origin.Y())/pageLength == curPage) 
                || (curPage+1 == numPages && y >= bodyLength-extraTiles))
                    screen.setChar(x,y, TCOD_CHAR_BLOCK3);
                else screen.setChar(x,y, TCOD_CHAR_BLOCK1);
            } 
            screen.setChar(x,y, TCOD_CHAR_ARROW2_S);
        }
        else
        {
            for(; x<bodyLength; ++x)
            {
                if(((x-origin.X())/pageLength == curPage) 
                || (curPage+1 == numPages && x >= bodyLength-extraTiles))
                    screen.setChar(x,y, TCOD_CHAR_BLOCK3);
                else screen.setChar(x,y, TCOD_CHAR_BLOCK1);
            } 
            screen.setChar(x,y, TCOD_CHAR_ARROW2_S);
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : MenuScreen::drawTabs
        Description : draws the tabs that sit on top of the folder structure.
        Inputs      : two ints representing the coordinates of the topleft of the
                      folder area.
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void MenuScreen::drawTabs(int& x, int& y, const vector< pair<int, TCODColor> >& tabChars) 
    {
        const int numTabs = static_cast<int>(tabChars.size());
        const int xstart = x;

        // draw the tops of the tabs
        for(; x<numTabs*TABWIDTH; ++x)
        {
            // if this is the left tab border
            if(x % TABWIDTH == 0) screen.setChar(x,y, TCOD_CHAR_NW);
            // else if this is the right tab border
            else if(x % TABWIDTH == TABWIDTH-1) screen.setChar(x,y, TCOD_CHAR_NE);
            else screen.setChar(x,y, TCOD_CHAR_HLINE);
        } 
        ++y; 
        x = xstart;

        // draw the next line
        for(int i=0; x<numTabs*TABWIDTH; ++x)
        {
            // if this is one of the tab borders
            if((x % TABWIDTH == 0) || (x % TABWIDTH == TABWIDTH-1)) 
                screen.setChar(x,y, TCOD_CHAR_VLINE);
            else if(x % TABWIDTH == TABWIDTH/2) 
            {
                screen.putCharEx(x,y, tabChars.at(i).first, 
                                 tabChars.at(i).second, UI_BACK_COLOR);
                ++i;
            }
        } 
        ++y; 
        x = xstart;
    }



    /*--------------------------------------------------------------------------------
        Function    : MenuScreen::update
        Description : Contains all the code needed to put the menu screen's contents
                      onto the root console
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void MenuScreen::update()
    {
        screen.clear();
        display->refresh();
        refresh();

        // now draw the screen on the playfield
        display->playfield()->blit(&screen, 0, 0, 
                                   screen.getWidth(), 
                                   screen.getHeight(),
                                   display->playfield().get(), 
                                   0, 0, 1.0f, 0.7f);

        display->draw();
        TCODConsole::flush();
    }



    /*--------------------------------------------------------------------------------
        Function    : MenuScreen::refresh
        Description : Displays the screen console on top of the playfield.
        Inputs      : None
        Outputs     : see description
        Return      : void
    --------------------------------------------------------------------------------*/
    void MenuScreen::refresh() 
    {
        // construct screen content
        screen.printEx(screen.getWidth()/2, 
                       screen.getHeight()/2, 
                       TCOD_BKGND_NONE, 
                       TCOD_CENTER, 
                       "Menu Screen");
    }



    /*--------------------------------------------------------------------------------
        Function    : MenuScreen::eventLoop
        Description : Governs the actions that the player can perform while viewing 
                      this menu screen.  In this base class, this is just a boiler
                      plate 'hello world' function, just to make sure that menu
                      screens work correctly.
        Inputs      : None
        Outputs     : Actions of the menu screen
        Return      : bool (whether the action performed in the menu screen consumes 
                      a turn)
    --------------------------------------------------------------------------------*/
    bool MenuScreen::eventLoop()
    {
        EventHandler eventHandler;

        do
        {
            screen.clear();
            display->refresh();
            refresh();

            // now draw the screen on the playfield
            display->playfield()->blit(&screen, 0, 0, 
                                       screen.getWidth(), 
                                       screen.getHeight(),
                                       display->playfield().get(), 
                                       0, 0, 1.0f, 0.7f);

            display->draw();
            TCODConsole::flush();
        }
        while(eventHandler.getPlayerInput() != CANCEL);

        return false;
    }



    /*--------------------------------------------------------------------------------
        Function    : InventoryScreen::refresh
        Description : draws the InventoryScreen to the game window
        Inputs      : None
        Outputs     : updated InventoryScreen
        Return      : void
    --------------------------------------------------------------------------------*/
    void InventoryScreen::refresh()
    {
        Point dim(screen.getWidth(), screen.getHeight());

        // set the colorscheme of the selected line
        TCODConsole::setColorControl(TCOD_COLCTRL_1, TCODColor::white, TCODColor::blue);

        // create a vector of the different item symbols
        vector< pair<int, TCODColor> > tabChars;
        tabChars.push_back(pair<int, TCODColor>('\\', UI_FORE_COLOR));
        tabChars.push_back(pair<int, TCODColor>('[', UI_FORE_COLOR));
        tabChars.push_back(pair<int, TCODColor>('!', UI_FORE_COLOR));
        tabChars.push_back(pair<int, TCODColor>('*', UI_FORE_COLOR));

        // draw interface
        int x=0, y=2;
        //screen.printEx(dim.X()/2, 0, TCOD_BKGND_SET, TCOD_CENTER, 
        //               (client->personalStats.name() + string("'s Inventory")).c_str());
        screen.printEx(dim.X()/2, 0, TCOD_BKGND_SET, TCOD_CENTER, "Inventory");

        // 'start' is the point at which the folder contents are drawn
        Point start = drawFolders(x, y, dim, tabChars);

        // draw the information line
        x = start.X();
        y = start.Y();
        screen.print(x+1, y++, "Qt.  Item");
        screen.hline(x, y++, dim.X()-3);

        // update start position to where items will be drawn
        start.setY(y);
        
        linesPerPage = dim.Y()-y;
        currentPage = (selectedLine==0) ? 0 : selectedLine / (linesPerPage-1);

        int numItems = client->inventory.getIndexSize(selectedTab);

        if(numItems <= 0) return;

        drawScrollBar(Point(dim.X()-2, y), linesPerPage, currentPage, 
                      (numItems/linesPerPage)+1);

        // determine at what line should be highlighted
        int hlOffset = selectedLine % ((currentPage==0) ? linesPerPage : linesPerPage-1); 

        // set an iterator to the lowest spot that will show the selected line
        vector<ItemPtr>::const_iterator it;
        it = client->inventory.getIndexBegin(selectedTab) + (currentPage*linesPerPage) - currentPage;

        for(; it!=client->inventory.getIndexEnd(selectedTab); ++it, ++y)
        {
            // ignore lines that are off the display area
            if(y >= start.Y()+linesPerPage-1) break;

            // construct the display string
            // three spaces are allocated to the item count number.  The buffer string
            // ensures that this number is right aligned in this space.
            string buffer = "";
            unsigned int c = (*it)->getCount();
            while(c<100) { buffer += " "; c *= 10;}
            string disp = buffer + boost::lexical_cast<string>((*it)->getCount()) + "  " + (*it)->getName();

            // if this is the selected line on the current page, highlight it
            if(y == (hlOffset) + start.Y()) 
            {
                disp = "%c" + disp + "%c";
                screen.printEx(start.X(), y, TCOD_BKGND_SET, TCOD_LEFT, 
                               disp.c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
            }
            else // display the line normally
            {
                screen.printEx(start.X(), y, TCOD_BKGND_SET, TCOD_LEFT, disp.c_str()); 
            }
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : InventoryScreen::interpretEvent
        Description : Given an event, this function executes the appropriate action.
        Inputs      : EventType
        Outputs     : Results of the passed event.
        Return      : void
    --------------------------------------------------------------------------------*/
    void InventoryScreen::interpretEvent(const EventType event)
    {
        switch(event)
        {
            case CANCEL:
            case EXIT:
            {
                keepRunning = false;
                break;
            }
            case ITEM:
            {
                ItemPtr item = client->inventory.removeItem(selectedTab, selectedLine);
                item->setPosition(client->getPosition());
                Level::getCurrentLevel()->addItem(item);
                break;
            }
            case MOVE_NORTH:
            {
                decSelectedLine(client->inventory.getIndexSize(selectedTab));
                break;
            }
            case MOVE_SOUTH:
            {
                incSelectedLine(client->inventory.getIndexSize(selectedTab));
                break;
            }
            case MOVE_EAST:
            {
                incSelectedTab(client->inventory.getSize());
                break;
            }
            case MOVE_WEST:
            {
                decSelectedTab(client->inventory.getSize());
                break;
            }
            default: break;
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : InventoryScreen::eventLoop
        Description : Event loop for the inventory screen.
        Inputs      : None
        Outputs     : Results of the player's actions in the inventory.
        Return      : bool (whether the action costs a turn)
    --------------------------------------------------------------------------------*/
    bool InventoryScreen::eventLoop()
    {
        EventHandler eventHandler;
        EventType event;

        do
        {
            update();
            event = eventHandler.getPlayerInput();
            interpretEvent(event);
        }
        while(keepRunning);

        return false;
    }
}
