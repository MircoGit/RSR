// OpenCVTemplate.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "ImageTools.h"
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <cstdio>

using namespace cv;

/** @function main */
int main(int argc, char** argv)
{
	Mat src, src_gray;

	/// Read the image
	src = imread( argv[1], 1 );

	if( !src.data )
	{ return -1; }

	vector<Vec3f>* circles;
	circles = ImageTools::detectCircles(src);
  
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
	namedWindow( "Road sign detection", CV_WINDOW_AUTOSIZE );
	imshow( "Road sign detection", src );

	waitKey(0);
	return 0;
}