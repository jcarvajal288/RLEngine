#ifndef RLNS_UTILITY_HPP
#define RLNS_UTILITY_HPP

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

#include "CheckedSave.hpp"
#include "Point.hpp"

namespace rlns
{
    void fatalError(const std::string&);

    TCOD_key_t getKeypress();

    int relDistance(const Point&, const Point&);

    void printColor(const TCODColor&);

    void findStraightLineBetweenPoints(std::vector<Point>* result, const Point&, const Point&);

    int signcos2(const int , const int , const int , const int , const int , const int);




// save buffer template functions

    /*--------------------------------------------------------------------------------
        Function    : saveContainer 
        Description : saves a container of typename T to the given save buffer.  An
                      overloaded version for containers of shared_ptrs exist, since their
                      iterators require another degree of indirection.
        Inputs      : typename T container, TCODzip buffer
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    template<typename container>
    void saveContainer(const container& c, RLNSZip& zip)
    {
        // save the size of the container
        zip.putInt(static_cast<int>(c.size()));

        // now save the container itself
        typename container::const_iterator cit;
        for(cit=c.begin(); cit!=c.end(); ++cit) cit->saveToDisk(zip);
    }
    template<typename container>
    void saveSharedPtrContainer(const container& c, RLNSZip& zip)
    {
        // save the size of the container
        zip.putInt(static_cast<int>(c.size()));

        // now save the container itself
        typename container::const_iterator cit;
        for(cit=c.begin(); cit!=c.end(); ++cit)
        {
            //zip.putInt((*cit)->getTypeID()); // save the object's typeID
            (*cit)->saveToDisk(zip);         // save the object itself
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : loadContainer
        Description : loads a container of typename T from the given save buffer.  An
                      overloaded version for containers of shared_ptrs exist, since their
                      iterators require another degree of indirection.
        Inputs      : TCODzip buffer
        Outputs     : None
        Return      : container<typename T> or container< shared_ptr<T> >
    --------------------------------------------------------------------------------*/
    template<typename container, typename T>
    void loadContainer(container& c, RLNSZip& zip)
    {
        c.clear();

        // load the size of the saved container
        int numT = zip.getInt();

        // load the saved container
        for(int i=0; i<numT; ++i)
        {
            T savedT;
            savedT.loadFromDisk(zip);
            c.push_back(savedT);
        }
    }
    template<typename container, typename T>
    void loadSharedPtrContainer(container& c, RLNSZip& zip)
    {
        c.clear();

        // load the size of the saved container
        int numT = zip.getInt();

        // load the saved container
        for(int i=0; i<numT; ++i)
        {
            //int type = zip.getInt();
            //T::loadType(c, type, zip);

            boost::shared_ptr<T> savedTPtr;
            savedTPtr->loadFromDisk(zip);
            c.push_back(savedTPtr);
        }
    }
}

#endif
