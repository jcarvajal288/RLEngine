#include "Point.hpp"
#include <iostream>

namespace rlns
{
    namespace utl
    {
        /*--------------------------------------------------------------------------------
            Function    : Point::shift
            Description : shifts the point one tile in the given direction.
                          Since the upper left corner is (0,0), moving North decrements y,
                          while moving South increments it.
            Inputs      : direction enum 
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Point::shift(DirectionType direction)
        {
            //std::cout << "shift" << std::endl;
            switch(direction)
            {
                case DirectionType::NORTH:
                    --y;
                    break;
                case DirectionType::NORTHEAST:
                    ++x; 
                    --y;
                    break;
                case DirectionType::EAST:
                    ++x;
                    break;
                case DirectionType::SOUTHEAST:
                    ++x;
                    ++y;
                    break;
                case DirectionType::SOUTH:
                    ++y;
                    break;
                case DirectionType::SOUTHWEST:
                    --x;
                    ++y;
                    break;
                case DirectionType::WEST:
                    --x;
                    break;
                case DirectionType::NORTHWEST:
                    --x;
                    --y;
                    break;
                case DirectionType::CENTER:
                case DirectionType::NO_DIRECTION:
                    return;
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Point::withinBounds
            Description : checks if this Point is within the boundaries created by two
                          other Points.
            Inputs      : two Points
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool Point::withinBounds(const Point& TL, const Point& BR) const
        {
            if(x < TL.getX()) return false;
            if(x > BR.getX()) return false;
            if(y < TL.getY()) return false;
            if(y > BR.getY()) return false;

            // Success
            return true;
        }



        /*--------------------------------------------------------------------------------
            Function    : Point::withinBounds
            Description : checks if this point is within the boundaries created by four
                          integer variables.
            Inputs      : left int, top int, right int, bottom int
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool Point::withinBounds(const int l, const int t, const int r, const int b) const
        {
            if(x < l) return false;
            if(x > r) return false;
            if(y < t) return false;
            if(y > b) return false;

            // Success
            return true;
        }
    }
}
