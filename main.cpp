#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;

int main(int argc, char * argv[]) {
	cv::Mat test = cv::imread("C:/Users/sfsfk/Desktop/j.jpg", cv::IMREAD_COLOR);
	cv::Mat bgr[3];
	cv::split(test, bgr);

	/*
	cv::namedWindow("Original");
	cv::namedWindow("red");
	cv::namedWindow("green");
	cv::namedWindow("blue");
	*/

	//cout << bgr[0] << endl; //blue
	
	//cout << bgr[1] << endl; //green
	cout << bgr[2] << endl; //red
	

	/*
	cv::imshow("original", lena);
	cv::imshow("red", bgr[2]);
	cv::imshow("green", bgr[1]);
	cv::imshow("blue", bgr[0]);
	*/
	cv::waitKey(0);
	return 0;
}