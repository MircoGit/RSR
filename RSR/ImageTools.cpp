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
vector<Vec3f>* ImageTools::detectCircles(const Mat& src)
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
	return ImageTools::circleize(croppedImage);
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