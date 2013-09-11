#include "AbilityScore.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : AbilityScore::mod
        Description : Returns the modifier for the Ability Score.
        Inputs      : None
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    int AbilityScore::mod() const
    {
        switch(current)
        {
            case 3:
                return -3;
            case 4:
            case 5:
                return -2;
            case 6:
            case 7:
            case 8:
                return -1;
            case 9:
            case 10:
            case 11:
            case 12:
                return 0;
            case 13:
            case 14:
            case 15:
                return 1;
            case 16:
            case 17:
                return 2;
            case 18:
                return 3;
            default:
                // due to validation, this should never happen
                return 0;
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : validateScore
        Description : Returns a number between 3 and 18, given a number that may not be.
                      Ability Scores must be between these numbers.
        Inputs      : None
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    int validateScore(const int n)
    {
        if(n < 3) 
        {
            return 3;
        }
        else if (n > 18) 
        {
            return 18;
        }
        else
        {
            return n;
        }
    }
}
