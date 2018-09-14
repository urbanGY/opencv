#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
using namespace std;

void diff(int * width, int col) {
	for (int i = 0; i < col-1; i++) {
		width[i] = width[i + 1] - width[i];
	}
	width[col - 1] = 0;
}

int main(int argc, char* argv[]) {
	cv::Mat image = cv::imread("C:/Users/sfsfk/Desktop/js.png", cv::IMREAD_COLOR);
	int row = image.rows;//470 ����
	int col = image.cols;//624 ����

	int width[624] = { 0 };
	float width_f[624] = { 0,0 };
	int red = 0;
	int green = 0;
	int blue = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			red = image.at<cv::Vec3b>(i, j)[2];
			green = image.at<cv::Vec3b>(i, j)[1];
			blue = image.at<cv::Vec3b>(i, j)[0];
			width[j] += (red + green + blue);
		}
	}
	
	
	for (int i = 0; i < col; i++) {
		width_f[i] = (float)width[i] / (3 * row);
		width[i] = (int)width_f[i];
	}

	diff(width, col);
	/*for (int i = 0; i < col; i++) {
		printf("%d, ", width[i]);
	}*/
	
	//���⼭ ���� ��� �Ǿ��ٰ� ġ��
	int long_under = 0, long_over = 0;
	int cur_under = 0, cur_over = 0;

	int start = 0, end = 0;
	int cur_start = 0, cur_end = 0;

	for (int i = 0; i < col; i++) {
		if (width[i] < 0) {//���� �����߼��� ��			
			cur_under++;
			if (cur_under == 1) {
				cur_start = i;
			}			
		}
		else {//�����߼��� ��
			if (cur_under > long_under) {//�ֱ� ���� ���̰� ���� ��ٸ�
				start = cur_start;
				long_under = cur_under;
			}
			cur_under = 0;
			cur_start = 0;
		}
		//printf("%d, ",start);
	}	
	cout << start << endl;
	return 0;
}