#include "AbstractTile.hpp"

using namespace std;

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : AbstractTile::saveToDisk
        Description : Saves the AbstractTile's data members into the given RLNSZip 
                      buffer.
        Inputs      : RLNSZip buffer
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void AbstractTile::saveToDisk(RLNSZip& zip) const
    {
        zip.putInt(character);
        zip.putColor(&fgColor);
        zip.putColor(&bgColor);
        zip.putString(shortdesc.c_str());
        zip.putString(longdesc.c_str());
    }
}
