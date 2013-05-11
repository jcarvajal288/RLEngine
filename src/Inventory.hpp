#ifndef RLNS_INVENTORY_HPP
#define RLNS_INVENTORY_HPP

#include <vector>

#include "Types.hpp"

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Class       : Inventory
            Description : Manages all of the carried items and equipment for a character
                          or monster.  This class is a component of the Actor class.  The
                          inventory is subdivided into items of different types for
                          easier sorting.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Inventory
        {
            // Member Variables
            private:
                enum
                {
                    MISC = 0,
                    NUM_INDICES = 1
                } index;

                std::vector< std::vector<ItemPtr> > contents;

            // Member Functions
            public:
                Inventory();

                void addItem(const ItemPtr);
                ItemPtr removeItem(const int, const int);

                size_t getSize() const
                { return NUM_INDICES; }

                size_t getIndexSize(const int) const;

                std::vector<ItemPtr>::iterator getIndexBegin(const int i)
                { return contents.at(i).begin(); }

                std::vector<ItemPtr>::iterator getIndexEnd(const int i)
                { return contents.at(i).end(); }

                std::vector<ItemPtr> getMiscItems() const
                { return contents.at(MISC); }
        };
    }
}

#endif
