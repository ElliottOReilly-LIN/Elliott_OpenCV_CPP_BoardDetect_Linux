#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "highgui_c.h"
#include <iostream>
#include <opencv2/features2d.hpp>

using namespace cv;
using namespace std;

int main()
{
	char charCheckForEscKey = 0; // variable for escape key
	// Set up Matrices data types for the various input/output images 
	Mat imgOriginal;
	Mat imgCanny;
	Mat imgBlurred;
	Mat gray;

	cv::VideoCapture capWebcam(0);   // declare a VideoCapture object to associate webcam, 0 means use 1st (default) webcam

	if (capWebcam.isOpened() == false)  //  To check if object was associated to webcam successfully
	{
		std::cout << "error: Webcam connect unsuccessful\n"; // if not then print error message
		return(0);            // and exit program
	}
	while (charCheckForEscKey != 27 && capWebcam.isOpened()) {            // until the Esc key is pressed or webcam connection is lost
		bool blnFrameReadSuccessfully = capWebcam.read(imgOriginal);   // get next frame

		if (!blnFrameReadSuccessfully || imgOriginal.empty()) {    // if frame read unsuccessfully
			std::cout << "error: frame can't read \n";      // print error message
			break;
		}

		cvtColor(imgOriginal, gray, CV_BGR2GRAY);// Convert to gray
		Canny(gray, imgCanny, 100, 200, 3); // Simple Line detection function

		GaussianBlur(imgCanny, imgBlurred, cv::Size(5, 5), 1.8);    // Reduce the noise on the image
		/// Find contours   
		imshow("gray", imgBlurred); int largest_area = 0;
		int largest_contour_index = 0;
		Rect bounding_rect;
		vector<vector<Point>> contours; // Vector for storing contour
		vector<Vec4i> hierarchy;
		findContours(imgBlurred, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		// iterate through each contour.
		for (int i = 0; i < contours.size(); i++)
		{
			//  Find the area of contour
			double a = contourArea(contours[i], false);
			if (a > largest_area) {
				largest_area = a; cout << i << " area  " << a << endl;
				// Store the index of largest contour
				largest_contour_index = i;
				// Find the bounding rectangle for biggest contour
				bounding_rect = boundingRect(contours[i]);
			}
		}
		Mat cropImg = imgBlurred(bounding_rect);
		imwrite("cropOut.png", imgBlurred);

		Scalar color(255, 255, 255);  // color of the contour in the image
		//Draw the contour and rectangle
		drawContours(imgOriginal, contours, largest_contour_index, color, CV_FILLED, 8, hierarchy);
		rectangle(imgOriginal, bounding_rect, Scalar(0, 255, 0), 2, 8, 0);
		imshow("Largest Contour", cropImg);
		namedWindow("Display window", CV_WINDOW_AUTOSIZE);
		imshow("Display window", imgOriginal);
		charCheckForEscKey = cv::waitKey(1);        // delay and get key press
	}
	return 0;
}
