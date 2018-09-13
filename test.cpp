#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;

int main(int argc, char* argv[]) {
	cv::Mat image = cv::imread("C:/Users/sfsfk/Desktop/j.jpg", cv::IMREAD_COLOR);
	int row = image.rows;//284
	int col = image.cols;//436

	int matrix_red[284][436] = {}; // c++은 변수로 배열의 동적 할당이 안됨으로 이 함수가 시작되기 전에 잘 가공된 약속된 크기의 이미지를 불러와야함
	int matrix_green[284][436] = {};
	int matrix_blue[284][436] = {};

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			matrix_red[i][j] = image.at<cv::Vec3b>(i, j)[2] + 0; // red value store
			matrix_green[i][j] = image.at<cv::Vec3b>(i, j)[1] + 0; // green value store
			matrix_blue[i][j] = image.at<cv::Vec3b>(i, j)[0] + 0; // blue value store
		}
	}
	for (int i = 0; i < col; i++) {
		printf("%d ", matrix_red[0][i]);
	}
	//cout << matrix_red << endl;



	//cv::Mat bgr[3];
	//cv::split(test, bgr); 

	//
	//cv::namedWindow("Original");
	//cv::namedWindow("red");
	//cv::namedWindow("green");
	//cv::namedWindow("blue");
	//

	//cout << bgr[0] << endl; //blue	
	//cout << bgr[1] << endl; //green
	//cout << bgr[2] << endl; //red

	//
	//cv::imshow("original", test);
	//cv::imshow("red", bgr[2]);
	//cv::imshow("green", bgr[1]);
	//cv::imshow("blue", bgr[0]);
	//

	cv::waitKey(0);

	return 0;
}