#ifndef RLNS_ABSTRACTTILE_HPP
#define RLNS_ABSTRACTTILE_HPP

#include <string>
#include <vector>

#include "CheckedSave.hpp"
#include "Light.hpp"
#include "Utility.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : AbstractTile
            Description : Represents any object in the game.  This is the base class for
                          Actors, Items, etc.

            Members     : character - the ascii representation of this object.
                          fgColor - what color the object's character is displayed as.
                          bgColor - what color the background of the object's tile is.
                          shortdesc - a brief blurb describing the object.
                          longdesc - a longer, possibly more flowerly description for flavor.
                          ownedLights - which light sources the object currently controls.
                                If the Object is an actor, for instance, than this might
                                be torches or other lights that the actor is carrying.
            Parents     : None
            Children    : Actor, Feature, Item
            Friends     : None
        --------------------------------------------------------------------------------*/
        class AbstractTile
        {
            // Member Variables
            private:
                int character;
                TCODColor fgColor;
                TCODColor bgColor;
                std::string shortdesc;
                std::string longdesc;


            // Member Functions
            public:
                AbstractTile(const int c, 
                             const TCODColor& fg, 
                             const TCODColor& bg,
                             const std::string& sd,
                             const std::string& ld)
                : character(c), fgColor(fg), bgColor(bg), shortdesc(sd), longdesc(ld) {}

                AbstractTile(utl::RLNSZip& zip) // used for save game loading
                : character(zip.getInt()), fgColor(zip.getColor()), bgColor(zip.getColor()),
                  shortdesc(std::string(zip.getString())), longdesc(std::string(zip.getString())) {}

                virtual ~AbstractTile() {}

                int getChar() const { return character; }
                TCODColor getFgColor() const { return fgColor; }
                TCODColor getBgColor() const { return bgColor; }

                std::string getShortDesc() const { return shortdesc; }
                std::string getLongDesc() const { return longdesc; }

                virtual void saveToDisk(utl::RLNSZip&) const;
        };
    }
}

#endif
