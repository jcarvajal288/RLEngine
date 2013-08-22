#ifndef RLNS_ABILITYSCORE_HPP
#define RLNS_ABILITYSCORE_HPP

namespace rlns
{
    class AbilityScore
    {
        public:
            AbilityScore()
            : max(0), current(0) {}

            AbilityScore(const int n)
            : max(n), current(n) {}

            int getCurrent() const { return current; }
            int getMax() const { return max; }

            void setMaxAndCurrent(const int n) { max = n; current = n; }

        private:
            int max;
            int current;
    };
}

#endif
