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
	cv::Mat image = cv::imread("C:/Users/sfsfk/Desktop/case10.jpg", cv::IMREAD_COLOR);
	cv::Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));//마스킹
	//cv::dilate(image, image, /*cv::Mat(3, 3, CV_8U, cv::Scalar(1))*/mask, cv::Point(-1, -1), 2);//팽창연산
	//cv::Mat image = cv::imread("C:/Users/sfsfk/Desktop/js.png", cv::IMREAD_COLOR);
	int row = image.rows;//470 세로 427
	int col = image.cols;//624 가로 398
	/*흑백 전환*/
	cv::Mat black;
	cv::cvtColor(image, black, CV_BGR2YCrCb);
	/*default 0, 135, 80   255, 171, 124*/
	/* test1 ->  0, 110, 80 앞의 값을 내릴 수 록 더 많은 영역이 하얀색에 포함된다(이 범위 안으로 들어온다) / 255, 171, 128(값을 올릴 수 록 하얀색이 넓어짐
	팽창 연산(노이즈 캔슬링) 3회 진행
	식별 실패 : case3(엷은 갈색이 넓게 분포), case4, case5(코와 더불어 코털 등이 식별에 방해됨, 점이 너무 작아서 노이즈 캔슬링에 치명적), case7,
	*/
	cv::inRange(black, cv::Scalar(0, 110, 80), cv::Scalar(255, 171, 128), black);	
	cv::dilate(black, black, /*cv::Mat(3, 3, CV_8U, cv::Scalar(1))*/mask, cv::Point(-1, -1), 3);//팽창연산
	/*자르기*/
	int left = 0, right = 0, top = 0, bottom = 0;
	int read = -1;	
	bool flag = false;
	for (int i = 0; i < row; i++) {
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
	for (int i = row-1; i >= 0; i--) {
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
	for (int i = 0; i < col; i++) {
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
	for (int i = col-1; i >= 0; i--) {
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
	
	cout << "left" << left << "right" << right << endl;
	cout << "top" << top << "bottom" << bottom << endl;
	cv::Mat capture = image;
	if (!(left == 0 || right == 0 || top == 0 || bottom == 0))
		capture = image(cv::Range(top, bottom), cv::Range(left, right));
		
	/*배경색 걷어내기*/
	/*cv::Mat capture_black = black(cv::Range(top, bottom), cv::Range(left, right));

	int cap_row = capture.rows;
	int cap_col = capture.cols;
	for (int i = 0; i < cap_row; i++) {
		for (int j = 0; j < cap_col; j++) {
			read = capture_black.at<uchar>(i, j);
			if (read == 255) {
				capture.at<cv::Vec3b>(i, j)[0] = 255;
				capture.at<cv::Vec3b>(i, j)[1] = 255;
				capture.at<cv::Vec3b>(i, j)[2] = 255;
			}
		}
	}*/
	/*배경색 걷어내기*/
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
	
	//cv::imshow("draw capture_black", capture_black);
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
	 cv::imshow("slice image", capture);
	cv::imshow("cvt ycrcb", black);
	cv::imshow("original image", image);	
	cv::waitKey(0);
	return 0;
}
