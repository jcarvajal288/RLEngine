#include "VitalStats.hpp"

using namespace std;

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
        chooseOccupation();
        hp.setMaxAndCurrent(rollDice(1, D4) + getSTA().mod());
    }



    /*--------------------------------------------------------------------------------
        Function    : VitalStats::rollAbilityScores
        Description : Rolls each ability score using 3d6.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void VitalStats::rollAbilityScores()
    {
        STR.setMaxAndCurrent(rollDice(3, D6));
        AGI.setMaxAndCurrent(rollDice(3, D6));
        STA.setMaxAndCurrent(rollDice(3, D6));
        PER.setMaxAndCurrent(rollDice(3, D6));
        INT.setMaxAndCurrent(rollDice(3, D6));
        LUCK.setMaxAndCurrent(rollDice(3, D6));
    }



    /*--------------------------------------------------------------------------------
        Function    : VitalStats::chooseOccupation
        Description : Randomly chooses an occupation from occupations.txt
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void VitalStats::chooseOccupation()
    {
        ifstream occupationsFile;
        occupationsFile.open("./datafiles/occupations.txt");

        vector<string> lines;
        string line;
        while(occupationsFile.good())
        {
            getline(occupationsFile, line);
            lines.push_back(line);
        }
        occupationsFile.close();

        int i = randomInt(0, lines.size());
        occupation = lines.at(i); 
    }
}
