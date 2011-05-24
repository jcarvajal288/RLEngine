#include "GameData.hpp"

using namespace std;
using namespace rlns::utl;

namespace rlns
{
    namespace model
    {
        GameDataPtr GameData::_instance(new GameData());

        /*--------------------------------------------------------------------------------
            Function    : GameData::get
            Description : Public access function for the private GameData instance.
            Inputs      : None
            Outputs     : None
            Return      : GameDataPtr
        --------------------------------------------------------------------------------*/
        GameDataPtr GameData::get()
        {
            if(_instance == 0)
            {
                _instance.reset(new GameData());
            }
            return _instance;
        }



        /*--------------------------------------------------------------------------------
            Function    : GameData::getPartyPositions
            Description : Returns a vector of the party members' positions
            Inputs      : None
            Outputs     : None
            Return      : vector<Point>
        --------------------------------------------------------------------------------*/
        vector<Point> GameData::getPartyPositions() const
        {
            vector<Point> positions;

            vector<ActorPtr>::const_iterator it, end = party.end();
            for(it=party.begin(); it!=end; ++it)
            {
                positions.push_back((*it)->getPosition());
            }
            return positions;
        }



        /*--------------------------------------------------------------------------------
            Function    : GameData::getPartyLights
            Description : Returns a vector of the IDs of all the party's lights.
            Inputs      : None
            Outputs     : None
            Return      : vector<int>
        --------------------------------------------------------------------------------*/
        vector<int> GameData::getPartyLights() const
        {
            vector<int> lights;
            vector<int>::iterator lit, lend;

            vector<ActorPtr>::const_iterator it, end = party.end();
            for(it=party.begin(); it!=end; ++it)
            {
                
                lend = (*it)->getLights().end();
                for(lit=(*it)->getLights().begin(); lit!=lend; ++lit)
                {
                    lights.push_back(*lit);
                }
            }
            return lights;
        }



        /*--------------------------------------------------------------------------------
            Function    : GameData::createNewGameState
            Description : Initializes a new a game state by creating the first level.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void GameData::createNewGameState()
        {
            // set Light::nextID to 0
            Light::setNextID(0);

            // create the first level
            LevelPtr firstLevel(new Level("Castle", true));
            levelTree.reset(new LevelTree(firstLevel));

            // add a single party member to the party and put him on the up stairs
            Point upstair = getCurrentLevel()->getUpStairs().at(0);
            ActorPtr newActor(new Actor(upstair, '@', TCODColor::white));
            view::Display::get()->setFocalPoint(upstair);
            party.push_back(newActor);
        }



        /*--------------------------------------------------------------------------------
            Function    : GameData::useStairs
            Description : Moves the party to the next level of the dungeon.  If the next
                          level doesn't exist, this adds a new level to the level tree.  
                          The new level will be accessed by the stairs at the given point.
            Inputs      : coordinates of stairs leading to next level, whether the new
                          level is above the current one.
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void GameData::useStairs(const Point& pt, const bool ascending)
        {
            if(!levelTree->shiftCurrentLevel(pt)) // if the new level doesn't exist yet
            {
                LevelPtr newLevel(new Level("Castle", ascending));
                levelTree->addNewLevel(pt, newLevel, ascending);
            }
            // TODO: this line must sometimes move the party to a non parentLevel point,
            // in case it is moving up in the LevelTree.
            Point destination = levelTree->getDestinationStairs(pt);
            movePartyToPoint(destination); 
        }



        /*--------------------------------------------------------------------------------
            Function    : GameData::moveActivePlayer
            Description : Moves the active player one space in the given direction.
            Inputs      : DirectionType
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void GameData::moveActivePlayer(const DirectionType& dir)
        {
            getActivePlayer()->move(dir);
            view::Display::get()->moveFocalPoint(dir);
            getCurrentLevel()->examineTile(getActivePlayer()->getPosition());
        }



        /*--------------------------------------------------------------------------------
            Function    : GameData::movePartyToPoint
            Description : Distributes the party around the given point.
            Inputs      : Point
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void GameData::movePartyToPoint(const utl::Point& destination)
        {
            // first check that the destination point is walkable
            if(!getCurrentLevel()->isWalkable(destination))
            {
                cerr << "ERROR: moving party to unwalkable location." << endl;
                return;
            }

            // TODO: make this work for a group of characters
            party.at(0)->setPosition(destination);
            view::Display::get()->setFocalPoint(destination);
        }



        /*--------------------------------------------------------------------------------
            Function    : GameData::calculatePartyFOV
            Description : Constructs a vector of all the party member's positions, and
                          sends them to the current level's calculatePartyFOV method.
            Inputs      : TCODMap
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void GameData::calculatePartyFOV(vector< vector<bool> >& map)
        {
            vector<Point> partyPositions;
            vector<ActorPtr>::iterator it, end;
            end = party.end();
            for(it=party.begin(); it!=end; ++it)
            {
                partyPositions.push_back((*it)->getPosition());
            }

            getCurrentLevel()->calculatePartyFOV(map, partyPositions);
        }



        /*--------------------------------------------------------------------------------
            Function    : GameData::getLights
            Description : Returns a vector of all the lights used by the player's Party
                          and on the current Level.
            Inputs      : None
            Outputs     : None
            Return      : vector<int>
        --------------------------------------------------------------------------------*/
        vector<int> GameData::getLights() const
        {
            vector<int> lights = getCurrentLevel()->getLights();
            vector<model::ActorPtr>::const_iterator it, end;
            end = party.end();
            for(it=party.begin(); it!=end; ++it)
            {
                // get the light vector of a party member
                vector<int> newLights = (*it)->getLights();
                
                // append the party member's light vector to the end of the lights vector
                lights.insert(lights.end(), newLights.begin(), newLights.end());
            }
            return lights;
        }



        /*--------------------------------------------------------------------------------
            Function    : GameData::saveToDisk
            Description : saves the GameData object to the given RLNSZip save buffer
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void GameData::saveToDisk(RLNSZip& zip) const
        {
            saveSharedPtrContainer(party, zip);
            zip.putInt(currentPlayer);
            levelTree->saveToDisk(zip);

            // save Light data
            Light::saveLights(zip);
        }



        /*--------------------------------------------------------------------------------
            Function    : GameData::loadFromDisk
            Description : Load the game state from the given RLNSZip buffer.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void GameData::loadFromDisk(RLNSZip& zip)
        {
            // load party
            int numPartyMembers = zip.getInt();
            for(int i=0; i<numPartyMembers; ++i)
            {
                model::ActorPtr actor(new model::Actor(zip));
                party.push_back(actor);
            }
            currentPlayer = zip.getInt();

            // set the Game Window's focal point to the active player's position
            view::Display::get()->setFocalPoint(getActivePlayer()->getPosition());

            // load level tree
            int valid = zip.getInt();
            if(valid) // test if root node is valid (it sure better be)
            {
                levelTree.reset(new LevelTree(zip));
            }

            // load Light data
            Light::loadLights(zip);
        }
    }
}
