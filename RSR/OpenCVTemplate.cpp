// OpenCVTemplate.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include "opencv2\opencv.hpp"

using namespace cv;

int _tmain(int argc, _TCHAR* argv[])
{
	Mat img;
	img = imread("opencv.jpg");
	imshow("OpenCVTemplate", img);

	waitKey();

	return 0;
}

