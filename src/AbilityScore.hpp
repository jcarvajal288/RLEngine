#ifndef RLNS_ABILITYSCORE_HPP
#define RLNS_ABILITYSCORE_HPP

namespace rlns
{
    class Score
    {
        public:
            Score()
            : max(0), current(0) {}

            Score(const int n)
            : max(n), current(n) {}

            int getCurrent() const { return current; }
            int getMax() const { return max; }

            void setMaxAndCurrent(const int n) { max = n; current = n; }

        protected:
            int max;
            int current;
    };


    int validateScore(const int n);

    class AbilityScore: public Score
    {
        public:
            AbilityScore()
            : Score(10) {}
            
            AbilityScore(const int n)
            : Score(validateScore(n)) {}

            int mod() const;
            void setMaxAndCurrent(const int n) { max = validateScore(n); current = max; }
    };
}

#endif
