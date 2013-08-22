#include "MessageTracker.hpp"

using namespace std;

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : MessageTracker::addMessage
        Description : Adds a new Message object to the front of the message log. If 
                      the new size of the message log is larger than the maximum size,
                      it deletes the oldest message.
        Inputs      : Content of the message, the foreground color, the background
                      color
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void MessageTracker::addMessage(const string& str, const TCODColor& fg, const TCODColor& bg)
    {
        MessagePtr message(new Message(str, fg, bg));
        messageLog.push_front(message);

        if(messageLog.size() > logSize)
        {
            messageLog.pop_back();
        }
    }
}
