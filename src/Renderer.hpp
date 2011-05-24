#ifndef RLNS_RENDERER_HPP
#define RLNS_RENDERER_HPP

#include <algorithm>
#include <iostream>
#include <vector>

#include "GameData.hpp"
#include "Level.hpp"
#include "Point.hpp"
#include "Tile.hpp"
#include "Types.hpp"
#include "Utility.hpp"

namespace rlns
{
    namespace view
    {
        /*--------------------------------------------------------------------------------
            Class       : Renderer
            Description : Responsible for rendering the game map, characters, items, etc.
                          to the root console.  Uses the Singleton design pattern.
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Renderer
        {
            // Member Variables
            private:
                static RendererPtr _instance;

                float elapsedTime;  // tracks the amount of time elapsed in seconds
                float burstTime;  // tracks the amount of time between refresh bursts

                // These values are updated before every refresh.
                utl::Point dispTL;      // top left of the display area
                utl::Point dispBR;      // bottom right of the display area
                utl::Point dispCenter;  // center of the display area
                utl::Point camTL;       // top left of what the camera is looking at 
                utl::Point camBR;       // bottom right of what the camera is looking at

                std::vector< std::vector<bool> > partyFOV;

            // Member Functions
            private:
                Renderer(): elapsedTime(0.0f), burstTime(0.0f) {}
                Renderer(const Renderer&);

                void setDisplayValues(const utl::Point&);
                bool isInDisplayArea(const utl::Point&) const;
                bool lightVisibleToPlayers(const utl::Point&, const utl::Point&) const;
                void calculateLightMap(const model::LevelPtr);
                void calculatePartyFOV(const model::LevelPtr);
                void drawMap(const model::LevelPtr);
                void drawLevelContents(const model::LevelPtr) const;
                void drawParty(const model::LevelPtr) const;

            public:
                void refresh(const utl::Point& focalPt = view::Display::get()->getFocalPoint());

                static RendererPtr get();

        };


        // inline functions

        // determines if the given point is in the game's display area
        inline bool Renderer::isInDisplayArea(const utl::Point& pt) const
        {
            int x = pt.getX(), y = pt.getY();
            return (x >= dispTL.getX() && x < dispBR.getX()) && (y >= dispTL.getY() && y < dispBR.getY());
        }
    }
}

#endif
