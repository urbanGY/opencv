#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;

cv::Mat masking(cv::Mat, float, float);//return masking image
void fill(cv::Mat);
int * find(cv::Mat);//return left,right,top,bottom
cv::Mat cutting(cv::Mat, int *);//return cuting image
void histogram(cv::Mat);

int main(int argc, char* argv[]) {
	string name = "js.png";
	cv::Mat image = cv::imread("C:/Users/sfsfk/Desktop/" + name, cv::IMREAD_COLOR);//���� �̹���
	cv::Mat original = cv::imread("C:/Users/sfsfk/Desktop/" + name, cv::IMREAD_COLOR);//���� �̹���
	int row = image.rows;//����
	int col = image.cols;//����

	cv::Mat black = masking(image, 0.3, 1.3);//��� �̹���
	cv::Mat rough = masking(image, 0.2, 1.5);
	int * index = find(black);//������ ������
	int * index_rough = find(rough);//�����ϰ� ������

	cout << "*** case B ***" << endl;
	cout << "left : " << index[0] - index_rough[0] << ", right : " << index_rough[1] - index[1] << endl;
	cout << "top : " << index[2] - index_rough[2] << ", bottom : " << index_rough[3] - index[3] << endl;


	cv::Mat capture = cutting(image, index);//�߸� �̹���
	fill(rough);

	//vector<vector<cv::Point>> contours;
	//cv::findContours(rough, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	//int cmin = 100;  // �ּ� �ܰ��� ����
	//int cmax = 1000; // �ִ� �ܰ��� ����
	//vector<vector<cv::Point>>::const_iterator itc = contours.begin();
	//while (itc != contours.end()) {
	//	if (itc->size() < cmin || itc->size() > cmax)
	//		itc = contours.erase(itc);
	//	else
	//		++itc;
	//}
	//cv::drawContours(original, contours, -1, cv::Scalar(255, 255, 255), 2);

	histogram(capture);//���� �Ǵ�
	cv::imshow("original", image);
	cv::imshow("masking", black);
	cv::imshow("rough", rough);
	cv::imshow("slice", capture);

	cv::waitKey(0);
	return 0;
}

cv::Mat masking(cv::Mat image, float low, float high) {
	int row = image.rows;
	int col = image.cols;
	int row_start = (row / 2) - 15;
	int col_start = (col / 2) - 15;
	int red = 0, green = 0, blue = 0;

	for (int i = 0; i < 31; i++) { //�߰��� �ֺ� 25 �ȼ��� rgb���� ��� ���
		for (int j = 0; j < 31; j++) {
			red += image.at<cv::Vec3b>(i + row_start, j + col_start)[2];
			green += image.at<cv::Vec3b>(i + row_start, j + col_start)[1];
			blue += image.at<cv::Vec3b>(i + row_start, j + col_start)[0];
		}
	}
	red /= 961;
	green /= 961;
	blue /= 961;

	cout << "rgb : " << red * low << ", " << green * low << ", " << blue * low << endl;
	cout << "rgb : " << red * high << ", " << green * high << ", " << blue * high << endl;

	cv::Mat black;
	cv::inRange(image, cv::Scalar((blue*low), (green*low), (red*low)), cv::Scalar((blue*high), (green*high), (red*high)), black);//��� rgb�� ���Ѱ��� ���Ѱ� ���� ����ŷ
	cv::Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));//���� ����� ����ŷ
	cv::erode(black, black, /*cv::Mat(3, 3, CV_8U, cv::Scalar(1))*/mask, cv::Point(-1, -1), 1);//���꿬�� ����(������ ĵ����)

	return black;//������� ����ŷ�� �̹��� ��ȯ
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
			if (read == 255) {
				end = x;
				break;
			}
			x--;
		}
		x = 0;
		while (x < col) {			
			read = black.at<uchar>(i, x);
			if (read == 255) {
				start = x;
				break;
			}
			x++;
		}
		for(int j = start ; j < end ; j++)
			black.at<uchar>(i, j) = 255;
		start = 0;
		end = 0;
	}

	for (int i = 0; i < col; i++) {
		int x = row - 1;
		while (x >= 0) {
			read = black.at<uchar>(x, i);
			if (read == 255) {
				end = x;
				break;
			}
			x--;
		}
		x = 0;
		while (x < row) {
			read = black.at<uchar>(x, i);
			if (read == 255) {
				start = x;
				break;
			}
			x++;
		}
		for (int j = start; j < end; j++)
			black.at<uchar>(j, i) = 255;
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
			if (read == 255) {
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
			if (read == 255) {
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
			if (read == 255) {
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
			if (read == 255) {
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
	cout << "left" << left << "right" << right << endl;
	cout << "top" << top << "bottom" << bottom << endl;
	int * index = (int *)malloc(sizeof(int) * 4);
	index[0] = left;
	index[1] = right;
	index[2] = top;
	index[3] = bottom;
	return index;
}
cv::Mat cutting(cv::Mat image, int * index) {//����ŷ�� �̹����� ������� ���� �̹����� �ڸ�
	int left = index[0];
	int right = index[1];
	int top = index[2];
	int bottom = index[3];

	cv::Mat capture = image;
	if (!(left == 0 || right == 0 || top == 0 || bottom == 0))//���� ó��
		capture = image(cv::Range(top, bottom), cv::Range(left, right));

	return capture;//�߸� �̹��� ����
}

void histogram(cv::Mat capture) {
	cv::Mat dst;
	cv::Mat bgr[3];
	cv::Mat hist; //Histogram ��갪 ����
	int channel[] = { 0,1,2 };
	int histSize = 255; //Histogram ���ΰ��� ��
	int count = 0;
	float range[] = { 0,255.0 };
	const float * ranges = range;
	int hist_w = 512; int hist_h = 400;
	int number_bins = 255;
	int bin_w = cvRound((double)hist_w / number_bins);
	unsigned row2 = capture.rows; unsigned col2 = capture.cols; //�ڸ� ������ ũ�� ����

	cvtColor(capture, dst, CV_HSV2BGR); //Color ����
	calcHist(&dst, 3, channel, cv::Mat(), hist, 1, &histSize, &ranges, true, false); //Histogram ���
	cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
	normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

	for (int i = 1; i < number_bins; i++) {	//Histogram �� �׸���
		line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))), cv::Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))), cv::Scalar(0, 255, 0), 2, 8, 0);
	}

	for (int i = 0; i < histSize; i++) { //���� �پ缺 ����
		//printf("%d��° %f \n", i, hist.at<float>(i));
		if (hist.at<float>(i) > 229) {
			count++;
		}
	}

	printf("ī��Ʈ �� : %d\n", count);

	if (count > 10) {
		printf("�پ��� ������ ���Դϴ�.");
	}
	else {
		printf("�پ��� ������ ������ �ʽ��ϴ�.");
	}

	cv::namedWindow("Histogram", CV_WINDOW_AUTOSIZE);
	cv::imshow("HSV2BGR", dst);
	cv::imshow("Histogram", histImage);
}