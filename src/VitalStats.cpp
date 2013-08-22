#include "VitalStats.hpp"

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : VitalStats::generate
        Description : Starts the character generation process
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void VitalStats::generate()
    {
        rollAbilityScores();
    }



    /*--------------------------------------------------------------------------------
        Function    : VitalStats::rollAbilityScores
        Description : Rolls each ability score using 3d6.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void VitalStats:rollAbilityScores()
    {
        STR.setMaxAndCurrent(DiceRoller->rollDice(3, DiceType::D6).result());
        AGI.setMaxAndCurrent(DiceRoller->rollDice(3, DiceType::D6).result());
        STA.setMaxAndCurrent(DiceRoller->rollDice(3, DiceType::D6).result());
        PER.setMaxAndCurrent(DiceRoller->rollDice(3, DiceType::D6).result());
        INT.setMaxAndCurrent(DiceRoller->rollDice(3, DiceType::D6).result());
        LUCK.setMaxAndCurrent(DiceRoller->rollDice(3, DiceType::D6).result());
    }
}
