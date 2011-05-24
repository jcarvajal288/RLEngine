#ifndef RLNS_LEVELTREE_HPP
#define RLNS_LEVELTREE_HPP

#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <cassert>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

#include "CheckedSave.hpp"
#include "Level.hpp"
#include "Point.hpp"
#include "Types.hpp"
#include "Utility.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace model
    {
        // a PointNodePair is a link in the LevelTree.  The point part is the coordinates
        // of the stairs that lead from the LevelNode part of the pair to the LevelNode that
        // holds the link.
        typedef std::pair<utl::Point, LevelNodePtr> PointNodePair;

        /*--------------------------------------------------------------------------------
            Class       : LevelNode
            Description : A Node in the LevelTree.  Each node has a level and a collection
                          of references to its parent and child levels.  Links represent
                          stairways between the levels, contained in a std::map.  The
                          map's keys represent the positions of stairs leading out of the
                          level.  The values are another pair (PointNodePair), where the
                          point part is the position of the stair that leads from the
                          destination level back to the this level, and the Node represents
                          the level node itself.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class LevelNode: public boost::enable_shared_from_this<LevelNode>
        {
            // Member Variables
            private:
                bool current; // is true if this is the current level when the game is saved
                LevelPtr level;
                utl::Point stairsToParent;
                std::map<utl::Point, PointNodePair> links;

            // Member Functions
            private:
                bool linksEqual(const std::map<utl::Point, PointNodePair>& ls) const { return links == ls; }

            public:
                LevelNode() {}
                LevelNode(const LevelPtr, const bool); // constructs the root node
                LevelNode(const LevelPtr, const LevelNodePtr, const utl::Point&, const bool); // constructs child nodes
                LevelNode(utl::RLNSZip&); // used for loading from saved games

                void attachToChildren(const LevelNodePtr);

                utl::Point getConnectingStairs(const utl::Point& pt) const { return links.find(pt)->second.first; }
                LevelNodePtr getNeighbor(const utl::Point& pt) const { return links.find(pt)->second.second; }
                bool linkExists(const utl::Point& pt) const { return links.find(pt) != links.end(); }
                void addLink(const utl::Point&, const utl::Point&, const LevelNodePtr);

                void setCurrent(const bool b) { current = b; }
                bool isCurrent() const { return current; }

                void findCurrentNode(LevelNodePtr&);

                LevelPtr getLevel() const { return level; }
                utl::Point getStairsToParent() const { return stairsToParent; }


                bool operator==(const LevelNode&) const;
                bool operator!=(const LevelNode&) const;

                void saveToDisk(utl::RLNSZip&) const;
                void saveStairPositions(utl::RLNSZip&) const;
        };

        // inline functions

        inline void LevelNode::addLink(const utl::Point& origin, const utl::Point& destination, const LevelNodePtr node) 
        { 
            links[origin] = PointNodePair(destination, node); 
        }

        // Defines the == operator for LevelNodes by comparing the points
        // of their links.  While theoretically there could be two
        // LevelNodes with the exact same link coordinates, it should be
        // suitably unlikely.
        inline bool LevelNode::operator==(const LevelNode& rhs) const
        {
            return rhs.linksEqual(links);
        }
        inline bool LevelNode::operator!=(const LevelNode& rhs) const
        {
            return !(*this == rhs);
        }



        /*--------------------------------------------------------------------------------
            Class       : LevelTree
            Description : Defines the sequence of the levels in the game.  The LevelTree's
                          root is the town level, and each tileset has its own branch in
                          the tree.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class LevelTree
        {
            // Member Variables
            private:
                LevelNodePtr root;
                LevelNodePtr currentNode;
                LevelNodePtr previousNode; // used when attaching new levels to the tree

            // Member Functions
            public:
                LevelTree() {}
                LevelTree(const LevelPtr level)
                : root(new LevelNode(level, true)), currentNode(root), previousNode(currentNode) {}

                LevelTree(utl::RLNSZip&);

                LevelPtr getCurrentLevel() const { return currentNode->getLevel(); }
                bool shiftCurrentLevel(const utl::Point&);

                utl::Point getDestinationStairs(const utl::Point&) const;

                void addNewLevel(const utl::Point&, const LevelPtr, const bool);

                void saveToDisk(utl::RLNSZip& zip) const;
        };


        // inline functions

        inline utl::Point LevelTree::getDestinationStairs(const utl::Point& pt) const
        { 
            // previousNode is used here, because when the connectin stairs
            // are requested during the level creation process, currentNode
            // is already pointing at the new level.
            return previousNode->getConnectingStairs(pt);
        }
    }
}

#endif
