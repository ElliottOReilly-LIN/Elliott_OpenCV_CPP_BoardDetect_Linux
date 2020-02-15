#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
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

	int countCoors = 0;

	string chessBoard[8][8]; // Set up a 2D array to hold the contour points found

	for (size_t cC = 0; cC < contours.size(); ++cC)
		for (size_t cP = 0; cP < contours[cC].size(); cP++)
		{

			Point currentContourPixel = contours[cC][cP];
			//cout << countCoors << " Contour co-ors: x = " << currentContourPixel.x << " y = " << currentContourPixel.y << endl;
			countCoors++;
			// do whatever you want

			// if the current contour pixel is within these bounds I populate my array with the co-ords 

			if (currentContourPixel.x < 74 && currentContourPixel.y <= 74) {

				chessBoard[0][0] = " - A8 - ";
				cout << chessBoard[0][0];

				cout << " Sq. A8:(" << currentContourPixel.x << "," << currentContourPixel.y << ")" << endl;
				//---my code here to display a rectangle in the given co-ordinates
				rectangle(drawing, Point(15, 15), Point(55, 55), Scalar(255, 255, 255), 2, 8, 0);
				cv::putText(drawing, "A8", cv::Point(20, 40), cv::FONT_HERSHEY_DUPLEX, 0.8, Scalar(118, 185, 0), 2);
			}
			if (currentContourPixel.x > 74 && currentContourPixel.x < 150 && currentContourPixel.y <= 74) {

				chessBoard[0][1] = " - B8 - ";
				cout << chessBoard[0][1];

				cout << " Sq. B8:(" << currentContourPixel.x << "," << currentContourPixel.y << ")" << endl;

			}
			if (currentContourPixel.x > 150 && currentContourPixel.x < 225 && currentContourPixel.y <= 74) {

				chessBoard[0][2] = " - C8 - ";
				cout << chessBoard[0][2];

				cout << " Sq. B8:(" << currentContourPixel.x << "," << currentContourPixel.y << ")" << endl;

			}
			if (currentContourPixel.x > 300 && currentContourPixel.x < 375 && currentContourPixel.y <= 74) {

				chessBoard[0][3] = " - D8 - ";
				cout << chessBoard[0][3];

				cout << " Sq. B8:(" << currentContourPixel.x << "," << currentContourPixel.y << ")" << endl;

			}
		}
	cout << "\nNumber of contours is: " << contours.size();
	cout << "\nNumber of total points on contours " << countCoors;


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