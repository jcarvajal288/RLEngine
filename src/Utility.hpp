#ifndef RLNS_UTILITY_HPP
#define RLNS_UTILITY_HPP

#include <cstdlib>
#include <iostream>
#include <math.h>
#include <string>

#include "CheckedSave.hpp"
#include "Point.hpp"

namespace rlns
{
    namespace utl
    {
       /*-------------------------------------------------------------------------------- 
            Function    : fatalError
            Description : Prints an error message to stderr, then exits the program
            Inputs      : error message
            Outputs     : error message
            Return      : void
        --------------------------------------------------------------------------------*/
        void fatalError(const std::string&);

        /*--------------------------------------------------------------------------------
            Function    : relDistance (relative distance)
            Description : performs the distance formula, minus the square root, on two
                          Points.  Useful for comparing distances while avoiding the
                          costly square root function.
            Inputs      : two Point objects
            Outputs     : None
            Return      : int
        --------------------------------------------------------------------------------*/
        int relDistance(const utl::Point&, const utl::Point&);

        /*--------------------------------------------------------------------------------
            Function    : printColor
            Description : Convenience function for printing a TCODColor.
            Inputs      : TCODColor
            Outputs     : TCODColor printed in (r,g,b) format
            Return      : void
        --------------------------------------------------------------------------------*/
        void printColor(const TCODColor&);



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
}

#endif
