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
	image = imread("cvChess.jpg", 1);
	namedWindow("Display window", CV_WINDOW_AUTOSIZE);
	imshow("Display window", image);
	Mat gray;
	cvtColor(image, gray, CV_BGR2GRAY);
	Canny(gray, gray, 100, 200, 3);
	/// Find contours   
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);

	findContours(gray, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// Draw contours
	Mat drawing = Mat::zeros(gray.size(), CV_8UC3);

	int chessBoard[8][8]; // Set up a 2D array to hold the contour points found

	// This loop gives me access to the points stored on the contour
	for (size_t X = 0; X < contours.size(); ++X)
		for (size_t Y = 0; Y < contours[X].size(); Y++)
		{
			Point currentContourPixel = contours[X][Y]; //Store each point in a new contour point
			Point centredPixel; // Create a new point to hold the offset of the centre of the 'cell'

			if (currentContourPixel.x == 74 && currentContourPixel.y == 0) { // We now this will be the top left square contour
				cout << "co-ors: x = " << currentContourPixel.x << " y = " << currentContourPixel.y << endl;

				centredPixel = Point(currentContourPixel.x - 24, currentContourPixel.y + 50);
				chessBoard[0][0] = (centredPixel.x, centredPixel.y); // Populate my array with the new centered offset coordinate.

				cout << "\nCentre.X: " << centredPixel.x << "  Centre.Y = " << centredPixel.y;
				cout << "\nArray " << centredPixel;
			}
			//cout << countCoors << " Contour co-ors: x = " << currentContourPixel.x << " y = " << currentContourPixel.y << endl;
		}
	// Displays the contoured lines using the random colour generator
	for (int i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
	}

	imshow("Result window", drawing);
	waitKey(0);
	return 0;
}