#ifndef RLNS_VITALSTATS_HPP
#define RLNS_VITALSTATS_HPP

#include <pair>

#include "AbilityScore.hpp"
#include "Types.hpp"

namespace rlns
{
    class VitalStats
    {
        public:
            VitalStats()
            : name(""), title(""), occupation(""),
              alignment(AlignmentType::LAWFUL),
              characterClass(CharacterClassType::WARRIOR),
              level(0), xp(0),
              STR(0,0), AGL(0,0), STA(0,0)

            void generate();

        private:
            void rollAbilityScores();

            std::string name;
            std::string title;
            std::string occupation;
            AlignmentType alignment;
            CharacterClassType characterClass;
            int level;
            int xp;

            // pairs are organized as <max, current>
            AbilityScore STR;
            AbilityScore AGI;
            AbilityScore STA;
            AbilityScore PER;
            AbilityScore INT;
            AbilityScore LUCK;

            AbilityScore hp;

            std::string luckyRoll;
    };
}

#endif
