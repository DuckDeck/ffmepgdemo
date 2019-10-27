#include <opencv2/opencv.hpp>
#include "stdafx.h"
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

//���Ҫ��������ɾ�ַ������������ֿ�����ʽ��Ҫ�ñ����ַ����ķ�ʽ
Mat current;
void practise1() {
	current = Mat::zeros(500, 500, CV_8UC3);
	imshow("dstImage", current);
	int left = 10;
	int top = 20;
	 while (true)
	 {
		 int s = waitKey(0);
		 if (s == 13) {
			 top += 25;
			 left = 10;
			 continue;
		 }
		 if (s == 8) {
			 if (left <= 10 && top <= 20){
				 continue;
			 }
			 else if (left <= 10 && top >= 20) {
				 left = 475;
				 top = top - 25;
			 }
			
			 continue;
		 }
		 const char c = (char)s;
		 cout << c << endl;
		 putText(current, string(1,c), Point(left, top), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255));
		 imshow("dstImage", current);
		 left += 15;
		 if (left >= 485) {
			 top += 25;
			 left = 10;
		 }
		 if (left >= 485 && top >= 490) {
			 break;
		 }
	 }
}

string content ;

//���ַ�������Ҫ���кܲ��Ѻã�����Ҫ��string����
void practice1() {
	
	int numOfLines = 32;

	int top = 20;
	while (true)
	{
		current = Mat::zeros(500, 500, CV_8UC3);
		int s = waitKey(0);
		if (s == 13) { //�����ַ��������鷳��,�������Ūһ��string����
			top += 25;
			
			continue;
		}
		
		const char c = (char)s;
		content += c;
		//�����ж������ݻ�������
		int length = content.length();
		string tmp = content.substr(0, numOfLines);
		int i = 1;
		while (tmp.length() == numOfLines)
		{
			
			putText(current, tmp, Point(5, top), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255));
			imshow("dstImage", current);
			
			tmp = content.substr(i * numOfLines, numOfLines);
			i += 1;
			top = top +  25;
		}
		putText(current, tmp, Point(5, top), FONT_HERSHEY_SIMPLEX, 1, Scalar::all(255));
		imshow("dstImage", current);
		
		if (top >= 490) {
			break;
		}
		top = 20; //Ҫ��λһ��
	}
	
}



int learnOpenCV4()
{
	practice1();
	
	waitKey(0);
	return 0;
}

