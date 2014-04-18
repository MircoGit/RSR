#include "stdafx.h"
#include "Point.h"
#include <cmath>
#include <sstream>

Point::Point(float px, float py)
	: x(px), y(py)
{}

Point::Point()
	: x(0), y(0)
{}


bool Point::operator == (Point& other)
{
	return this->x == other.x && this->y == other.y;
}

bool Point::operator != (Point& other)
{
	return ! (*this == other);
}

bool Point::operator  < (Point& other)
{
	return this->x < other.x || (this->x == other.x && this->y < other.y);
}

bool Point::operator > (Point& other)
{
	return other < *this;
}

bool Point::operator <= (Point& other)
{
	return ! (other > *this);
}

bool Point::operator >= (Point& other)
{
	return ! (*this < other);
}

double Point::distanceFrom(const Point& p) const
{
	return sqrt( (x - p.x)*(x-p.x) + (y - p.y)*(y - p.y));
}

string Point::tostring() const
{
	ostringstream out;
	out << "(" << x << ", " << y << ")";
	return out.str();
}

ostream& operator << (ostream& os, const Point& p)
{
	os << p.tostring();
	return os;
}

