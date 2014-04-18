#include "stdafx.h"
#include "ImageTools.h"
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "Vector2D.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <Windows.h>
#include <tchar.h>


/**
*  Return a vector of circles found in an image. Circle are represented as Vec3F : 
*  vec[0] = centerX
*  vec[1] = centerY
*  vec[2] = radius
*  /!\ you must free the pointer returned by this method when not needed.
*/
vector<Vec3f>* ImageTools::detectCircles(const Mat& src)
{
	Mat src_gray = src;
	vector<Vec3f>* circles = new vector<Vec3f>();

	if(src.data)		
	{
		/// Convert it to gray
		//cvtColor( src, src_gray, CV_BGR2GRAY );

		/// Reduce the noise so we avoid false circle detection
		GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );	

		/// Apply the Hough Transform to find the circles
		HoughCircles( src_gray, *circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 170, 70, src_gray.rows/100000000, src_gray.rows );
	}

	cout << "[ImageTools::detectCircles] " << circles->size() << " circles found on image" << endl;

	return circles;
}

Mat ImageTools::scale(const Mat& from, const Mat& to)
{
	Mat resizedImage = from.clone();

	double fromWidthHeightRatio = 1.0 * from.cols / from.rows;

	Size newSize;

	//Resize considering width
	if(to.cols < to.rows)
	{
		newSize.width = to.cols;
		newSize.height = newSize.width / fromWidthHeightRatio;
	}
	//Or heigth
	else
	{
		newSize.height = to.rows;
		newSize.width = newSize.height * fromWidthHeightRatio;
	}

	cv::resize(from, resizedImage, newSize);	

	return resizedImage;
}

Mat ImageTools::crop(const Mat& src, Rect croppingRect)
{
	Mat croppedImage = src(croppingRect);
	return croppedImage;
}

Mat ImageTools::crop(const Mat& src, Vec3f croppingCircle)
{
	int x = (int) (croppingCircle[0] - croppingCircle[2]);
	int y = (int) (croppingCircle[1] - croppingCircle[2]);
	int w = (int) (2 * croppingCircle[2]);
	int h = w;
	Rect croppingRect(x, y, w, h);

	Mat croppedImage =  ImageTools::crop(src, croppingRect);
	return croppedImage;//ImageTools::circleize(croppedImage);
}

bool ImageTools::isObjectInScene(const Mat& img_object, const Mat& img_scene)
{
	bool result = false;

	if( !img_object.data || !img_scene.data )
	{ std::cout<< " --(!) Error reading images " << std::endl; return false; }

	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	SurfFeatureDetector detector( minHessian );

	std::vector<KeyPoint> keypoints_object, keypoints_scene;

	detector.detect( img_object, keypoints_object );
	detector.detect( img_scene, keypoints_scene );

	//-- Step 2: Calculate descriptors (feature vectors)
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;

	extractor.compute( img_object, keypoints_object, descriptors_object );
	extractor.compute( img_scene, keypoints_scene, descriptors_scene );

	//-- Step 3: Matching descriptor vectors using FLANN matcher
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	matcher.match( descriptors_object, descriptors_scene, matches );

	double max_dist = 0; double min_dist = 100;

	//-- Quick calculation of max and min distances between keypoints
	for( int i = 0; i < descriptors_object.rows; i++ )
	{ double dist = matches[i].distance;
	if( dist < min_dist ) min_dist = dist;
	if( dist > max_dist ) max_dist = dist;
	}

	printf("-- Max dist : %f \n", max_dist );
	printf("-- Min dist : %f \n", min_dist );

	//-- Draw only "good" matches (i.e. whose distance is less than 3*min_dist )
	std::vector< DMatch > good_matches;

	for( int i = 0; i < descriptors_object.rows; i++ )
	{ if( matches[i].distance < 1.7*min_dist )
	{ good_matches.push_back( matches[i]); }
	}		

	return !containsPointMoreThanOnce(keypoints_scene, good_matches);
}

Mat ImageTools::circleize(const Mat& src)
{
	assert(src.rows == src.cols);

	Mat circleizedSrc = src .clone();

	int radius2 = (src.rows / 2) * (src.rows / 2); //square radius
	int cx = src.rows / 2;
	int cy = src.rows / 2;

	int d = 0;
	int dx = 0;
	int dy = 0;

	for(int i=0; i<circleizedSrc.rows; ++i)
	{
		for(int j=0; j<circleizedSrc.cols; ++j)
		{
			dx = abs(i-cx);
			dy = abs(j-cy);
			d = dx * dx + dy * dy;

			//Pixel out of the circle
			if( d > radius2 )
			{
				circleizedSrc.at<Vec3b>(i, j) = 0;
			}
		}
	}
	return circleizedSrc;
}

bool ImageTools::containsPointMoreThanOnce(vector<KeyPoint>& keypoints, vector<DMatch>& good_matches)
{
	vector<Vector2D> foundKeypoints;
	KeyPoint* keypoint = NULL;

	for(int i=0; i<good_matches.size(); ++i)
	{
		keypoint = &keypoints[good_matches[i].trainIdx];

		Vector2D v(keypoint->pt.x, keypoint->pt.y);		
		if(find(foundKeypoints.begin(), foundKeypoints.end(), v) == foundKeypoints.end())
		{
			foundKeypoints.push_back(v);		
		}
		else
		{
			return true;
		}		
	}			

	return false;
}

void ImageTools::compareImgHisto(Mat& src, Mat& test1, Mat& test2)
{
	// Declaration of the HSV images
	Mat hsv_base;
	Mat hsv_test1;
	Mat hsv_test2;

	// Convertion of the RGB images to HSV images
	cvtColor( src, hsv_base, CV_BGR2HSV );
	cvtColor( test1, hsv_test1, CV_BGR2HSV );
	cvtColor( test2, hsv_test2, CV_BGR2HSV );

	// Using 30 bins for hue and 32 for saturation
	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	// Hue varies from 0 to 256, saturation from 0 to 180
	float h_ranges[] = { 0, 256 };
	float s_ranges[] = { 0, 180 };

	const float* ranges[] = { h_ranges, s_ranges };

	// Use the o-th and 1-st channels
	int channels[] = { 0, 1 };

	// Declaration of the histograms
	MatND hist_base;
	MatND hist_half_down;
	MatND hist_test1;
	MatND hist_test2;

	// Calculation of the histograms for the HSV images
	calcHist( &hsv_base, 1, channels, Mat(), hist_base, 2, histSize, ranges, true, false );
	normalize( hist_base, hist_base, 0, 1, NORM_MINMAX, -1, Mat() );

	calcHist( &hsv_test1, 1, channels, Mat(), hist_test1, 2, histSize, ranges, true, false );
	normalize( hist_test1, hist_test1, 0, 1, NORM_MINMAX, -1, Mat() );

	calcHist( &hsv_test2, 1, channels, Mat(), hist_test2, 2, histSize, ranges, true, false );
	normalize( hist_test2, hist_test2, 0, 1, NORM_MINMAX, -1, Mat() );

	// We use four comparaison methods on the histograms
	for( int i = 0; i < 4; i++ )
	{
		int compare_method = i;
		double base_base = compareHist( hist_base, hist_base, compare_method );
		double base_test1 = compareHist( hist_base, hist_test1, compare_method );
		double base_test2 = compareHist( hist_base, hist_test2, compare_method );

		printf( " Method [%d] Perfect, Base-Test(1), Base-Test(2) : %f, %f, %f \n", i, base_base, base_test1, base_test2 );
	}
}

vector<RoadSignPath> ImageTools::getFilesList(string dir, string filter)
{
	vector<RoadSignPath> filesList;
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;	
	
	string path = dir + filter;	
	wstring wdir = wstring(path.begin(), path.end());

	if((hFind = FindFirstFile(wdir.c_str(), &FindFileData)) != INVALID_HANDLE_VALUE){
		do{						
			wstring wstr = FindFileData.cFileName;			
			string str = string(wstr.begin(), wstr.end());
			filesList.push_back(RoadSignPath(dir + str, str));				
		}while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}

	return filesList;
}
