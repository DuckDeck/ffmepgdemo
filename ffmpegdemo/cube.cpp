#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;

//#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"

using namespace std;

int thresh = 50, N = 11;
const char* wndname = "cude demo";

static double angle(Point pt1, Point pt2, Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1 * dy2) / sqrt((dx1*dx1 + dy1 * dy1)*(dx2*dx2 + dy2 * dy2) + 1e-10);
}

static void findSquares(const Mat& image, vector<vector<Point>>& squares) {
	squares.clear();
	Mat pyr, timg, gray0(image.size(), CV_8U), gray;
	//��Сͼ�񲢷Ŵ�ͼ�����˳����
	pyrDown(image, pyr, Size(image.cols / 2, image.rows / 2));
	pyrUp(pyr, timg, image.size());
	vector<vector<Point> > contours;
	// find squares in every color plane of the image
		//��ͼ���ÿ����ɫƽ�����ҵ�������
	for (int c = 0; c < 3; c++)
	{
		int ch[] = { c, 0 };
		mixChannels(&timg, 1, &gray0, 1, ch, 1);
		// try several threshold levels
		for (int l = 0; l < N; l++)
		{
			// hack: use Canny instead of zero threshold level.
			// Canny helps to catch squares with gradient shading
			// hack��ʹ��Canny����������ֵ����
			// Canny�����ڲ�׽������ɫ�ķ���
			if (l == 0) {
				//Ӧ��Canny�� �ӻ����ȡ������ֵ
				//����lower����Ϊ0��ǿ�Ʊߺϲ���
				Canny(gray0, gray, 0, thresh, 5);
				//����canny�õ�����������������Ե��֮���Ǳ�ڿ׶�
				dilate(gray, gray, Mat(), Point(-1, -1));
			}
			else {
				gray = gray0 >= (l + 1) * 255 / N;
			}
			//�ҵ�������������ȫ���洢Ϊ�б�
			findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
			vector<Point> approx;
			for (size_t i = 0; i < contours.size(); i++)
			{
				//���������������������ܳ�������
				approxPolyDP(contours[i], approx, arcLength(contours[i], true)*0.02, true);
				//���������ڽ��ƺ�Ӧ����4������
				//��Խϴ�������˳��������ӵ㣩
				//������͹��
				//ע�⣺ʹ������ľ���ֵ����Ϊ
				//�������������Ҳ�����Ǹ��� - �������������ж�
				if (approx.size() == 4 &&
					fabs(contourArea(approx)) > 1000 &&
					isContourConvex(approx)) {
					double maxCosine = 0;

					for (int j = 2; j < 5; j++)
					{
						// find the maximum cosine of the angle between joint edges
												//�ҵ����ӱ�Ե֮��Ƕȵ��������ֵ 
						double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
						maxCosine = MAX(maxCosine, cosine);
					}
					if (maxCosine < 0.3)
						squares.push_back(approx);
				}
			}
		}
	}
}

static void drawSquares(Mat& image, const vector<vector<Point> >& squares)
{
	for (size_t i = 0; i < squares.size(); i++)
	{
		const Point* p = &squares[i][0];
		int n = (int)squares[i].size();
		polylines(image, &p, &n, 1, true, Scalar(0, 255, 0), 3, LINE_AA);
	}

	imshow(wndname, image);
}

int cube() {
	vector<vector<Point> > squares;
	Mat img = imread("asset/cube.jpg",IMREAD_COLOR);
	if (img.empty()) {
		cout << "Couldn't load cube.jpg " << endl;
		return 0;
	}
	findSquares(img, squares);
	drawSquares(img, squares);
	int c = waitKey();
	if (c == 27)
		return 0;
}