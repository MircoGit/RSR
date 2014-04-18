#pragma once

#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "RoadSignPath.h"
#include <cstdio>

using namespace cv;
using namespace std;

class ImageTools
{
public:
	static vector<Vec3f>* detectCircles(const Mat& src);
	static Mat scale(const Mat& from, const Mat& to);
	static Mat crop(const Mat& src, Rect croppingRect);
	static Mat crop(const Mat& src, Vec3f croppingCircle);
	static void compareImgHisto(Mat& src, Mat& test1, Mat& test2);
	static bool isObjectInScene(const Mat& img_object, const Mat& img_scene);
	static vector<RoadSignPath> getFilesList(string dir, string filter);

private:
	static Mat circleize(const Mat& src);			
	static bool containsPointMoreThanOnce(vector<KeyPoint>& keypoints, vector<DMatch>& good_matches);
};

