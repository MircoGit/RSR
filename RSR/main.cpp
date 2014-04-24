// OpenCVTemplate.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "ImageTools.h"
#include "RoadSign.h"
#include "RoadSignPath.h"
#include "opencv2\opencv.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <iostream>
#include <sstream>
#include <cstdio>

using namespace cv;

/** @function main */
int main(int argc, char** argv)
{	
	//Mat img_object = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
	Mat img_scene = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );

	vector<RoadSignPath> roadSignsPath;
	vector<RoadSignPath> pngs = ImageTools::getFilesList("..\\Images\\RoadSigns\\", "*.png");	
	vector<RoadSignPath> jpgs = ImageTools::getFilesList("..\\Images\\RoadSigns\\", "*.jpg");	
		
	roadSignsPath.insert(roadSignsPath.end(), pngs.begin(), pngs.end());
	roadSignsPath.insert(roadSignsPath.end(), jpgs.begin(), jpgs.end());

	vector<Vec3f>* circles;
	vector<Mat> croppedImages;
	vector<RoadSign> roadSigns;
	circles = ImageTools::detectCircles(img_scene);

	Mat img_object;

	for(int i=0; i< roadSignsPath.size(); ++i)
	{
		img_object = imread( roadSignsPath[i].getPath(), CV_LOAD_IMAGE_GRAYSCALE );

		cout << roadSignsPath[i].getName() << endl;
		
		for(int j=0; j<circles->size(); ++j)
		{
			Mat cropped_scene = ImageTools::crop(img_scene, (*circles)[j]);		  //Rectangle containing a potential circular road sign
			Mat img_object_scaled = ImageTools::scale(img_object, cropped_scene); //Scale the road sign template to the potential found road sign

			if(ImageTools::isObjectInScene(img_object_scaled, cropped_scene) && ImageTools::isOrientationCorrect(img_object_scaled, cropped_scene))
			{
				RoadSign rs((*circles)[j], roadSignsPath[i].getName());
				roadSigns.push_back(rs);
				break;
			}
		}
	}

	delete circles;

	//Draw the circle for the found road signs
	vector<RoadSign>::iterator it;
	for(it = roadSigns.begin(); it != roadSigns.end(); ++it)
	{
		RoadSign rs = (*it);
		Point center(cvRound(rs.getCircle()[0]), cvRound(rs.getCircle()[1]));
		int radius = cvRound(rs.getCircle()[2]);		

		// circle outline
		circle( img_scene, center, radius, Scalar(0,0,255), 3, 8, 0 );

		// Road sign name
		Point text_center(cvRound(rs.getCircle()[0]), cvRound(rs.getCircle()[1]));
		putText(img_scene, rs.getName(), text_center, FONT_HERSHEY_COMPLEX_SMALL, 1.0, cvScalar(0,0,0), 1, CV_AA);	
	}

	imshow("Scene", img_scene);

	cv::waitKey(0);
	return 0;
}