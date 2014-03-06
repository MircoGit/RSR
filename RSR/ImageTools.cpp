#include "stdafx.h"
#include "ImageTools.h"
#include <iostream>

/**
 *  Return a vector of circles found in an image. Circle are represented as Vec3F : 
 *  vec[0] = centerX
 *  vec[1] = centerY
 *  vec[2] = radius
 *  /!\ you must free the pointer returned by this method when not needed.
 */
vector<Vec3f>* ImageTools::detectCircles(Mat& src)
{
	Mat src_gray;
	vector<Vec3f>* circles = new vector<Vec3f>();

	if(src.data)		
	{
		/// Convert it to gray
		cvtColor( src, src_gray, CV_BGR2GRAY );

		/// Reduce the noise so we avoid false circle detection
		GaussianBlur( src_gray, src_gray, Size(9, 9), 2, 2 );	
  
		/// Apply the Hough Transform to find the circles
		HoughCircles( src_gray, *circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 66, 0, 0 );
	}

	cout << "[ImageTools::detectCircles] " << circles->size() << " circles found on image" << endl;

	return circles;
}

