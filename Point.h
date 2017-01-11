// ----------------------------------------------------
// Point class    -----------
// ----------------------------------------------------

#ifndef __POINT_H__
#define __POINT_H__

template<class TYPE>
class Point
{
public:

	TYPE x, y, z;

	Point()
	{}
	Point(TYPE x, TYPE y, TYPE z) : x(x), y(y), z(z)
	{}

	//we addd this constructor so we dont have to change all the calls to point
	Point(TYPE x, TYPE y) : x(x), y(y), z(0)
	{}

	// Operators ------------------------------------------------
	Point operator -(const Point &v) const
	{
		Point<TYPE> r;
		r.x = x - v.x;
		r.y = y - v.y;
		r.z = z - v.z;
		return(r);
	}

	Point operator + (const Point &v) const
	{
		Point<TYPE> r;
		r.x = x + v.x;
		r.y = y + v.y;
		r.z = z + v.z;
		return(r);
	}

	const Point& operator -=(const Point &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return(*this);
	}

	const Point& operator +=(const Point &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return(*this);
	}

	bool operator ==(const Point& v) const
	{
		return (x == v.x && y == v.y && z == v.z);
	}

	bool operator !=(const Point& v) const
	{
		return (x != v.x || y != v.y || z != v.z);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0 && z == 0);
	}

	Point& SetToZero()
	{
		x = y = z = 0;
		return(*this);
	}

	Point& Negate()
	{
		x = -x;
		y = -y;
		z = -z;
		return(*this);
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const Point& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;
		TYPE fz = z - v.z;

		return sqrt((fx*fx) + (fy*fy) + (fz*fz));
	}


	void BloquedFrom()
	{
		TYPE dx = std::abs(x);
		TYPE dy = std::abs(y);
		TYPE dz = std::abs(z);

		if ((dy < 5) && (dx < 5 || dz < 5))
		{
			dy = 50;
		}

		if (dx < dy && dx < dz)
		{
			y = 0;
			z = 0;
		}
		else if (dy < dx && dy < dz)
		{
			x = 0;
			z = 0;
		}
		else if (dz < dx && dz < dy)
		{
			x = 0;
			y = 0;
		}
	}

};

typedef Point<int> iPoint;
typedef Point<float> fPoint;

#endif // __POINT_H__