#pragma once

#include <string>
#include "opencv2\opencv.hpp"

using namespace std;
using namespace cv;

class RoadSign
{
public:
	RoadSign(void);
	RoadSign(Vec3f circle, string name);
	~RoadSign(void);

	Vec3f getCircle();
	string getName();

private:
	string name;
	Vec3f circle;
};

