#include "Dice.hpp"

namespace rlns
{
    DieRollerPtr DieRoller::instance;

    /*--------------------------------------------------------------------------------
        Function    : DieRoller::get
        Description : Public access to the private DieRoller instance, as part of the
                      Singleton pattern
        Inputs      : None
        Outputs     : None
        Return      : DieRollerPtr
    --------------------------------------------------------------------------------*/
    DieRollerPtr DieRoller::get()
    {
        if(instance.get() == 0)
        {
            RNGType rng;
            rng.seed(static_cast<const unsigned int>(time(0)));
            instance.reset(new DieRoller(rng));
        }
        return instance;
    }



    DieRoll DieRoller::roll(int die)
    {
        if (die < D3) die = D3;
        else if (die > D30) die = D30;

        switch(die)
        {
            case D3:
                return DieRoll(D3, d3());        
            case D4:
                return DieRoll(D4, d4());        
            case D5:
                return DieRoll(D5, d5());        
            case D6:
                return DieRoll(D6, d6());        
            case D7:
                return DieRoll(D7, d7());        
            case D8:
                return DieRoll(D8, d8());        
            case D10:
                return DieRoll(D10, d10());        
            case D12:
                return DieRoll(D12, d12());        
            case D14:
                return DieRoll(D14, d14());        
            case D16:
                return DieRoll(D16, d16());        
            case D20:
                return DieRoll(D20, d20());        
            case D24:
                return DieRoll(D24, d24());        
            case D30:
                return DieRoll(D30, d30());        
            default:
                // TODO: put proper error handling here
                return DieRoll(D3, d3());
        }
    }



    DieRoll rollDie(const DieType die, const int mod)
    {
        return DieRoller::get()->roll(die + mod);
    }

    int rollDice(const int num, const DieType die)
    {
        int sum = 0;
        for(int i=0; i<num; ++i)
        {
            sum += rollDie(die).result();
        }
        return sum;
    }

    int randomInt(const int lower, const int upper)
    {
        SmallIntDistribution dist(lower, upper);
        return dist(DieRoller::get()->rngType);
    }
}
