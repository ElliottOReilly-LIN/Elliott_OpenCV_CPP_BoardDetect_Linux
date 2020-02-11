#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include<stdint.h>
#include<iostream>>

using namespace cv;
using namespace std;

int width, height;


int main(int argc, char** argv)
{

	// Loads an image, not Mat is essentially a Matrix, which is just an OpenCV data type.
	Mat original = imread("cvChess.jpg", IMREAD_GRAYSCALE);

	// This will hold the modified chess board image for manipulation
	Mat modified = imread("cvChess.jpg", IMREAD_GRAYSCALE);

	//Iterate through the image, row by column
	for (int x = 0; x < 150; x++)
	{
		for (int y = 0; y < 150; y++)
		{

			/* Testing to be done in here..............
			 Note - 'at' is just a template container, can hold any data type,
			 we are basically saying get the pixel value 'at' that location
			 I make sure I use an unsigned int here so I know I can hold a 255 value for my pixel */


			int pixel = modified.at<uint8_t>(y, x);
			if (pixel == 255) {
				cout << " |W| " << "[" << x << ","<<  y << "]";
				
			}
			if (pixel == 0) {
			
				cout << " |B| " << "[" << x << "," << y << "]";


			}
		}


	}

	imshow("Original", original);
	imshow("Modified", modified);

	waitKey();


}
