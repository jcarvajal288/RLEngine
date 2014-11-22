#ifndef RLNS_VITALSTATS_HPP
#define RLNS_VITALSTATS_HPP

#include <fstream>
#include <vector>

#include "AbilityScore.hpp"
#include "Dice.hpp"
#include "Types.hpp"

namespace rlns
{
    class VitalStats
    {
        public:
            VitalStats()
            : name(""), title(""), occupation(""),
              //alignment(AlignmentType::LAWFUL),
              //characterClass(CharacterClassType::WARRIOR),
              //level(0), xp(0),
              STR(0), AGI(0), STA(0), PER(0), INT(0), LUCK(0) {}

            AbilityScore getSTR() const { return STR; }
            AbilityScore getAGI() const { return AGI; }
            AbilityScore getSTA() const { return STA; }
            AbilityScore getPER() const { return PER; }
            AbilityScore getINT() const { return INT; }
            AbilityScore getLUCK() const { return LUCK; }

            Score getHP() const { return hp; }

            std::string getOccupation() const { return occupation; }

            void generate();

        private:
            void rollAbilityScores();
            void chooseOccupation();

            std::string name;
            std::string title;
            std::string occupation;
            //AlignmentType alignment;
            //CharacterClassType characterClass;
            //int level;
            //int xp;

            AbilityScore STR;
            AbilityScore AGI;
            AbilityScore STA;
            AbilityScore PER;
            AbilityScore INT;
            AbilityScore LUCK;

            Score hp;

            std::string luckyRoll;
    };
}

#endif
