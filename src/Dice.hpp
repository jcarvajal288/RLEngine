#ifndef RLNS_DICE_HPP
#define RLNS_DICE_HPP

#include <iostream>

#include "boost/random.hpp"
#include "boost/shared_ptr.hpp"

#include "Types.hpp"

namespace rlns
{
    typedef boost::random::mt19937 RNGType;
    typedef boost::random::uniform_smallint<> SmallIntDistribution;
    typedef boost::variate_generator<RNGType, SmallIntDistribution> GenType;

    enum DieType
    {
        D3 = 3,
        D4 = 4,
        D5 = 5,
        D6 = 6,
        D7 = 7,
        D8 = 8,
        D10 = 10,
        D12 = 12,
        D14 = 14,
        D16 = 16,
        D20 = 20,
        D24 = 24,
        D30 = 30
    };

    class DieRoll
    {
        public:
            DieRoll(const DieType d, const int n)
            : number(n), critical(n == d) {}

            int result() const { return number; }

            bool isCritSuccess() const { return critical; }

            bool isCritFail() const { return number == 1; }

        private:
            int number;
            bool critical;
            DieRoll();
    };

    class DieRoller
    {
        public:
            friend DieRoll rollDie(const DieType, const int);
            friend int randomInt(const int, const int);


        private:
            RNGType rngType;
            DieRoll roll(int);
            static DieRollerPtr get();

            DieRoller(RNGType rng)
            : rngType(rng),
              d3(rng, SmallIntDistribution(1,3)),
              d4(rng, SmallIntDistribution(1,4)),
              d5(rng, SmallIntDistribution(1,5)),
              d6(rng, SmallIntDistribution(1,6)),
              d7(rng, SmallIntDistribution(1,7)),
              d8(rng, SmallIntDistribution(1,8)),
              d10(rng, SmallIntDistribution(1,10)),
              d12(rng, SmallIntDistribution(1,12)),
              d14(rng, SmallIntDistribution(1,14)),
              d16(rng, SmallIntDistribution(1,16)),
              d20(rng, SmallIntDistribution(1,20)),
              d24(rng, SmallIntDistribution(1,24)),
              d30(rng, SmallIntDistribution(1,30)) {}

            static DieRollerPtr instance;

            GenType d3;
            GenType d4;
            GenType d5;
            GenType d6;
            GenType d7;
            GenType d8;
            GenType d10;
            GenType d12;
            GenType d14;
            GenType d16;
            GenType d20;
            GenType d24;
            GenType d30;
    };

    DieRoll rollDie(const DieType, const int mod=0);

    int rollDice(const int, const DieType);

    int randomInt(const int, const int);
}

#endif
