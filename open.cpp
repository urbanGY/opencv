#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <math.h>
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
float symmetry(cv::Mat, int *);// return degree of symmetry

int main(int argc, char* argv[]) {
	string name = "mole/test10.jpg";
	cv::Mat image = cv::imread("C:/Users/sfsfk/Desktop/" + name, cv::IMREAD_COLOR);//원본 이미지
	cv::Mat roug = image;
	cv::Mat original = cv::imread("C:/Users/sfsfk/Desktop/" + name, cv::IMREAD_COLOR);//원본 이미지
	cv::Mat black;
	cv::Mat rough;
	cv::imshow("img", image);
	int row = image.rows;//세로
	int col = image.cols;//가로
	int * index;
	int * index2;
	int resultA = -1, resultC = -1;
	int resultB = 0;
	int cloudy = 60;
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
	rough = masking(image, cloudy - 20, cloudy);	
	index2 = find(rough);
	resultB = boundary(index, index2)*100;//return result B 1이면 암 0이면 점

	fill(black);
	resultA = (int)symmetry(black, index);//return result A
	resultA = 100 - resultA;
	original = cutting(original, index);
	roug= cutting(roug, index2);
	cv::imshow("roug", roug);
	cv::imshow("original", original);
	resultC = (int)histogram(original);//return result C

	int total = (resultA + resultB + resultC) / 3;
	cout << "*********************" << endl;
	cout << "cloud : " << cloudy << endl;
	cout << "result A : " << resultA << "%" << endl;//100% 기준으로 대칭성
	cout << "result B : " << resultB << "%" << endl;//1이면 암
	cout << "result C : " << resultC << "%" << endl;//count/40*100
	cout << "total : " << total << "%" << endl;
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

int boundary(int * index1, int * index2) { // case 5 6 7 안됨
	int size1, size2;
	size1 = (index1[1] - index1[0])*(index1[3] - index1[2]);
	size2 = (index2[1] - index2[0])*(index2[3] - index2[2]);
	
	cout << "경계차이 비율 : " << (float)size2 / size1 * 100 << "\n";
	if (((float)size2 / size1 * 100) > 100 && ((float)size2 / size1 * 100) < 300) {
		cout << "경계선이 모호합니다" << "\n";
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

float symmetry(cv::Mat image, int * index) {//날것의 마스킹
	int row = image.rows;
	int col = image.cols;
	int read = -1;
	int read2 = -1;
	int count = 0;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			read = image.at<uchar>(i, j);
			if (read == 0) {
				count++;
			}
		} 
	}
	double temp = sqrt((float)count / 3.141592);
	int r = (int)temp;

	int x = (index[1] - index[0]) / 2 + index[0];
	int y = (index[3] - index[2]) / 2 + index[2];
	cv::Mat background(row, col, CV_8UC3, cv::Scalar(255, 255, 255));
	cv::circle(background, cv::Point(x,y), r+(count/500)+1, cv::Scalar(0,0,0),-1);
	cv::cvtColor(background, background, CV_BGR2GRAY);
	
	int sub = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			read = image.at<uchar>(i, j);
			read2 = background.at<uchar>(i, j);
			int temp = read - read2;
			if ((temp > 200) || (temp < -200)) {
				sub++;
			}
		}
	}
	cout << "count : " << count << endl;
	cout << "sub : " << sub << endl;
	cv::imshow("circle", background);
	sub = count - sub;
	/*cout << "count : " << count << endl;
	cout << "sub : " << sub << endl;
	cout << "answer : " << (float)sub / (float)count << endl;*/

	return ((float)sub / (float)count) * 100;
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
	/*double p1 = 0.000002069;
	double p2 = -0.0002921;
	double p3 = 0.01324;
	double p4 = -0.1994;
	double p5 = 2.271;
	double p6 = - 1.959;	*/
	double p1 = 0.000000577;
	double p2 = -0.00006435;
	double p3 = 0.00124;
	double p4 = 0.04832;
	double p5 = 0.7406;
	double p6 = -0.8064;
	double answer = p1 * pow(count, 5.0) + p2 * pow(count, 4.0) + p3 * pow(count, 3.0) + p4 * pow(count, 2.0) + p5 * pow(count, 1.0) + p6;
	if (answer >= 100) {
		answer = 100;
	}
	return answer;
}
