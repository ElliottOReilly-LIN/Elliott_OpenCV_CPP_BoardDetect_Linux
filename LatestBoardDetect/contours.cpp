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

	int chessBoard[8][8]; // Set up a 2D array to hold the contour points found

	// This loop gives me access to the points stored on the contour
	for (size_t X = 0; X < contours.size(); ++X)
		for (size_t Y = 0; Y < contours[X].size(); Y++)
		{
			Point currentContourPixel = contours[X][Y]; //Store each point in a new contour point
			Point centredPixel; // Create a new point to hold the offset of the centre of the 'cell'
			//cout << "co-ors: x = " << currentContourPixel.x << " y = " << currentContourPixel.y << endl;



			if (currentContourPixel.x <= 10 && currentContourPixel.y <= 10) { // We now this will be the top left square contour
				cout << "co-ors: x = " << currentContourPixel.x << " y = " << currentContourPixel.y << endl;

				centredPixel = Point(currentContourPixel.x + 50, currentContourPixel.y + 50);
				chessBoard[0][0] = (centredPixel.x, centredPixel.y); // Populate my array with the new centered offset coordinate.

				cout << "Centre.X: " << centredPixel.x << "  Centre.Y = " << centredPixel.y <<"\n";
				cout << "\nArray " << centredPixel;
				rectangle(cropImage, Point(45, 45), Point(55, 55), Scalar(255, 255, 255), 1, 1, 0); // Simply draws a rectangle at given co-ords 
				cv::putText(drawing, "A8", cv::Point(30, 50), cv::FONT_HERSHEY_DUPLEX, 0.8, Scalar(102, 255, 102), 2); // Put some text on the image
			}
		}
		// Displays the contoured lines using the random colour generator
		for (int i = 0; i < contours.size(); i++)
		{
			// Not needed for us, just a representation of the contours for reference 
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		}
		
		imshow("Bounded and Cropped Contoured image", cropImage);
		cout << "\n----Bounded and Cropped Contoured image size-----\n" << rect.size();

		imshow("Result window", drawing);

		waitKey(0);
		return 0;
	}