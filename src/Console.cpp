#include "Console.hpp"

using namespace std;
using namespace rlns::utl;

namespace rlns
{
    namespace view
    {
        /*--------------------------------------------------------------------------------
            Function    : Console::addMessage
            Description : Adds a new Message object to the front of the message log.
            Inputs      : content of the message, the foreground color, the background
                          color
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Console::addMessage(const std::string& s, const TCODColor& fc, const TCODColor& bc)
        {
            MessagePtr message(new Message(s, fc, bc));
            messageLog.push_front(message);
            if(messageLog.size() > logSize) messageLog.pop_back();
        }



        /*--------------------------------------------------------------------------------
            Function    : Console::draw
            Description : draws the Console, including its frame, to the root console.
                          Messages are printed in chronological order, from the top of the
                          Console to the bottom.
            Inputs      : None
            Outputs     : the Console and its messages to the root console.
            Return      : void
        --------------------------------------------------------------------------------*/
        void Console::draw() const
        {
            Point TL = topLeft(), BR = bottomRight();
            int x = TL.getX(), y = TL.getY();
            int width = BR.getX() - TL.getX();
            int height = BR.getY() - TL.getY();

            // draw the Console frame
            TCODConsole::root->printFrame(x, y, width, height, false);
            TCODConsole::root->printEx(x+(width/2), y, 
                                       TCOD_BKGND_NONE, 
                                       TCOD_CENTER, 
                                       " MESSAGE LOG ");

            // change the dimensions to fit the text inside the frame
            ++x; ++y; width -= 2; --height;

            // print the Console's messages
            int count = 0;
            int textHeight;
            list<MessagePtr>::const_iterator it, end;
            end = messageLog.end();
            for(it=messageLog.begin(); it!=end; ++it)
            {
                string message = "%c" + (*it)->getText() + "%c";
                TCODConsole::setColorControl(TCOD_COLCTRL_1, 
                                             (*it)->getForeColor(),
                                             (*it)->getBackColor());
                textHeight = TCODConsole::root->getHeightRect(x, y, width, height,
                                                              message.c_str(),
                                                              TCOD_COLCTRL_1,
                                                              TCOD_COLCTRL_STOP);
                if(count+textHeight > height-1) 
                    break;
                TCODConsole::root->printRectEx(x, y, width, height,
                                               TCOD_BKGND_SET,
                                               TCOD_LEFT,
                                               message.c_str(),
                                               TCOD_COLCTRL_1,
                                               TCOD_COLCTRL_STOP);
                y += textHeight;
                count += textHeight;
            }
        }



////////////////////////
// PartyWindow Functions
////////////////////////



        /*--------------------------------------------------------------------------------
            Function    : PartyWindow::drawInfoCard
            Description : draws the info card for each member of the party.  An info card
                          is a brief description of a character's status, consisting of 
                          his name, his health and sanity levels, and any status effects
                          he is affected by.
            Inputs      : the upper left corner of the info card, and a shared_ptr to a 
                          Creature object
            Outputs     : the info card on the root console
            Return      : topleft point of the next info card
        --------------------------------------------------------------------------------/
        Point PartyWindow::drawInfoCard(const Point& cardTL, CreaturePtr client) const
        {
            int x = cardTL.getX(), y = cardTL.getY();
            int frameWidth = BR.getX()-x+1;
            int frameHeight = y+CARDHEIGHT;

            // draw the card frames, and highlight the currently selected party member
            if(clients->members.at(clients->leaderPosition()) == client)
            {
                TCODConsole::root->setBackgroundColor(TCODColor::darkGrey);
                TCODConsole::root->printFrame(x, y, frameWidth, frameHeight, true,
                                              TCOD_BKGND_SET, NULL);
            }
            else 
            {
                TCODConsole::root->setBackgroundColor(TCODColor::darkerGrey);
                TCODConsole::root->printFrame(x, y, frameWidth, frameHeight, true);
            }

            // draw the information
            ++x; ++y; // put the corner dimensions inside the frame

            // draw the character's name
            TCODConsole::root->printEx(x+(width/2), y++, TCOD_BKGND_SET, TCOD_CENTER, 
                                       client->personalStats.name().c_str());

            // draw a horizontal dividing line
            TCODConsole::root->hline(x, y++, width-1);

            // draw Health status
            pair<string, TCODColor> status = client->vitalStats.getHealthStatus(true);
            string fmt = "Hth:%c" + status.first + "%c";
            TCODConsole::setColorControl(TCOD_COLCTRL_1, status.second, 
                                         TCODConsole::root->getBackgroundColor());
            TCODConsole::root->printEx(x, y++, TCOD_BKGND_DEFAULT, TCOD_LEFT,
                                       fmt.c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);

            // draw Sanity status
            status = client->vitalStats.getSanityStatus(false);
            fmt = "San:%c" + status.first + "%c";
            TCODConsole::setColorControl(TCOD_COLCTRL_1, status.second, 
                                         TCODConsole::root->getBackgroundColor());
            TCODConsole::root->printEx(x, y++, TCOD_BKGND_DEFAULT, TCOD_LEFT,
                                       fmt.c_str(), TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);

            // TODO: draw Status Effects in here 

            // change background color back to default
            TCODConsole::root->setBackgroundColor(UIBackColor);

            return Point(cardTL.getX(), cardTL.getY()+CARDHEIGHT);
        }

        

        *--------------------------------------------------------------------------------
            Function    : PartyWindow::draw
            Description : draws the PartyWindow to the screen during refreshes.
            Inputs      : None
            Outputs     : updated PartyWindow to the root console
            Return      : void
        --------------------------------------------------------------------------------/
        void PartyWindow::draw() const
        {
            Point cardTL = TL;

            vector<CreaturePtr>::const_iterator it;
            for(it=clients->members.begin(); it!=clients->members.end(); ++it)
                cardTL = drawInfoCard(cardTL, *it);
        }*/
    }
}

