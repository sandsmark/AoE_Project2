// ----------------------------------------------------
// Point class    -----------
// ----------------------------------------------------

#ifndef __P2POINT_H__
#define __P2POINT_H__

#include "p2Defs.h"
#include <cmath>

template <class TYPE>
class p2Point
{
public:
    TYPE x, y;

    p2Point()
    {
    }

    p2Point(const p2Point<TYPE> &v)
    {
        this->x = v.x;
        this->y = v.y;
    }

    p2Point(const TYPE &x, const TYPE &y)
    {
        this->x = x;
        this->y = y;
    }

    p2Point &create(const TYPE &x, const TYPE &y)
    {
        this->x = x;
        this->y = y;

        return (*this);
    }

    // Math ------------------------------------------------
    p2Point operator-(const p2Point &v) const
    {
        p2Point r;

        r.x = x - v.x;
        r.y = y - v.y;

        return (r);
    }

    p2Point operator+(const p2Point &v) const
    {
        p2Point r;

        r.x = x + v.x;
        r.y = y + v.y;

        return (r);
    }

    p2Point operator*(const float &m) const
    {
        p2Point r;

        r.x = x * m;
        r.y = y * m;

        return (r);
    }

    const p2Point &operator-=(const p2Point &v)
    {
        x -= v.x;
        y -= v.y;

        return (*this);
    }

    const p2Point &operator+=(const p2Point &v)
    {
        x += v.x;
        y += v.y;

        return (*this);
    }

    bool operator==(const p2Point &v) const
    {
        return (x == v.x && y == v.y);
    }

    bool operator!=(const p2Point &v) const
    {
        return (x != v.x || y != v.y);
    }

    // Utils ------------------------------------------------
    bool IsZero() const
    {
        return (x == 0 && y == 0);
    }

    p2Point &SetToZero()
    {
        x = y = 0;
        return (*this);
    }

    p2Point &Negate()
    {
        x = -x;
        y = -y;

        return (*this);
    }

    // Distances ---------------------------------------------
    TYPE DistanceTo(const p2Point &v) const
    {
        TYPE fx = x - v.x;
        TYPE fy = y - v.y;

        return (TYPE)sqrtf((fx * fx) + (fy * fy));
    }

    TYPE DistanceNoSqrt(const p2Point &v) const
    {
        TYPE fx = x - v.x;
        TYPE fy = y - v.y;

        return (fx * fx) + (fy * fy);
    }

    TYPE DistanceManhattan(const p2Point &v) const
    {
        return abs(v.x - x) + abs(v.y - y);
    }

    TYPE DistanceOctile(const p2Point &v) const
    {
        int xDistance = abs(x - v.x);
        int yDistance = abs(y - v.y);
        if (xDistance > yDistance) {
            return 14 * yDistance + 10 * (xDistance - yDistance);
        } else {
            return 14 * xDistance + 10 * (yDistance - xDistance);
}
    }

    void Normalize()
    {
        float module = sqrt(x * x + y * y);
        x = x / module;
        y = y / module;
    }
};

typedef p2Point<int> iPoint;
typedef p2Point<float> fPoint;

#endif // __P2POINT_H__