#pragma once

#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <cstdio>

using namespace cv;
using namespace std;

class ImageTools
{
public:
	static vector<Vec3f>* detectCircles(Mat& src);
};
