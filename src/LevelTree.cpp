#include "LevelTree.hpp"

using namespace std;
using namespace rlns::utl;

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Function    : LevelNode::LevelNode(const LevelPtr)
            Description : Constructs the root Node of the LevelTree
            Inputs      : LevelPtr, whether this level is approached from above
            Outputs     : None
            Return      : None (constructor)
        --------------------------------------------------------------------------------*/
        LevelNode::LevelNode(const LevelPtr lvl, const bool ascending)
        : level(lvl)
        {
            // construct links map
            vector<Point> stairs(level->getUpStairs());
            vector<Point> downStairs(level->getDownStairs());

            // assign the stairway leading back to the parent node and
            // remove that stairway from the list of stairs
            if(ascending)
            {
                stairsToParent = *downStairs.begin();
            }
            else
            {
                stairsToParent = *stairs.begin();
            }
            // leave assignment of the parent node until after this function,
            // as the parent node is kept track of in LevelTree

            stairs.insert(stairs.end(), downStairs.begin(), downStairs.end());

            vector<Point>::const_iterator it, end;
            it = stairs.begin(), end = stairs.end();
            for(; it!=end; ++it)
            {
                LevelNodePtr node = boost::make_shared<LevelNode>();
                addLink(*it, Point(0,0), node);
                //links.insert(pair<Point, LevelNodePtr>(*it, node));
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : LevelNode::LevelNode(const LevelPtr, const LevelNodePtr, const bool)
            Description : Constructs a new LevelNode given a LevelPtr.
            Inputs      : LevelPtr, parent node, whether this level was approached from up
                          or down.
            Outputs     : None
            Return      : None (constructor)
        --------------------------------------------------------------------------------*/
        LevelNode::LevelNode(const LevelPtr lvl, const LevelNodePtr parent, const Point& oldStairs, const bool up=false)
        : level(lvl)
        {
            // create a vector containing the positions of all the stairs in the level
            // the stairs leading to the parent level will be the first in this list, so
            // if this level was reached by going up stairs, the down stairs of this level
            // need to be first, and vice versa
            vector<Point> stairs;
            vector<Point> upStairs = level->getUpStairs(), downStairs = level->getDownStairs();
            if(up)
            {
                stairs = downStairs;
                stairs.insert(stairs.end(), upStairs.begin(), upStairs.end());
            }
            else
            {
                stairs = upStairs;
                stairs.insert(stairs.end(), downStairs.begin(), downStairs.end());
            }

            // now construct the links map
            // the first point in the stairs vector is the stairs to the parent level.
            // the other stairs are to levels that don't yet exist, so make them point
            // to a NULL value.
            vector<Point>::const_iterator it, end;
            it = stairs.begin(), end = stairs.end();
            stairsToParent = *it;
            links[*(it++)] = PointNodePair(oldStairs, parent);
            for(; it!=end; ++it)
            {
                LevelNodePtr node = boost::make_shared<LevelNode>();
                addLink(*it, Point(0,0), node);
                //links.insert(pair<Point, LevelNodePtr>(*it, node));
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : LevelNode::LevelNode(RLNSZip&)
            Description : Constructs the LevelNode and its links from the given RLNSZip
                          save buffer.
            Inputs      : RLNSZip buffer, parent node
            Outputs     : None
            Return      : None (constructor)
        --------------------------------------------------------------------------------*/
        LevelNode::LevelNode(RLNSZip& zip)
        : current(static_cast<bool>(zip.getInt())), level(new Level(zip)), stairsToParent(zip)
        {
            int numLinks = zip.getInt();

            // for now, the parent link points to a blank LevelNodePtr.  Once all the nodes are
            // created, we'll go back through and attach them to their parents
            addLink(stairsToParent, Point(0,0), boost::make_shared<LevelNode>());
            //links.insert(pair<Point, LevelNodePtr>(stairsToParent, boost::make_shared<LevelNode>()));

            // this loop will retrace the preorder traversal the nodes were saved in
            // i is set to 1 to skip the link leading to the node's parent loaded above
            for(int i=1; i<numLinks; ++i)
            {
                if(zip.getInt()) // is the node valid?  If so, load it.
                {
                    LevelNodePtr node(new LevelNode(zip));
                    Point from(zip);
                    Point to(0,0);
                    //links.insert(pair<Point, LevelNodePtr>(pt, node));
                    addLink(from, to, node);
                }
                else
                {
                    Point from(zip);
                    Point to(0,0);
                    addLink(from, to, boost::make_shared<LevelNode>());
                    //links.insert(pair<Point, PointNodePair>(pt, boost::make_shared<LevelNode>()));
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : LevelNode::attachToChildren
            Description : This function is called after all of the LevelNodes in the
                          LevelTree have been loaded from a save game.  It recurs through
                          each node, linking all of the node's children to their parent.
            Inputs      : Parent node
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void LevelNode::attachToChildren(const LevelNodePtr parentNode)
        {
            //links[stairsToParent] = parentNode;
            links[stairsToParent].second = parentNode;

            map<Point, PointNodePair>::iterator it, end;
            end = links.end();
            for(it=links.begin(); it!=end; ++it)
            {
                // if this node is not the parent node, recur
                if(it->first != stairsToParent)
                {
                    it->second.second->attachToChildren(shared_from_this());
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : LevelNode::findCurrentNode
            Description : Traverses the tree using depth-first search looking for a node 
                          marked as current. It then sets currentNode to that node.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void LevelNode::findCurrentNode(LevelNodePtr& currentNode)
        {
            if(isCurrent())
                currentNode = shared_from_this();

            map<Point, PointNodePair>::const_iterator it, end;
            end = links.end();
            for(it=links.begin(); it!=end; ++it)
            {
                it->second.second->findCurrentNode(currentNode);
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : LevelNode::saveToDisk
            Description : Saves the LevelNode to the given RLNSZip save buffer.
            Inputs      : RLNSZip buffer.
            Outputs     : None
            Return      : bool (indicating if a valid node was saved)
        --------------------------------------------------------------------------------*/
        void LevelNode::saveToDisk(RLNSZip& zip) const
        {
            // test if this node is null
            if(level.get() == NULL) 
            {
                zip.putInt(0); // marks this node as invalid
                return;
            }
            else 
            {
                zip.putInt(1); // marks this node as valid
            }

            zip.putInt(static_cast<int>(current));

            level->saveToDisk(zip);
            stairsToParent.saveToDisk(zip);

            // save the size of root's links map
            zip.putInt(links.size());

            // save the rest of this subtree
            map<Point, PointNodePair>::const_iterator it, end;
            end = links.end();
            for(it=links.begin(); it!=end; ++it)
            {
                // if this node is not the parent node, save it
                if(it->first != stairsToParent)
                {
                    it->second.second->saveToDisk(zip);
                    it->second.first.saveToDisk(zip);
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : LevelTree::LevelTree(RLNSZip&)
            Description : Loads the Level Tree from a saved game buffer.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : None (constructor)
        --------------------------------------------------------------------------------*/
        LevelTree::LevelTree(RLNSZip& zip)
        : root(new LevelNode(zip)), 
          currentNode(boost::make_shared<LevelNode>())
        {
            root->findCurrentNode(currentNode);
            currentNode->setCurrent(false);
            LevelNodePtr nullNode = boost::make_shared<LevelNode>();
            root->attachToChildren(nullNode);
        }



        /*--------------------------------------------------------------------------------
            Function    : LevelTree::shiftCurrentLevel
            Description : Shifts the current level pointer from the current level to the
                          level accessed by the given point in the current level's links
                          map.  This function is used when the party uses a stairway to
                          move between levels.
            Inputs      : Point
            Outputs     : None
            Return      : bool (indicating if the destination level exists)
        --------------------------------------------------------------------------------*/
        bool LevelTree::shiftCurrentLevel(const Point& pt)
        {
            assert(currentNode->linkExists(pt)); // check the point is actually a stair
            previousNode = currentNode;
            currentNode = currentNode->getNeighbor(pt);

            return currentNode->getLevel() != NULL;
        }



        /*--------------------------------------------------------------------------------
            Function    : LevelTree::addNewLevel
            Description : Adds a new level to the LevelTree.  The new level is accessed
                          by using the stairs at the given point on the current level.
                          At this point currentNode is the node the party is traveling to, 
                          previousNode is the node the party is traveling from.
            Inputs      : stairs to the new level from the old level, LevelPtr, whether
                          the new level is being approached from below.  
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void LevelTree::addNewLevel(const Point& oldStairs, const LevelPtr newLevel, const bool ascending)
        {
            currentNode.reset(new LevelNode(newLevel, ascending));

            Point newStairs = currentNode->getStairsToParent();

            // attach the previous node to the current node
            previousNode->addLink(oldStairs, newStairs, currentNode);

            // attach the current node to the previous node
            currentNode->addLink(newStairs, oldStairs, previousNode);
        }



        /*--------------------------------------------------------------------------------
            Function    : LevelTree::saveToDisk
            Description : Saves the level tree to the given RLNSZip save buffer.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void LevelTree::saveToDisk(RLNSZip& zip) const
        {
            currentNode->setCurrent(true);
            root->saveToDisk(zip);
        }
    }
}
