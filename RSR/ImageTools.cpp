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

