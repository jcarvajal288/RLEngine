#ifndef RLNS_MAPOBJECT_HPP
#define RLNS_MAPOBJECT_HPP

#include <string>

#include "AbstractTile.hpp"
#include "Point.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : MapObject
            Description : Any object that needs to be drawn on the map that is not a
                          terrain tile.  This is a child of AbstractTile that just adds
                          a position variable.
            Parents     : AbstractTile
            Children    : Actor, Item
            Friends     : None
        --------------------------------------------------------------------------------*/
        class MapObject: public AbstractTile
        {
            // Member Variables
            private:
                utl::Point position;

            public:
                MapObject(const int c, 
                          const TCODColor& fg, 
                          const TCODColor& bg,
                          const std::string& sd,
                          const std::string& ld,
                          const utl::Point& pos)
                : AbstractTile(c, fg, bg, sd, ld),
                  position(pos) {}

                utl::Point getPosition() const { return position; }
                void setPosition(const utl::Point&);
        };


        // Inline Functions

        inline void MapObject::setPosition(const utl::Point& pt)
        {
            position = pt;
        }


        // Function Predicates

        // tests if the given MapObject's position is equal to the point 
        // supplied in the constructor
        class PositionEquals
        {
            private:
                utl::Point pt;

            public:
                PositionEquals(const utl::Point& p)
                : pt(p) {}

                bool operator()(const MapObjectPtr object) const
                {
                    return object->getPosition() == pt;
                }
        };
    }
}

#endif
