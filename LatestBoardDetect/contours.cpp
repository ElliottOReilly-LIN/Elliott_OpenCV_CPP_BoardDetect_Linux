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
	Mat image;
	image = imread("800noBRD.png", 1); // REad in my image, doesn't matter which my boundingRect function will deal with it
	namedWindow("Display window", CV_WINDOW_AUTOSIZE);
	imshow("Display window", image);
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY); // Covert to gray-scale binary image
	Canny(gray, gray, 100, 200, 3);
	/// Find contours   
	vector<vector<Point> > contours; // Create a vector point of contours
	vector<Vec4i> hierarchy; // Need to read up on this
	RNG rng(12345); // Random number bernerato
	   
	// Experimenting with drawing a bounding rectangle around the board
	Rect rect;
	rect = boundingRect(gray);
	Mat cropImage = gray(rect);
	
	//---------------This important function save the output image-----------------------------------
	imwrite("matCrop3.png", cropImage);

	findContours(cropImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// Draw contours
	Mat drawing = Mat::zeros(gray.size(), CV_8UC3);

	imwrite("contOut.png", gray);
	
	Point offSet; // Create a new point to hold the offset of the centre of the 'cell'
	

	// This loop gives me access to the points stored on the contour
	for (size_t X = 0; X < contours.size(); ++X)
		for (size_t Y = 0; Y < contours[X].size(); Y++)
		{
			Point currentContourPixel = contours[X][Y]; //Store each point in a new contour point
			
			if (currentContourPixel.x == 0 && currentContourPixel.y == 2) { // We now this will be the top left square contour
				//cout << "co-ors: x = " << currentContourPixel.x << " y = " << currentContourPixel.y << endl;

				offSet = Point(currentContourPixel.x + 50, currentContourPixel.y + 48);

				rectangle(cropImage, Point(45, 45), Point(55, 55), Scalar(255, 255, 255), 1, 1, 0); // Simply draws a rectangle at given co-ords 
				cv::putText(drawing, "A8", cv::Point(30, 50), cv::FONT_HERSHEY_DUPLEX, 0.8, Scalar(102, 255, 102), 2); // Put some text on the image
			}
		}
	
	Point centredPx;
	Point centre[8][8];

	for (size_t y = 0; y < 8; y++)	{
		for (size_t x = 0; x < 8; x++)
		{
			centredPx = Point(offSet.x + x*100, offSet.y + y*100);
			centre[x][y] = centredPx;

			rectangle(cropImage, Point(centredPx.x -5, centredPx.y -5 ), Point(centredPx.x +5, centredPx.y +5), Scalar(255, 255, 255), 1, 1, 0); // Simply draws a rectangle at given co-ords 
		}
	}
	cout << "Test cases ----------\n";
	cout << "Top row, square (0,7) : " << centre[0][7] << endl;
	cout << "4th row, square (4,2) : " << centre[4][2] << endl;

		// Displays the contoured lines using the random colour generator
		for (int i = 0; i < contours.size(); i++)
		{
			// Not needed for us, just a representation of the contours for reference 
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		}
		
		imshow("BoundingRect/Contoured_image", cropImage);
		cout << "\n----Bounded and Cropped Contoured image size-----\n" << rect.size();

		imshow("Result window", drawing);

		waitKey(0);
		return 0;
	}
	