#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;

cv::Mat masking(cv::Mat, int, int);//return masking image
int boundary(int *, int *);
void fill(cv::Mat);
int * find(cv::Mat);//return left,right,top,bottom
cv::Mat cutting(cv::Mat, int *);//return cuting image
float histogram(cv::Mat);
float symmetry(cv::Mat);// return degree of symmetry

int main(int argc, char* argv[]) {
	string name = "mole/test6.jpg";
	cv::Mat image = cv::imread("C:/Users/sfsfk/Desktop/" + name, cv::IMREAD_COLOR);//원본 이미지
	cv::Mat original = cv::imread("C:/Users/sfsfk/Desktop/" + name, cv::IMREAD_COLOR);//원본 이미지
	cv::Mat black;
	cv::Mat rough;
	cv::imshow("img", image);
	int row = image.rows;//세로
	int col = image.cols;//가로
	int * index;
	int * index2;
	int resultA = -1, resultB = -1, resultC = -1;
	int cloudy = 30;
	while (true) {
		black = masking(image, cloudy, cloudy);
		index = find(black);
		int sum = 0;
		for (int i = 0; i < 4; i++)
			sum += index[i];
		if (sum == 0) {//못잡았다는 뜻
			cloudy -= 10;
		}
		else {
			break;
		}
	}	
	rough = masking(image, cloudy - 5, cloudy);	
	index2 = find(rough);

	resultB = boundary(index, index2)*100;//return result B 1이면 암 0이면 점

	cv::Mat capture = cutting(black, index);
	fill(capture);
	resultA = (int)symmetry(capture);//return result A

	original = cutting(original, index);
	resultC = (int)histogram(original);//return result C
	cout << "*********************" << endl;
	cout << "cloud : " << cloudy << endl;
	cout << "result A : " << resultA << "%" << endl;//100% 기준으로 대칭성
	cout << "result B : " << resultB << "%" << endl;//1이면 암
	cout << "result C : " << resultC << "%" << endl;//count/40*100
	cout << "*********************" << endl;
	cv::waitKey(0);
	return 0;
}


cv::Mat masking(cv::Mat image, int degree, int cloudy) {//60 50
	cv::Mat black;
	cv::Mat gray_image;
	medianBlur(image, image, 7);
	cv::cvtColor(image, gray_image, CV_BGR2GRAY); // 흑백영상으로 변환
	cv::adaptiveThreshold(gray_image, black, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 401, degree);
	if (degree == cloudy) {
		cv::imshow("Tight masking", black);
	}
	else {
		cv::imshow("Rough masking", black);
	}
	
	return black;//흑백으로 마스킹된 이미지 반환
}

int boundary(int * index1, int * index2) {
	int size1, size2;
	size1 = (index1[1] - index1[0])*(index1[3] - index1[2]);
	size2 = (index2[1] - index2[0])*(index2[3] - index2[2]);
	
	//cout << "경계차이 비율 : " << (float)size2 / size1 * 100 << "\n";
	if (((float)size2 / size1 * 100) > 100 && ((float)size2 / size1 * 100) < 200) {
		//cout << "경계선이 모호합니다" << "\n";
		return 1;
	}
	return 0;
}

void fill(cv::Mat black) {
	int row = black.rows;
	int col = black.cols;
	int read = -1;	
	int start = 0;
	int end = 0;

	for (int i = 0; i < row; i++) {		
		int x = col - 1;
		while (x >= 0) {
			read = black.at<uchar>(i, x);
			if (read == 0) {
				end = x;
				break;
			}
			x--;
		}
		x = 0;
		while (x < col) {			
			read = black.at<uchar>(i, x);
			if (read == 0) {
				start = x;
				break;
			}
			x++;
		}
		for(int j = start ; j < end ; j++)
			black.at<uchar>(i, j) = 0;
		start = 0;
		end = 0;
	}

	for (int i = 0; i < col; i++) {
		int x = row - 1;
		while (x >= 0) {
			read = black.at<uchar>(x, i);
			if (read == 0) {
				end = x;
				break;
			}
			x--;
		}
		x = 0;
		while (x < row) {
			read = black.at<uchar>(x, i);
			if (read == 0) {
				start = x;
				break;
			}
			x++;
		}
		for (int j = start; j < end; j++)
			black.at<uchar>(j, i) = 0;
		start = 0;
		end = 0;
	}
}

int * find(cv::Mat black) {
	int row = black.rows;
	int col = black.cols;
	int left = 0, right = 0, top = 0, bottom = 0;
	int read = -1;
	bool flag = false;
	for (int i = 0; i < row; i++) {//find top
		for (int j = 0; j < col; j++) {
			read = black.at<uchar>(i, j);
			if (read == 0) {
				top = i;
				read = -1;
				flag = true;
				break;
			}
		}
		if (flag) {
			flag = false;
			break;
		}
	}
	for (int i = row - 1; i >= 0; i--) {//find bottom
		for (int j = 0; j < col; j++) {
			read = black.at<uchar>(i, j);
			if (read == 0) {
				bottom = i;
				read = -1;
				flag = true;
				break;
			}
		}
		if (flag) {
			flag = false;
			break;
		}
	}
	for (int i = 0; i < col; i++) {//find left
		for (int j = 0; j < row; j++) {
			read = black.at<uchar>(j, i);
			if (read == 0) {
				left = i;
				read = -1;
				flag = true;
				break;
			}
		}
		if (flag) {
			flag = false;
			break;
		}
	}
	for (int i = col - 1; i >= 0; i--) {//find right
		for (int j = 0; j < row; j++) {
			read = black.at<uchar>(j, i);
			if (read == 0) {
				right = i;
				read = -1;
				flag = true;
				break;
			}
		}
		if (flag) {
			flag = false;
			break;
		}
	}
	//cout << "left" << left << "right" << right << endl;
	//cout << "top" << top << "bottom" << bottom << endl;
	int * index = (int *)malloc(sizeof(int) * 4);
	index[0] = left;
	index[1] = right;
	index[2] = top;
	index[3] = bottom;
	return index;
}//884281
cv::Mat cutting(cv::Mat image, int * index) {//마스킹된 이미지를 기반으로 원본 이미지를 자름
	int left = index[0];
	int right = index[1];
	int top = index[2];
	int bottom = index[3];

	cv::Mat capture = image;
	if (!(left == 0 || right == 0 || top == 0 || bottom == 0))//예외 처리
		capture = image(cv::Range(top, bottom), cv::Range(left, right));

	return capture;//잘린 이미지 리턴
}

float symmetry(cv::Mat image) {
	int row = image.rows;
	int col = image.cols;
	int end_top = 0, start_bottom = row / 2;
	
	if (row % 2 == 0) // even
		end_top = row / 2;
	else //odd
		end_top = (row / 2) + 1; // row가 짝수든 홀수든 균등하게 보정
	
	cv::Mat top = image(cv::Range(0, end_top), cv::Range(0, col));
	cv::Mat bottom = image(cv::Range(start_bottom, row), cv::Range(0, col));
	cv::flip(bottom, bottom, -1);
	int count = 0;
	int read_top = -1, read_bottom = -1;
	for (int i = 0; i < end_top; i++) {
		for (int j = 0; j < col; j++) {
			read_top = top.at<uchar>(i, j);
			read_bottom = bottom.at<uchar>(i, j);
			if (read_top != read_bottom) {
				count++;
			}
		}
	}
	cv::imshow("top", top);
	cv::imshow("bottom", bottom);
	float matrix = end_top * col;
	return (float)((matrix - count) / matrix)*100;
}

float histogram(cv::Mat capture) { // 30정도
	cv::Mat dst;
	cv::Mat bgr[3];
	cv::Mat hist; //Histogram 계산값 저장
	int channel[] = { 0,1,2 };
	int histSize = 255; //Histogram 가로값의 수
	int count = 0;
	float range[] = { 0,255.0 };
	const float * ranges = range;
	int hist_w = 512; int hist_h = 400;
	int number_bins = 255;
	int bin_w = cvRound((double)hist_w / number_bins);
	unsigned row2 = capture.rows; unsigned col2 = capture.cols; //자른 사진의 크기 저장

	cvtColor(capture, dst, CV_HSV2BGR); //Color 변경
	calcHist(&dst, 3, channel, cv::Mat(), hist, 1, &histSize, &ranges, true, false); //Histogram 계산
	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
	normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

	for (int i = 1; i < number_bins; i++) {	//Histogram 선 그리기
		line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))), cv::Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))), cv::Scalar(0, 255, 0), 2, 8, 0);
	}

	for (int i = 0; i < histSize; i++) { //색의 다양성 검출
		//printf("%d번째 %f \n", i, hist.at<float>(i));
		if (hist.at<float>(i) > 229) {
			count++;
		}
	}

	printf("카운트 수 : %d\n", count);

	if (count > 30) {
		//printf("다양한 색조를 보입니다.\n");
	}
	else {
		//printf("다양한 색조를 보이지 않습니다.\n");
	}

	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("HSV2BGR", dst);
	cv::imshow("Histogram", histImage);
	return ((float)count/40.0)*100;
}
