#include "Utility.hpp"

using namespace std;

namespace rlns
{
   /*-------------------------------------------------------------------------------- 
        Function    : fatalError
        Description : Prints an error message to stderr, then exits the program
        Inputs      : error message
        Outputs     : error message
        Return      : void
    --------------------------------------------------------------------------------*/
    void fatalError(const string& msg)
    {
        cerr << "FATAL ERROR: " << msg << endl;
        abort();
    }



    /*--------------------------------------------------------------------------------
        Function    : getKeypress
        Description : Depending on various flags, this function either waits for a key
                      to be pressed or it returns a TCODK_NONE indicating no key has
                      been pressed.
        Inputs      : None
        Outputs     : None
        Return      : TCOD_key_t
    --------------------------------------------------------------------------------*/
    TCOD_key_t getKeypress() 
    {
        if(LIGHT_FLICKER_ENABLED)
        {
            // this allows the screen to update while waiting for the player
            // to press a key.
            return TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
        }
        else
        {
            return TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
            // currently waitForKeypress triggers on key released events too,
            // causing movement doubling
            //return TCODConsole::waitForKeypress(true);
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : relDistance (relative distance)
        Description : performs the distance formula, minus the square root, on two
                      Points.  Useful for comparing distances while avoiding the
                      costly square root function.
        Inputs      : two Point objects
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    int relDistance(const Point& a, const Point& b)
    {
        int result;

    #ifdef _WIN32
        // Visual C++ has some rather silly overloads for pow -_-
        float first = static_cast<float>(b.X() - a.X());
        float second = static_cast<float>(b.Y() - a.Y());
        result = static_cast<int>(pow(first, 2) + pow(second, 2));
    #else
        result = pow(b.X() - a.X(), 2) + pow(b.Y() - a.Y(), 2);
    #endif

        return result;
    }



    /*--------------------------------------------------------------------------------
        Function    : printColor
        Description : Convenience function for printing a TCODColor.
        Inputs      : TCODColor
        Outputs     : TCODColor printed in (r,g,b) format
        Return      : void
    --------------------------------------------------------------------------------*/
    void printColor(const TCODColor& c)
    {
        cerr << "(" << static_cast<int>(c.r) << "," 
                    << static_cast<int>(c.g) << "," 
                    << static_cast<int>(c.b) << ")" << endl;
    }



    /*--------------------------------------------------------------------------------
        Function    : straightLineBetweenPoints
        Description : Uses libtcod's bresenham toolkit to fill a vector of points
                      width the straight line between the two provided points.
        Inputs      : result vector, origin point, destination point
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void findStraightLineBetweenPoints(vector<Point>* result, const Point& p1, const Point& p2)
    {
        result->clear();

        int x = p1.X(), y = p1.Y();
        TCODLine::init(x, y, p2.X(), p2.Y());
        do
        {
            result->push_back(Point(x,y));
        } 
        while(!TCODLine::step(&x, &y));
    }



    /*--------------------------------------------------------------------------------
        Function    : signcos2
        Description : Returns the square of the cosine of the angle between vectors
                      p0->p1 and p1->p2, with the sign of the cosine, in permil
                      (1.0 = 1000) This function is taken from the winding road
                      algorithm by Kusigrosz.  See MapBuilder.hpp for the source.
        Inputs      : coordinates of the three points
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    int signcos2(const int x0, const int y0, const int x1, const int y1, const int x2, const int y2)
    {
        int sqlen01, sqlen12, prod, val;
        
        sqlen01 = (x1 - x0)*(x1 - x0) + (y1 - y0)*(y1 - y0);
        sqlen12 = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);

        prod = (x1 - x0) * (x2 - x1) + (y1 - y0) * (y2 - y1);
        val = 1000 * (prod * prod / sqlen01) / sqlen12; /* Overflow? */
        if (prod < 0)
        {
            val = -val;
        }

        return val;
    }
}
