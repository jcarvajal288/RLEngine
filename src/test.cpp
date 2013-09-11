#include <iostream>

#include "VitalStats.hpp"

using namespace std;
using namespace rlns;

int main()
{
    VitalStats stats;
    stats.generate();
    cout << stats.getSTR().getCurrent() << endl;
    cout << stats.getAGI().getCurrent() << endl;
    cout << stats.getSTA().getCurrent() << endl;
    cout << stats.getPER().getCurrent() << endl;
    cout << stats.getINT().getCurrent() << endl;
    cout << stats.getLUCK().getCurrent() << endl;

    cout << stats.getOccupation() << endl;
    cout << stats.getHP().getCurrent() << endl;
}
