#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;

void diff(int * width, int col) {
	for (int i = 0; i < col-1; i++) {
		width[i] = width[i + 1] - width[i];
	}
	width[col - 1] = 0;
}

int main(int argc, char* argv[]) {
	//cv::Mat image = cv::imread("C:/Users/sfsfk/Desktop/test.jpg", cv::IMREAD_COLOR);
	cv::Mat image = cv::imread("C:/Users/sfsfk/Desktop/js.png", cv::IMREAD_COLOR);
	int row = image.rows;//470 세로 427
	int col = image.cols;//624 가로 398
	/*흑백 전환*/
	cv::Mat black;
	cv::cvtColor(image, black, CV_BGR2YCrCb);
	cv::inRange(black, cv::Scalar(0, 135, 80), cv::Scalar(255, 171, 124), black);
	/*자르기*/
	int left = 0, right = 0, top = 0, bottom = 0;
	int read = -1;	
		
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			read = black.at<uchar>(i, j);
			if (read == 0) {
				top = i;
				read = -1;
				break;
			}
		}
	}
	for (int i = row-1; i >= 0; i--) {
		for (int j = 0; j < col; j++) {
			read = black.at<uchar>(i, j);
			if (read == 0) {
				bottom = i;
				read = -1;
				break;
			}
		}
	}
	for (int i = 0; i < col; i++) {
		for (int j = 0; j < row; j++) {
			read = black.at<uchar>(j, i);
			if (read == 0) {
				left = i;
				read = -1;
				break;
			}
		}
	}
	for (int i = col-1; i >= 0; i--) {
		for (int j = 0; j < row; j++) {
			read = black.at<uchar>(j, i);
			if (read == 0) {
				right = i;
				read = -1;
				break;
			}
		}
	}
	cout << "left" << left << "right" << right << endl;
	cout << "top" << top << "bottom" << bottom << endl;
	cv::Mat capture = image(cv::Range(bottom, top),cv::Range(right, left));
	//int width[624] = { 0 };
	//float width_f[624] = { 0,0 };
	//int red = 0;
	//int green = 0;
	//int blue = 0;
	//for (int i = 0; i < row; i++) {
	//	for (int j = 0; j < col; j++) {
	//		red = image.at<cv::Vec3b>(i, j)[2];
	//		green = image.at<cv::Vec3b>(i, j)[1];
	//		blue = image.at<cv::Vec3b>(i, j)[0];
	//		width[j] += (red + green + blue);
	//	}
	//}
	//
	//
	//for (int i = 0; i < col; i++) {
	//	width_f[i] = (float)width[i] / (3 * row);
	//	width[i] = (int)width_f[i];
	//}

	//diff(width, col);
	///*for (int i = 0; i < col; i++) {
	//	if (i % 10 == 0) {
	//		printf("\n");
	//	}
	//	printf("%3d, ", width[i]);
	//}*/
	//
	////여기서 대충 계산 되었다고 치고
	//int long_under = 0, long_over = 0;
	//int cur_under = 0, cur_over = 0;

	//int start = 0, end = 0;
	//int cur_start = 0, cur_end = 0;

	//for (int i = 0; i < col; i++) {
	//	if (width[i] < 0) {//값이 감소추세일 때			
	//		cur_under++;
	//		if (cur_under == 1) {
	//			cur_start = i;
	//		}			

	//		if (cur_over > long_over) {
	//			end = cur_end;
	//			long_over = cur_over;
	//		}
	//		cur_over = 0;
	//		cur_end = 0;
	//	}
	//	else if(width[i] > 0) {//증가추세일 때
	//		if (cur_under > long_under) {//최근 측정 길이가 가장 길다면
	//			start = cur_start;
	//			long_under = cur_under;
	//		}
	//		cur_under = 0;
	//		cur_start = 0;

	//		cur_over++;
	//		if (cur_over == 1) {
	//			cur_end = i;
	//		}
	//	}
	//	else {//0일 경우
	//		if (cur_under > long_under) {//최근 측정 길이가 가장 길다면
	//			start = cur_start;
	//			long_under = cur_under;
	//		}
	//		cur_under = 0;
	//		cur_start = 0;

	//		if (cur_over > long_over) {
	//			end = cur_end;
	//			long_over = cur_over;
	//		}
	//		cur_over = 0;
	//		cur_end = 0;
	//	}
	//	
	//	//printf("%d, ",start);
	//}	
	//cout << start << endl;
	//cout << end << endl;
	cv::imshow("draw capture", capture);
	/*for (int i = 0; i < row; i++) {
		image.at<cv::Vec3b>(i, left)[0] = 255;
		image.at<cv::Vec3b>(i, right)[0] = 255;
		image.at<cv::Vec3b>(i, left)[1] = 255;
		image.at<cv::Vec3b>(i, right)[1] = 255;
		image.at<cv::Vec3b>(i, left)[2] = 255;
		image.at<cv::Vec3b>(i, right)[2] = 255;
	}
	for (int i = 0; i < col; i++) {
		image.at<cv::Vec3b>(top, i)[0] = 255;
		image.at<cv::Vec3b>(bottom, i)[0] = 255;
		image.at<cv::Vec3b>(top, i)[1] = 255;
		image.at<cv::Vec3b>(bottom, i)[1] = 255;
		image.at<cv::Vec3b>(top, i)[2] = 255;
		image.at<cv::Vec3b>(bottom, i)[2] = 255;
	}*/
	cv::imshow("draw black", black);
	cv::imshow("draw line", image);	
	cv::waitKey(0);
	return 0;
}
