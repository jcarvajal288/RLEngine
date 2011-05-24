#ifndef RLNS_ACTOR_HPP
#define RLNS_ACTOR_HPP

#include <iostream>
#include <vector>

#include "CheckedSave.hpp"
#include "Light.hpp"
#include "AbstractTile.hpp"
#include "Point.hpp"
#include "Utility.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : Actor
            Description : Represents players, creatures, etc.  Essentially anything that
                          can move.
            Parents     : AbstractTile
            Children    : None (yet?)
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Actor: public AbstractTile
        {
            // Member Variables
            private:
                utl::Point position;
                std::vector<int> ownedLights; // indices into the object's level's lights map

            // Member Functions
            private:
                void moveLights();

            public:
                Actor(const utl::Point& p, const int ch, const TCODColor& col)
                : AbstractTile(ch, col, TCODColor::fuchsia, "",""), position(p) {}

                Actor(utl::RLNSZip& zip); // used for save game loading

                virtual ~Actor() {}

                utl::Point getPosition() const { return position; }
                void setPosition(const utl::Point& p);
                std::vector<int> getLights() const { return ownedLights; }

                void attachLight(const int);

                virtual const char* description() const { return "an actor"; }

                virtual void move(const DirectionType dir);

                void saveToDisk(utl::RLNSZip& zip) const;
        };


        // inline functions
        inline void Actor::setPosition(const utl::Point& p)
        {
            position = p;
            moveLights();
        }
    }
}

#endif
