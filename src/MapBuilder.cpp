#include "MapBuilder.hpp"

using namespace std;

namespace rlns
{
    /*--------------------------------------------------------------------------------
        Function    : MapBuilder::placeStairs
        Description : Places the up and down staircases on the map.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void MapBuilder::placeStairs()
    {
        AreaPtr start, end;
        start = *(areas.begin());
        end = *(areas.end()-1);

        // Place the up stair
        Point up;
        do { up = start->getRandomPoint(rand); } 
        while(!map->isWalkable(up));
        map->addFeature(up, map->tileset->getUpStairTileID());
        map->setUpStairLocation(up);

        // Place the up stair
        Point down;
        do { down = end->getRandomPoint(rand); } 
        while(!map->isWalkable(down));
        map->addFeature(down, map->tileset->getDownStairTileID());
        map->setDownStairLocation(down);
    }



    /*--------------------------------------------------------------------------------
        Function    : MapBuilder::buildMap
        Description : Calls all of the subordinate map building routines.
        Inputs      : None
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void MapBuilder::buildMap() 
    {
        buildBSPTree();
        constructAreas();
        connectAreas();
        addLights();
        finishMap();
    }



    /*--------------------------------------------------------------------------------
        Function    : getBorderCode
        Description : assembles an integer code that tells which of the tiles
                      surrounding another are of a given character code. The code is
                      similar to how Unix file permissions work; each unique code is,
                      when converted to binary, a series of boolean flags.
        Inputs      : a map, a Point, and a character constant.
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    int getBorderCode(const MapPtr map, const Point& center, const int ch)
    {
        // get center Point coordinates
        int x = center.X();
        int y = center.Y();
        int code = 0;

        // assemble code.  The try/catch's are in case the Point is at the edge of the
        // map, in which case, the code is not incremented, since a non-existant tile
        // is certainly not equal to ch.
        //
        // try each compass direction and trip a flag if the character is found
        // from North -> NorthEast -> ... -> NorthWest
        try { if(map->bottomMostAt(x,y-1) == ch) code += 1; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x+1,y-1) == ch) code += 2; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x+1,y) == ch) code += 4; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x+1,y+1) == ch) code += 8; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x,y+1) == ch) code += 16; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x-1,y+1) == ch) code += 32; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x-1,y) == ch) code += 64; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x-1,y-1) == ch) code += 128; }
        catch(out_of_range&) {}

        return code;
    }



    /*--------------------------------------------------------------------------------
        Function    : getBorderCode
        Description : A variation of the above function that takes a vector of tile
                      ids instead of a single one, and treats all of the tile ids in
                      the vector as one for determining the border code.
        Inputs      : a map, a Point, and a vector of character constants.
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    int getBorderCode(const MapPtr map, const Point& center, const vector<int>& ids)
    {
        // get center Point coordinates
        int x = center.X();
        int y = center.Y();
        int code = 0;

        vector<int>::const_iterator begin, end;
        begin = ids.begin(); end = ids.end();

        // try each compass direction and trip a flag if the character is found
        // from North -> NorthEast -> ... -> NorthWest
        try { if(find(begin, end, map->bottomMostAt(x,y-1)) != end) code += 1; }
        catch(out_of_range&) {}

        try { if(find(begin, end, map->bottomMostAt(x+1,y-1)) != end) code += 2; }
        catch(out_of_range&) {}

        try { if(find(begin, end, map->bottomMostAt(x+1,y)) != end) code += 4; }
        catch(out_of_range&) {}

        try { if(find(begin, end, map->bottomMostAt(x+1,y+1)) != end) code += 8; }
        catch(out_of_range&) {}

        try { if(find(begin, end, map->bottomMostAt(x,y+1)) != end) code += 16; }
        catch(out_of_range&) {}

        try { if(find(begin, end, map->bottomMostAt(x-1,y+1)) != end) code += 32; }
        catch(out_of_range&) {}

        try { if(find(begin, end, map->bottomMostAt(x-1,y)) != end) code += 64; }
        catch(out_of_range&) {}

        try { if(find(begin, end, map->bottomMostAt(x-1,y-1)) != end) code += 128; }
        catch(out_of_range&) {}

        return code;
    }



    /*--------------------------------------------------------------------------------
        Function    : numNeighbors
        Description : Counts the number of neighbor tiles that match the given tile
                      ID.
        Inputs      : map, a Point, and the tile ID to look for.
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    int numNeighbors(const MapPtr map, const Point& center, const int ch)
    {
        // get center Point coordinates
        int x = center.X();
        int y = center.Y();
        int count = 0;

        // try each compass direction and increment the counter if the character is found
        // from North -> NorthEast -> ... -> NorthWest
        try { if(map->bottomMostAt(x,y-1) == ch) ++count; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x+1,y-1) == ch) ++count; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x+1,y) == ch) ++count; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x+1,y+1) == ch) ++count; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x,y+1) == ch) ++count; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x-1,y+1) == ch) ++count; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x-1,y) == ch) ++count; }
        catch(out_of_range&) {}

        try { if(map->bottomMostAt(x-1,y-1) == ch) ++count; }
        catch(out_of_range&) {}

        return count;
    }



    /*--------------------------------------------------------------------------------
        Function    : isReachable
        Description : Determines if two points on the map can reach each other by 
                      walking.
        Inputs      : Map object and the two Points that need to be tested.
        Outputs     : None
        Return      : bool
    --------------------------------------------------------------------------------*/
    bool isReachable(const MapPtr map, const Point& origin, const Point& dest)
    {
        TCODPath path(&(map->pathingMap), 100.0f); // we don't want diagonal movements
        path.compute(origin.X(), origin.Y(), dest.X(), dest.Y());
        return !path.isEmpty();
    }



    /*--------------------------------------------------------------------------------
        Function    : connectWayPoints
        Description : Generates additional points to fill in the gaps between the
                      points in the waypoint vector.
        Inputs      : result vector, waypoint vector 
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    static void connectWayPoints(vector<Point>* path, const vector<Point>* wayPoints)
    {
        unsigned int i, j, k;
        const unsigned int MAX_SEQ = 1024;
        vector<Point> segment;
        unsigned int wayPointsSize = wayPoints->size();

        path->push_back(Point(wayPoints->at(0).X(), wayPoints->at(0).Y()));
        k = 1;

        for(i=0; i<wayPointsSize-1; ++i)
        {
            findStraightLineBetweenPoints(&segment, wayPoints->at(i), wayPoints->at(i+1));

            for(j=1; (j < segment.size()) && (k < MAX_SEQ); ++j)
            {
                path->push_back(Point(segment.at(j).X(), segment.at(j).Y()));
                ++k;
            }
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : nSteps
        Description : Returns the horizontal distance between two points if the 
                      horizontal distance is greater than vertical distance.
                      Otherwise, returns the vertical distance.
        Inputs      : coordinates of the two points
        Outputs     : None
        Return      : int
    --------------------------------------------------------------------------------*/
    static int nSteps(const int x1, const int y1, const int x2, const int y2)
    {
        int dx, dy;

        dx = abs(x1 - x2);
        dy = abs(y1 - y2);
        
        return ((dx > dy) ? dx : dy);
    }



    /*--------------------------------------------------------------------------------
        Function    : cutCorners
        Description : Helper function for the windPath function, this makes sure
                      there are no right angles in the path.
        Inputs      : vector of Points
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    static void cutCorners(vector<Point>* path)
    {
        unsigned int i, j;

        j = 1;
        for(i=1; i<path->size()-1; ++i)
        {
            path->at(j).setX(path->at(i).X());
            path->at(j).setY(path->at(i).Y());

            if(nSteps(path->at(j-1).X(), path->at(j-1).Y(),
                      path->at(i+1).X(), path->at(i+1).Y()) > 1)
            {
                ++j;
            }
        }

        path->at(j).setX(path->at(i).X());
        path->at(j).setY(path->at(i).Y());
        ++j;
    }



    /*--------------------------------------------------------------------------------
        Function    : perturbPath
        Description : Helper function for windPath.  This selects random points in
                      the provided trajectory and displaces them provided no sharp
                      angles are created and the new location isn't too close or too
                      far from the neighbors.
                      Apologies for the poor commenting in this function, but I copied
                      it directly from the source, which was not well commented either.
        Inputs      : waypoint vector, minimum distance, maximum distance, amount to
                      perturb
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    static void perturbPath(const MapPtr map, TCODRandom* rand, vector<Point>* wayPoints, const int mindist, const int maxdist, const int pertamt)
    {
        if(wayPoints->size() < 3)
        {
            // nothing to do
            return;
        }

        int Xoff[8] = {1,  1,  0, -1, -1, -1,  0,  1};
        int Yoff[8] = {0,  1,  1,  1,  0, -1, -1, -1};

        int nx, ny;
        int lox, loy, hix, hiy;
        int lod2, hid2;
        int ri, rdir; // random index, random direction
        int mind2 = mindist * mindist;
        int maxd2 = maxdist * maxdist;
        const int mincos2 = 500; // cos^2 in 1/1000, for angles < 45 degrees
        int wayPointsSize = wayPoints->size();

        for(int i=0; i<pertamt * wayPointsSize; ++i)
        {
            ri = 1 + rand->getInt(0, wayPointsSize - 3);
            rdir = rand->getInt(0,8);
            nx = wayPoints->at(ri).X() + Xoff[rdir];
            ny = wayPoints->at(ri).Y() + Yoff[rdir];
            lox = wayPoints->at(ri-1).X();
            loy = wayPoints->at(ri-1).Y();
            hix = wayPoints->at(ri+1).X();
            hiy = wayPoints->at(ri+1).Y();
            lod2 = (nx-lox)*(nx-lox) + (ny-loy)*(ny-loy);
            hid2 = (nx-hix)*(nx-hix) + (ny-hiy)*(ny-hiy);

            if((!map->isInBoard(nx, ny))
            || (lod2 < mind2) || (lod2 > maxd2)
            || (hid2 < mind2) || (hid2 > maxd2))
            {
                continue;
            }

            // Check the angle at ri (vertex at nx, ny)
            if(signcos2(lox, loy, nx, ny, hix, hiy) < mincos2)
            {
                continue;
            }

            // Check the angle at ri +1 (vertex at hix, hiy)
            if((ri < wayPointsSize - 2) && (signcos2(nx, ny, hix, hiy,
                wayPoints->at(ri+2).X(), wayPoints->at(ri+2).Y()) < mincos2))
            {
                continue;
            }

            wayPoints->at(ri).setX(nx);
            wayPoints->at(ri).setY(ny);
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : windPath
        Description : This is a helper function for getCurvyPathBetweenPoints.  It
                      does most of the legwork involved in winding the path.
        Inputs      : path to wind, random number generator, begin point, end point, 
                      perturbation amount
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    static void windPath(const MapPtr map, TCODRandom* rand, vector<Point>* path, const Point& origin, const Point& dest, const int pertamt)
    {
        vector<Point> wayPoints;

        findStraightLineBetweenPoints(&wayPoints, origin, dest);

        if(wayPoints.size() < 5)
        {
            // too short to wind, just use a straight line
            *path = wayPoints;
            return;
        }

        // Copy one cell in two/three, making sure the ends are copied

        unsigned int i = 0, j = 0;
        unsigned int wayPointsSize = wayPoints.size();

        while(i < wayPointsSize)
        {
            wayPoints.at(j).setX(wayPoints.at(i).X());
            wayPoints.at(j).setY(wayPoints.at(i).Y());
            ++j;

            if((i < wayPointsSize - 5) || (i >= wayPointsSize - 1))
            {
                i += 2 + rand->getInt(0,2);
            }
            else if(i == wayPointsSize - 5)
            {
                i += 2;
            }
            else
            {
                i = wayPointsSize - 1;
            }
        }

        wayPointsSize = j; // XXX: watch this line.  I don't know what it does

        perturbPath(map, rand, &wayPoints, 2, 5, pertamt);
        connectWayPoints(path, &wayPoints);
        cutCorners(path);
    }



    /*--------------------------------------------------------------------------------
        Function    : makePath
        Description : Makes a path along the given list of points using the provided
                      tile ID.
        Inputs      : Map object, vector of Points, tile ID
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void makePath(const MapPtr map, const vector<Point>& path, const int tileID)
    {
        vector<Point>::const_iterator it, end;
        it = path.begin(); end = path.end();
        for(; it!=end; ++it)
        {
            int x = it->X(), y = it->Y();

            // make a floor tile on the point in the line, and the three tiles
            // to the north, west, and northwest to create a two tile wide corridor
            try { map->setBottomMostAt(x-1,y-1, tileID); }
            catch(std::out_of_range&) {}
            try { map->setBottomMostAt(x,y-1, tileID); }
            catch(std::out_of_range&) {}
            try { map->setBottomMostAt(x-1,y, tileID); }
            catch(std::out_of_range&) {}
            try { map->setBottomMostAt(x,y, tileID); }
            catch(std::out_of_range&) {}
        }
    }



    /*--------------------------------------------------------------------------------
        Function    : getCurvyPathBetweenPoints
        Description : Surprisingly, this function creates a tunnel between two
                      specified points.  This algorithm was written by Kusigrosz and
                      the source is available at http://www.freak-search.com/en/thread
                      /3040900/winding_ways_for_your_roguelike_games
        Inputs      : empty Point vector, Map object, random number generator, 
                      begin point, end point
        Outputs     : None
        Return      : void
    --------------------------------------------------------------------------------*/
    void getCurvyPathBetweenPoints(vector<Point>* path, const MapPtr map, TCODRandom* rand, 
                                    const Point& origin, const Point& dest)
    {
        int pertamt = 10; // default perturbation amount
        windPath(map, rand, path, origin, dest, pertamt);
    }
}
