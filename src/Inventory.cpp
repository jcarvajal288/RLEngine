#include "Inventory.hpp"

using namespace std;

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : Inventory::Inventory
        Description : Default constructor for the Inventory class.
        Inputs      : None
        Outputs     : None
        Return      : None (constructor)
    --------------------------------------------------------------------------------*/
    Inventory::Inventory() 
    : contents(NUM_INDICES, vector<ItemPtr>()) {}



    /*--------------------------------------------------------------------------------
        Function    : Inventory::addItem
        Description : Adds the input item to the appropriate vector in the inventory.
        Inputs      : ItemPtr
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void Inventory::addItem(const ItemPtr newItem)
    {
        // TODO: add sorting code based on Item subclass
        //       for now, just add it to miscItems

        contents.at(MISC).push_back(newItem);
    }



    /*--------------------------------------------------------------------------------
        Function    : Inventory::removeItem
        Description : Removes an item from the indicated category and index and 
                      returns it.
        Inputs      : target category, target index
        Outputs     : None
        Return      : ItemPtr
    --------------------------------------------------------------------------------*/
    ItemPtr Inventory::removeItem(const int cat, const int index)
    {
        ItemPtr target(contents.at(cat).at(index));

        vector<ItemPtr>::iterator begin = contents.at(cat).begin();
        contents.at(cat).erase(begin + index);

        return target;
    }



    /*--------------------------------------------------------------------------------
        Function    : Inventory::getIndexSize
        Description : Returns the number of items present in the given inventory
                      category.
        Inputs      : Index to query
        Outputs     : possible error message
        Return      : size_t
    --------------------------------------------------------------------------------*/
    size_t Inventory::getIndexSize(const int i) const
    {
        if(contents.size() == 0) return 0;

        return contents.at(i).size();
    }
}
