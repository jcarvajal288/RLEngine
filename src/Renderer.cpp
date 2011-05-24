#include "Renderer.hpp"

using namespace rlns::utl;
using namespace std;

namespace rlns
{
    namespace view
    {
        RendererPtr Renderer::_instance(new Renderer());

        /*--------------------------------------------------------------------------------
            Function    : Renderer::setDisplayValues
            Description : This function calculates the dimensions of the display area and
                          what in the game the camera is looking at.
            Inputs      : Focal Point of the display
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Renderer::setDisplayValues(const Point& focalPt)
        {
            // get dimensions of the display area
            dispTL = Display::get()->gameWindowTL();
            dispBR = Display::get()->gameWindowBR();
            dispCenter = Point((dispTL.getX() + dispBR.getX())/2, 
                                     (dispTL.getY() + dispBR.getY())/2); 

            // get the difference of the focal point and the center of the display area
            Point diff(dispCenter.getX(), dispCenter.getY());
            camTL = focalPt - diff;
            camBR = focalPt + diff;
        }



        /*--------------------------------------------------------------------------------
            Function    : Renderer::lightVisibleToPlayers
            Description : This function is called when it is unclear whether a wall tile
                          should be lit or unlit.  This situation arises when a single
                          tile wall is between a light source and a player character.
                          Without this function, the single tile wall may be erroneously
                          lit by the hidden light, resulting in a 'glowing' wall from the
                          point of view of the players.  This function finds the dot
                          product between the vector from the wall to the light and the
                          vector from the wall to the player.  If the dot product is
                          negative, the wall is in between the light and the player.  If
                          not, we need to check whether the wall is part of a larger wall
                          stretching between the light and the player by checking the next
                          tile in line.
            Inputs      : light position, wall position
            Outputs     : None
            Return      : bool
        --------------------------------------------------------------------------------*/
        bool Renderer::lightVisibleToPlayers(const Point& lightPos, const Point& wallPos) const
        {
            int wx = wallPos.getX(), wy = wallPos.getY();
            int lx = lightPos.getX(), ly = lightPos.getY();

            // if the party can see the light source, it's legit
            if(partyFOV.at(lx).at(ly))
                return true;

            vector<Point> playerPos = model::GameData::get()->getPartyPositions();
            vector<Point>::const_iterator it, end = playerPos.end();
            bool result = false;

            // check whether any party member is at the right angle to see the lit wall
            for(it=playerPos.begin(); it!=end; ++it)
            {
                int ix = it->getX(), iy = it->getY();
                // vector L extends from the wall tile to the light
                // vector P extends from the wall tile to the player
                int Lx = lx - wx;
                int Ly = ly - wy;
                int Px = ix - wx;
                int Py = iy - wy;

                int dotProduct = Lx*Px + Ly*Py;
                result = (dotProduct >= 0);

                if(result)
                {
                    model::LevelPtr level = model::GameData::get()->getCurrentLevel();
                    if((wy < iy && wy < ly) && (!level->isTransparent(wallPos.neighbor(NORTH))))
                    {
                        result = false;
                    }
                    else if((wx < ix && wx < lx) && (!level->isTransparent(wallPos.neighbor(EAST))))
                    {
                        result = false;
                    }
                    else if((wx > ix && wx > lx) && (!level->isTransparent(wallPos.neighbor(WEST))))
                    {
                        result = false;
                    }
                    else if((wy > iy && wy > ly) && (!level->isTransparent(wallPos.neighbor(SOUTH))))
                    {
                        result = false;
                    }
                }
            }
            return result;
        }



        /*--------------------------------------------------------------------------------
            Function    : Renderer::calculateLightMap
            Description : Iterates through each of the current Map's light sources,
                          calculates their field of view, and assigns a brightness value
                          to each field of the map's light map as appropriate.
            Inputs      : Level object
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Renderer::calculateLightMap(const model::LevelPtr level)
        {
            const int TORCH_RADIUS = 10; // magic number that will be replaced later

            // noise variables
            float di = 0.0f;

            // get all the lights in the current level, as well as all lights carried by occupants
            vector<int> lights = model::GameData::get()->getLights();

            // get a list of all the lights belonging to the player
            vector<int> playerLights = model::GameData::get()->getPartyLights();

            vector<int>::iterator it, end;
            end = lights.end();
            for(it=lights.begin(); it!=end; ++it)
            {
                // fetch the Light with this ID
                model::LightPtr light = model::Light::getLight(*it);

                if(LIGHT_FLICKER_ENABLED)
                {
                    /*
                     * XXX  The way this is coded, only the first light will flicker,
                     *      because the other lights will have their loops happen
                     *      before .1 seconds is up!  To flicker properly, Lights 
                     *      may need their own timers! Or reset elapsedTime before
                     *      the loop starts.
                     */
                    elapsedTime = TCODSystem::getElapsedSeconds();
                    if(elapsedTime - burstTime > (light)->getFlickerRate()) 
                    {
                        burstTime = elapsedTime;
                        di = light->getFlickerDelta() * light->getNoise(elapsedTime);
                    }
                }

                int radius = static_cast<int>(TORCH_RADIUS * (light->getIntensity()));
                int radius_squared = radius*radius;
                Point position = (light)->getPosition();
                int px = position.getX(), py = position.getY();
                level->calculateFOV(position, radius);

                // find boundary of light's effect on the map,
                // keeping in mind the boundaries of the map itself
                Point TL(max(position.getX()-radius, 0), 
                         max(position.getY()-radius, 0));
                Point BR(min(position.getX()+radius, level->getWidth()), 
                         min(position.getY()+radius, level->getHeight()));

                float intensityCoef;
                for(int x=TL.getX(); x<BR.getX(); ++x)
                {
                    for(int y=TL.getY(); y<BR.getY(); ++y)
                    {
                        intensityCoef = 0.0f;
                        float dist = static_cast<float>((x-px)*(x-px)+(y-py)*(y-py));
                        if(dist < radius_squared)
                        {
                            intensityCoef = (radius_squared - 0.05f * dist) / 
                                            (0.05f * dist * radius_squared + radius_squared);
                        }
                        if(level->isInFOV(x,y))
                        {
                            // if the tile is not transparent, check if the players should see this tile lit
                            if(level->isTransparent(x,y) || lightVisibleToPlayers(position, Point(x,y)))
                            {
                                level->updateTileLightInfo(x,y, light->getColor() * 
                                                                light->getIntensity() * 
                                                                intensityCoef);
                            }
                        }
                    }
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Renderer::calculatePartyFOV
            Description : Creates an FOV map that shows which tiles are viewed by any 
                          member of the party.
            Inputs      : 2D vector of bools
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Renderer::calculatePartyFOV(const model::LevelPtr level)
        {
            partyFOV = vector< vector<bool> >(level->getWidth(), vector<bool>(level->getHeight(), false));
            model::GameData::get()->calculatePartyFOV(partyFOV);
        }



        /*--------------------------------------------------------------------------------
            Function    : Renderer::drawMap
            Description : Draws the level Map.  Before drawing anything, this function
                          firsts calculates how much of the map is actually displayed on
                          the screen and does not attempt to draw outside the map bounds.
                          (Sounds obvious, but the first implementation simply caught
                          std::out_of_range exceptions and ignored them.  Turned out to be
                          inefficient to catch a few thousand exceptions every refresh.  
                          Who knew.)
            Inputs      : Level object
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Renderer::drawMap(const model::LevelPtr level)
        {
            // determine if part of the camera view is off the map
            bool camTLX_gt_0 = camTL.getX() >= 0;
            bool camTLY_gt_0 = camTL.getY() >= 0;
            bool camBRX_lt_w = camBR.getX() < level->getWidth();
            bool camBRY_lt_h = camBR.getY() < level->getHeight();

            // adjust display and camera coordinates accordingly
            int dispTL_X = camTLX_gt_0 ? dispTL.getX() : dispTL.getX() - camTL.getX();
            int dispTL_Y = camTLY_gt_0 ? dispTL.getY() : dispTL.getY() - camTL.getY();
            int dispBR_X = camBRX_lt_w ? dispBR.getX() : dispBR.getX() + (level->getWidth() - camBR.getX()-1); // '-1' necessary to avoid drawing tiles off the edge of the map
            int dispBR_Y = camBRY_lt_h ? dispBR.getY() : dispBR.getY() + (level->getHeight() - camBR.getY());  // '-1' not necessary here, for some reason.
            int camTL_X  = (camTL.getX() >= 0) ? camTL.getX() : 0;
            int camTL_Y  = (camTL.getY() >= 0) ? camTL.getY() : 0;

            int a,b,x,y; // loop variables. (a,b) - root console location; (x,y) map location
            for(a=dispTL_X, x=camTL_X; a<dispBR_X; ++a, ++x)
            {
                for(b=dispTL_Y, y=camTL_Y; b<dispBR_Y; ++b, ++y)
                {
                    if(partyFOV.at(x).at(y) // check if a party member can see the tile
                    || DEBUG_MODE)          // OR if the player has cheats on
                    {
                        try 
                        {
                            level->drawTile(a,b,x,y);
                        }
                        catch(std::out_of_range&)
                        {
                            // still catch any exceptions, just in case.
                        }
                    }
                }
            }
        }



        /*--------------------------------------------------------------------------------
            Function    : Renderer::drawLevelContents
            Description : Draws all of the creatures, features, items, etc. in the level.
                          Things are drawn in increasing order of importance.  First
                          features, then items, and then finally creatures.
            Inputs      : None
            Outputs     : None
            Return      : void
        --------------------------------------------------------------------------------*/
        void Renderer::drawLevelContents(const model::LevelPtr level) const
        {
            (void) level;
            /* draw features
            vector<model::FeaturePtr> features = level->getFeatures();
            vector<model::FeaturePtr>::const_iterator fit, fend;
            fend = features.end();
            for(fit=features.begin(); fit!=fend; ++fit)
            {
                Point pt = (*fit)->getPosition() - camTL;
                if(isInDisplayArea(pt))
                {
                    TCODConsole::root->setChar(pt.getX(), pt.getY(), (*fit)->getChar());
                }
            }

            // TODO: draw creatures
            
            // TODO: draw items
            */

        }



        /*--------------------------------------------------------------------------------
            Function    : Renderer::drawParty
            Description : Draws the Actors in the party vector.  The party vector will
                          eventually be replaced by a Party class, and this function will
                          be deprecated.
            Inputs      : focal point of the display, level object
            Outputs     : Party to the game window
            Return      : void
        --------------------------------------------------------------------------------*/
        void Renderer::drawParty(const model::LevelPtr level) const
        {
            model::ActorPtr player = model::GameData::get()->getParty().at(0);
            Point position = player->getPosition();
            TCODColor bg = player->getBgColor();

            // get the list of tiles at the given position
            vector<model::TilePtr> tiles = level->getTiles(position);

            // create iterators
            vector<model::TilePtr>::const_reverse_iterator it, rbegin, rend;
            rbegin = tiles.rbegin();
            rend = tiles.rend();

            // iterate through the tiles list, finding the topmost foreground and background colors
            for(it=tiles.rbegin(); it!=rend; ++it)
            {
                // the fuchsia color is treated as being blank for our purposes,
                // so once we find a non-fuchsia color, we have a color we want to display
                if(bg == TCODColor::fuchsia) bg = (*it)->getBgColor();
            }

            // now display the correctly colored player character
            // XXX: These statements draw the player at the center of the display,
            // REGARDLESS OF WHETHER THAT'S HIS ACTUAL POSITION! MUST BE CHANGED LATER
            TCODColor light = level->getTileLightInfo(position);
            TCODConsole::root->putCharEx(dispCenter.getX(), dispCenter.getY(), 
                                         player->getChar(),
                                         player->getFgColor() * light,
                                         bg * light);
        }



        /*--------------------------------------------------------------------------------
            Function    : Renderer::refresh()
            Description : Calls the necessary functions to refresh the screen.
            Inputs      : focal point of the game window (defaults to the current focal
                          point)
            Outputs     : updated screen on the root console
            Return      : void
        --------------------------------------------------------------------------------*/
        void Renderer::refresh(const Point& focalPt)
        {
            // clear the old screen
            TCODConsole::root->clear();

            setDisplayValues(focalPt);

            // draw the current level
            const model::LevelPtr level = model::GameData::get()->getCurrentLevel();

            level->clearLightMap(); // erase old lighting
            calculatePartyFOV(level); // calculate what the players can see
            calculateLightMap(level); // calculate new lighting
            drawMap(level);
            //drawLevelContents(level);
            drawParty(level);

            // draw the console
            Display::get()->drawConsole();

            // draw the command line
            Display::get()->drawCommandPrompt();

            // flush the changes to the root console
            TCODConsole::flush();
        }



        /*--------------------------------------------------------------------------------
            Function    : Renderer::get
            Description : Public access point to the singleton Renderer instance.
            Inputs      : None
            Outputs     : None
            Return      : RendererPtr
        --------------------------------------------------------------------------------*/
        RendererPtr Renderer::get()
        {
            if(_instance == 0)
            {
                _instance.reset(new Renderer());
            }
            return _instance;
        }
    }
}
