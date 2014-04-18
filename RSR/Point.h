#pragma once
#include <string>
using namespace std;

class Point
{
public:
	Point();
	Point(float px, float py);

	string tostring()                   const;
	double distanceFrom(const Point& p) const;

	bool operator == (Point& other);
	bool operator != (Point& other);
	bool operator  < (Point& other);
	bool operator  > (Point& other);
	bool operator <= (Point& other);
	bool operator >= (Point& other);
private:
	float x;
	float y;
};


ostream& operator << (ostream& os, const Point& p);