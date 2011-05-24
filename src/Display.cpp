#include "Display.hpp"

namespace rlns
{
    namespace view
    {
        DisplayPtr Display::_instance(new Display());

        /*--------------------------------------------------------------------------------
            Function    : Display::get
            Description : Public access to the private Display instance.
            Inputs      : None
            Outputs     : None
            Return      : DisplayPtr
        --------------------------------------------------------------------------------*/
        DisplayPtr Display::get()
        {
            if(_instance->initialized == false)
            {
                _instance.reset(new Display(true));
            }
            return _instance;
        }



        /*--------------------------------------------------------------------------------
            Function    : Display::gameWindowCoords
            Description : Fetches the topleft and bottomright points of the GameWindow.
            Inputs      : None
            Outputs     : None
            Return      : std::pair<Point, Point>
        --------------------------------------------------------------------------------*/
        std::pair<utl::Point, utl::Point> Display::gameWindowCoords() const
        {
            return std::make_pair(gameWindow.topLeft(), gameWindow.bottomRight());
        }



        /*--------------------------------------------------------------------------------
            Function    : Display::drawCommandPrompt
            Description : Displays a prompt line to the player.  Command prompt lines are
                          transitory and disappear with each screen refresh.  They are
                          used to communicate instructions or questions to the player.
            Inputs      : None
            Outputs     : Prints the prompt above the Console window.
            Return      : void
        --------------------------------------------------------------------------------*/
        void Display::drawCommandPrompt()
        {
            TCODConsole::root->print(0, InitData::get()->getCommandLine(), commandLine.c_str());
            commandLine = "";
        }
    }
}
