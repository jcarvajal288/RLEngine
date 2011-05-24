#include "Actor.hpp"

using namespace std;
using namespace rlns::utl;

namespace rlns
{
    namespace model
    {
        /*--------------------------------------------------------------------------------
            Function    : Actor::moveLights
            Description : Moves all owned lights to the Actor's position.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Actor::moveLights()
        {
            Point destination = position;

            vector<int>::iterator it, end;
            vector<int> ownedLights = getLights();
            end = ownedLights.end();
            for(it=ownedLights.begin(); it!=end; ++it)
            {
                Light::moveLight(*it, destination);
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Actor::Actor(RLNSZip)
            Description : Constructs the actor from the given save buffer.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : None (constructor)
        --------------------------------------------------------------------------------*/
        Actor::Actor(RLNSZip& zip)
        : AbstractTile(zip), position(zip)
        {
            int numLights = zip.getInt();
            for(int i=0; i<numLights; ++i)
            {
                ownedLights.push_back(zip.getInt());
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Actor::attachLight
            Description : adds the given Light ID to the AbstractTile's ownedLights vector,
                          and sets the light's position to the position of the AbstractTile.
            Inputs      : ID of the light
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Actor::attachLight(const int lightID)
        {
            Light::moveLight(lightID, position);
            ownedLights.push_back(lightID);
        }



        /*--------------------------------------------------------------------------------
            Function    : Actor::move
            Description : Moves the actor one tile in the given direction.  Other objects
                          owned by this Actor, such as lights from torches it is carrying,
                          are also moved.
            Inputs      : DirectionType
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Actor::move(const DirectionType dir)
        {
            position.shift(dir); 
            moveLights(); 
        }



        /*--------------------------------------------------------------------------------
            Function    : Actor::saveToDisk
            Description : Saves the Actor to the given save buffer.
            Inputs      : RLNSZip buffer
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Actor::saveToDisk(RLNSZip& zip) const
        {
            AbstractTile::saveToDisk(zip);

            // save position
            position.saveToDisk(zip);

            // save ownedLights
            std::vector<int>::const_iterator it, end;
            end = ownedLights.end();
            zip.putInt(ownedLights.size());
            for(it=ownedLights.begin(); it!=end; ++it)
            {
                zip.putInt(*it);
            }
        }
    }
}
