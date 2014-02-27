// OpenCVTemplate.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;

vector<Vec3f>* detectCircles(Mat& src);

/** @function main */
int main(int argc, char** argv)
{
	Mat src, src_gray;

	/// Read the image
	src = imread( argv[1], 1 );

	if( !src.data )
	{ return -1; }

	vector<Vec3f>* circles;
	circles = detectCircles(src);
  
	/// Draw the circles detected
	for( size_t i = 0; i < circles->size(); i++ )
	{
		Point center(cvRound((*circles)[i][0]), cvRound((*circles)[i][1]));
		int radius = cvRound((*circles)[i][2]);
		// circle center
		circle( src, center, 3, Scalar(0,255,0), -1, 8, 0 );
		// circle outline
		circle( src, center, radius, Scalar(0,0,255), 3, 8, 0 );
   
	}

	delete circles;

	/// Show your results
	namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
	imshow( "Hough Circle Transform Demo", src );

	waitKey(0);
	return 0;
}

vector<Vec3f>* detectCircles(Mat& src)
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

	std::cout << "[detectCircles] " << circles->size() << " circles found on image";

	return circles;
}