#include "Utility.hpp"

using namespace std;

namespace rlns
{
    namespace utl
    {
        void fatalError(const string& msg)
        {
            cerr << "FATAL ERROR: " << msg << endl;
            abort();
        }


        int relDistance(const Point& a, const Point& b)
        {
            return pow(b.getX()-a.getX(), 2) + pow(b.getY()-a.getY(), 2);
        }

        void printColor(const TCODColor& c)
        {
            cerr << "(" << static_cast<int>(c.r) << "," 
                        << static_cast<int>(c.g) << "," 
                        << static_cast<int>(c.b) << ")" << endl;
        }
    }
}
