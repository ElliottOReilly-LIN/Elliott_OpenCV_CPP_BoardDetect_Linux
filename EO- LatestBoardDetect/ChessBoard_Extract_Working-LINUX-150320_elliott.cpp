/***************************************************************************************
*    Title: ChessBoard_Detect_Linux.cpp
*    Author: Elliott O'Reilly Level 2 Student
*    Group 10 TSE Project
*    Date: 15/03/20
*    Code version: Final_V1
*    Availability: https://github.com/TSE-Group10/OpenCV_CPP_BoardDetect/tree/master/EO-%20LatestBoardDetect
*
***************************************************************************************/
#include <opencv2/opencv.hpp> //Include file for every supported OpenCV function
#include<iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

int main()
{
	char charCheckForEscKey = 0; // variable for escape key
	// Set up Matrices data types for the various input/output images
	Mat imgOriginal;
	Mat imgCanny;
	Mat imgBlurred; // For the noise filtration
	Mat gray; // For the grayscale output

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
		vector<vector<Point> > contours; // Vector for storing contour
		vector<Vec4i> hierarchy;
		findContours(imgBlurred, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
		// iterate through each contour.
		for (int i = 0; i < contours.size(); i++)
		{
			//  Find the area of contour
			double a = contourArea(contours[i], false);
			if (a > largest_area) {
				largest_area = a;
				//cout << i << " area  " << a << endl;
				// Store the index of largest contour
				largest_contour_index = i;
				// Find the bounding rectangle for biggest contour
				bounding_rect = boundingRect(contours[i]);
			}
		}
		Mat cropImg = imgBlurred(bounding_rect); //Create a new matrix to store the extracted cropped board image
		imwrite("cropOut.png", imgBlurred);

		Point offSet; // Create a new point to hold the offset of the centre of the 'cell'
		// This loop gives me access to the points stored on the contour
		for (size_t X = 0; X < contours.size(); ++X)
			for (size_t Y = 0; Y < contours[X].size(); Y++)
			{
				Point currentContourPixel = contours[X][Y]; //Store each point in a new contour point

				if (currentContourPixel.x < 100 && currentContourPixel.y < 100) { // We now this will be the top left square contour
					cout << "co-ors: x = " << currentContourPixel.x << " y = " << currentContourPixel.y << endl;

					offSet = Point(currentContourPixel.x + 50, currentContourPixel.y + 48); // The offset should logically be the center of the board.

					//rectangle(cropImg, Point(45, 45), Point(55, 55), Scalar(255, 255, 255), 1, 1, 0); // Simply draws a rectangle at given co-ords
					cv::putText(cropImg, "A8", cv::Point(50, 50), cv::FONT_HERSHEY_DUPLEX, 0.8, Scalar(102, 255, 102), 2); // Put some text on the image
				}
			}

		Point centredPx; // a point data stucture for the new found centre
		Point centre[8][8]; // A simple 2D Array place holder for the board

		for (size_t y = 0; y < 8; y++) {
			for (size_t x = 0; x < 8; x++)
			{
				centredPx = Point(offSet.x + x * 100, offSet.y + y * 100);
				centre[x][y] = centredPx; // Populate the array with the 64 points

				rectangle(cropImg, Point(centredPx.x - 5, centredPx.y - 5), Point(centredPx.x + 5, centredPx.y + 5), Scalar(255, 255, 255), 1, 1, 0); // Simply draws a rectangle at given co-ords
			}
		}

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

