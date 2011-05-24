#ifndef POINT_HPP
#define POINT_HPP

#include <iostream>

#include "CheckedSave.hpp"
#include "Types.hpp"

#include "libtcod.hpp"

namespace rlns
{
    namespace utl
    {
        /*--------------------------------------------------------------------------------
            Class       : Point
            Description : Wrapper class for the map coordinates of an object
            Parents     : None
            Children    : None
            Friends     : None
        --------------------------------------------------------------------------------*/
        class Point
        {
        // Member Variables
            private:
                int x,y;

        // Member Functions
            public:
                Point(): x(0), y(0) {}
                Point(const int a, const int b): x(a), y(b) {}
                Point(RLNSZip& zip): x(zip.getInt()), y(zip.getInt()) {} // used for loading save games

                void saveToDisk(RLNSZip&) const;
                void loadFromDisk(RLNSZip&) {}

                Point& operator+=(const Point&);
                Point& operator-=(const Point&);
                bool operator==(const Point&) const;
                bool operator!=(const Point&) const;
                bool operator<(const Point&) const;
                friend std::ostream& operator<<(std::ostream&, const Point&);

                int getX() const {return x;}
                int getY() const {return y;}
                void setX(const int a) {x = a;}
                void setY(const int b) {y = b;}

                void shift(DirectionType);
                Point neighbor(DirectionType) const;
                bool withinBounds(const Point&, const Point&) const;
                bool withinBounds(const int, const int, const int, const int) const;
        };



        // save the Point to the given RLNSZip save buffer
        inline void Point::saveToDisk(RLNSZip& zip) const
        {
            zip.putInt(x);
            zip.putInt(y);
        }


    // Point overloaded operators
        inline Point& Point::operator+=(const Point& rhs)
        {
            x += rhs.getX();
            y += rhs.getY();
            return *this;
        }

        inline Point& Point::operator-=(const Point& rhs)
        {
            x -= rhs.getX();
            y -= rhs.getY();
            return *this;
        }

        inline bool Point::operator==(const Point& rhs) const
        {
            bool result = true;
            if(x != rhs.getX()) result = false;
            if(y != rhs.getY()) result = false;
            return result;
        }

        inline bool Point::operator!=(const Point& rhs) const
        {
            return !(*this == rhs);
        }

        // used when ordering points in a std::map, not really a 
        // strict way of determining Point < Point.
        inline bool Point::operator<(const Point& rhs) const
        {
            int sumthis = x + y;
            int sumrhs = rhs.getX() + rhs.getY();
            return sumthis < sumrhs;
        }

        inline Point operator+(const Point& lhs, const Point& rhs)
        {
            Point temp(lhs);
            return temp += rhs;
        }

        inline Point operator-(const Point& lhs, const Point& rhs)
        {
            Point temp(lhs);
            return temp -= rhs;
        }

        inline std::ostream& operator<<(std::ostream& os, const Point& pt)
        {
            os << "(" << pt.getX() << "," << pt.getY() << ")";
            return os;
        }

        inline Point Point::neighbor(DirectionType dir) const
        {
            Point pt(*this);
            pt.shift(dir);
            return pt;
        }



    // Point non-member functions
        /*--------------------------------------------------------------------------------
            Function    : greatest/leastX/Y
            Description : returns the Point with the greatest or least X or Y attribute
            Inputs      : two Points
            Outputs     : None
            Return      : Point
        --------------------------------------------------------------------------------*/
        inline Point greatestX(const Point& p1, const Point& p2)
        {
            return (p1.getX() > p2.getX()) ? p1 : p2;
        }
        inline Point leastX(const Point& p1, const Point& p2)
        {
            return (p1.getX() < p2.getX()) ? p1 : p2;
        }
        inline Point greatestY(const Point& p1, const Point& p2)
        {
            return (p1.getY() > p2.getY()) ? p1 : p2;
        }
        inline Point leastY(const Point& p1, const Point& p2)
        {
            return (p1.getY() < p2.getY()) ? p1 : p2;
        }
    }
}

#endif
