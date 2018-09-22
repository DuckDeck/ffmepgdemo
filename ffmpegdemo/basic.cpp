#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
using namespace std;

//�������еĺ������Խ�YUV420P�����е�Y��U��V�����������뿪��������������ļ�
//�����Ƶ֡�Ŀ�͸߷ֱ�Ϊw��h����ôһ֡YUV420P��������һ��ռ��w*h*3/2 Byte�����ݡ�����ǰw*h Byte�洢Y��
//���ŵ�w*h*1/4 Byte�洢U�����w*h*1/4 Byte�洢V���������ú����Ĵ������к�
//�����һ�ŷֱ���Ϊ256x256������Ϊlena_256x256_yuv420p.
//yuv��YUV420P��ʽ�����������ļ������Ϊ�����ļ�
//output_420_y.y����Y���ݣ��ֱ���Ϊ256x256��
//output_420_u.y����U���ݣ��ֱ���Ϊ128x128��
//output_420_v.y����V���ݣ��ֱ���Ϊ128x128��
int yuv420_split(const char* url,int w,int h,int num){
    FILE *fp = fopen(url,"rb+");
    FILE *fp1 = fopen("asset/output_420_y.y","wb+");
    FILE *fp2 = fopen("asset/output_420_u.y","wb+");
    FILE *fp3 = fopen("asset/output_420_v.y","wb+");
    unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);
    for(int i=0;i<num;i++){
        size_t count = fread(pic, 1, w * h * 3 / 2, fp);
        cout << "count " << count << endl;
        fwrite(pic, 1, w * h, fp1);
        fwrite(pic + w * h, 1, w * h / 4, fp2);
        fwrite(pic + w * h * 5 / 4, 1, w * h / 4, fp3);
    }
    //YUV��������������ģ�y�����Ǳ����ڳ�*���ʣ�µĸ�1/4����ʣ�µ���������������ȡ������ʱ����ȡw * h��ȡu������ʱ����ȡʣ�µ�1/4�����Ǳ����ʱ��
    //�޴�picָ���ʼ�ĵط�����w*H�ķ�������Ҳ�պ���y�ķ�����Ȼ�󱣴�u����ʱ��Ҫ��pic + w * h�ĵط���ʼд��д������w * h / 4
    //�����һ��Ҫ��pic + w * h + w * h / 4�ڵط���ʼд��Ҳ����pic + w * h * 5 / 4��
    free(pic);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    return 0;
}
//(2)����YUV444P���������е�Y��U��V����
//�Ӵ�����Կ����������Ƶ֡�Ŀ�͸߷ֱ�Ϊw��h����ôһ֡YUV444P��������һ��ռ��w*h*3 Byte�����ݡ�����ǰw*h Byte�洢Y��
//���ŵ�w*h Byte�洢U�����w*h Byte�洢V���������ú����Ĵ������к󣬽����һ�ŷֱ���Ϊ256x256������Ϊlena_256x256_yuv444p.
//yuv��YUV444P��ʽ�����������ļ������Ϊ�����ļ���
int yuv444_split(char *url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("Media/material/output_444_y.y", "wb+");
	FILE *fp2 = fopen("Media/material/output_444_u.y", "wb+");
	FILE *fp3 = fopen("Media/material/output_444_v.y", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3);
	for (int i = 0; i<num; i++) {
		fread(pic, 1, w * h * 3, fp);
		fwrite(pic, 1, w * h, fp1);
		fwrite(pic + w * h, 1, w * h, fp2);
		fwrite(pic + w * h * 2, 1, w * h, fp3);
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}
//(3) ��YUV420P��������ȥ����ɫ����ɻҶ�ͼ��
//������YUV��ʽ�������ݱ�ɻҶ�ͼ��ֻ��Ҫ��U��V�������ó�128���ɡ�������ΪU��V��ͼ���еľ���ƫ�ô����ɫ�ȷ�����ɫ�ȷ�����ƫ�ô���ǰ��ȡֵ��Χ��-128��127��
//��ʱ�����ɫ��Ӧ���ǡ�0��ֵ������ƫ�ú�ɫ�ȷ���ȡֵ�����0��255�������ʱ����ɫ��Ӧ�ľ���128�ˡ��������ú����Ĵ������к�
//�����һ�ŷֱ���Ϊ256x256������Ϊlena_256x256_yuv420p.yuv��YUV420P��ʽ�����������ļ����������Ϊoutput_gray.yuv��YUV420P��ʽ�����������ļ�
int yuv420_gray(char *url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("Media/material/output_yuv420_gray.yuv", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);
	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3 / 2, fp);
		memset(pic + w * h, 128, w * h / 2);//��pic + w * h�����ã������ȫ��uv�����������ú�������w * h / 2
		fwrite(pic, 1, w * h * 3 / 2, fp1);
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
	return 0;
}

//(4)��YUV420P�������ݵ����ȼ���
//������㽫ͼ������ȼ��룬ֻҪ��ͼ���ÿ�����ص�Yֵȡ�����ֱ���г���2�Ĺ����Ϳ����ˡ�ͼ���ÿ��Yֵռ��1 Byte��ȡֵ��Χ��0��255��
//��ӦC�����е�unsigned char�������͡��������ú����Ĵ������к󣬽����һ�ŷֱ���Ϊ256x256������Ϊlena_256x256_yuv420p.yuv��YUV420P��ʽ�����������ļ����������Ϊoutput_half.yuv��YUV420P��ʽ�����������ļ�
int yuv420_harf_luminance(char *url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("Media/material/output_yuv420_harf_luminance.yuv", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);
	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3 / 2, fp);
		for (int j = 0; j < w * h; j++)
		{
			unsigned char temp = pic[j] / 2;
			pic[j] = temp;
		}
		fwrite(pic, 1, w * h * 3 / 2, fp1);
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
	return 0;
}
//(5)��YUV420P�������ݵ���Χ���ϱ߿�
//ͼ��ı߿�Ŀ��Ϊborder�������򽫾���ͼ���Եborder��Χ�ڵ����ص����ȷ���Y��ȡֵ���ó����������ֵ255���������ú����Ĵ������к�
//�����һ�ŷֱ���Ϊ256x256������Ϊlena_256x256_yuv420p.yuv��YUV420P��ʽ�����������ļ����������Ϊoutput_border.yuv��YUV420P��ʽ
//�����������ļ�
int yuv420_add_Border(char *url, int w, int h, int border, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("Media/material/output_yuv420_add_border.yuv", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2);
	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3 / 2, fp);
		for (int j = 0; j < h; j++) {
			for (int k = 0; k < w; k++) {
				if (k < border || k >(w - border) || j < border || j >(h - border)) {
					pic[j * w + k] = 255; //ֻ����0����255��������������������Ϊ�ⲻ��RGB����Ϊ�ǲ������ȣ�����ֱ�Ӳ�����һ��w*h����
				}
			}
		}
		fwrite(pic, 1, w * h * 3 / 2, fp1);
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
	return 0;
}

//(6) ����YUV420P��ʽ�Ļҽײ���ͼ
//������һ����ͨ���ҽײ���ͼ��������Сֵymin���������ֵymax���ҽ�����barnumȷ��ÿһ���Ҷ��������ص����ȷ���Y��ȡֵ��
//��һ���滹Ҫ����ͼ��Ŀ��width��ͼ��ĸ߶�height�Լ��ҽ�����barnumȷ��ÿһ���Ҷ����Ŀ�ȡ���������������Ϣ֮�󣬾Ϳ���������Ӧ��ͼƬ�ˡ�
//�������ú����Ĵ������к󣬻�����һ��ȡֵ��Χ��ymin-ymax��һ������barnum���Ҷ�����YUV420P��ʽ�Ĳ���ͼ��
//ymax������ߵ����ȣ�ymin�����ұߵ�����
int yuv420_graybar(int width, int height, int ymin, int ymax, int barnum, char *url_output)
{
	float barwidth; //ÿ��bar�Ŀ��
	float lum_inc;  //ÿ��bar�߶ȵĲ�ֵ
	unsigned char lum_temp;
	int uv_width, uv_height;  //uv�����ĸ߶ȿ��
	FILE *fp = NULL;
	unsigned char *data_y = NULL;     //y��������
	unsigned char *data_u = NULL;     //u��������
	unsigned char *data_v = NULL;     //v��������
	int t = 0, i = 0, j = 0;

	barwidth = (float)width / (float)barnum;
	lum_inc = (float)(ymax - ymin) / (float)(barnum - 1);
	uv_width = width / 2;
	uv_height = height / 2;

	data_y = (unsigned char *)malloc(width * height);
	data_u = (unsigned char *)malloc(uv_width * uv_height);
	data_v = (unsigned char *)malloc(uv_width * uv_height);
	if ((fp = fopen(url_output, "wb+")) == NULL) {
		printf("Error: Cannot create file!");
		return -1;
	}
	printf("Y, U, V value from picture's left to right:\n");
	for (t = 0; t < width / barwidth; t++)
	{
		lum_temp = ymin + (char)(t * lum_inc);
		printf("%3d,128,128\n", lum_temp);
	}
	for (j = 0; j<height; j++) {
		for (i = 0; i<width; i++) {
			t = i / barwidth;
			lum_temp = ymin + (char)(t * lum_inc);
			data_y[j * width + i] = lum_temp; //���y����������Ϊw*h
		}
	}
	for (j = 0; j<uv_height; j++) {
		for (i = 0; i<uv_width; i++) {
			data_u[j * uv_width + i] = 128; //���u����������Ϊuv_width��uv_height
		}
	}
	for (j = 0; j < uv_height; j++) {
		for (i = 0; i < uv_width; i++) {
			data_v[j * uv_width + i] = 128; //���v����������Ϊuv_width��uv_height
		}
	}
	fwrite(data_y, width * height, 1, fp);
	fwrite(data_u, uv_width * uv_height, 1, fp);
	fwrite(data_v, uv_width * uv_height, 1, fp);
	fclose(fp);
	free(data_y);
	free(data_u);
	free(data_v);
	return 0;
}

//(7)��������YUV420P�������ݵ�PSNR
//����8bit����������������˵��PSNR�ļ��㹫ʽ������ʾ��
//������ʽ��mse�ļ��㹫ʽ������ʾ������M��N�ֱ�Ϊͼ��Ŀ�ߣ�xij��yij�ֱ�Ϊ����ͼ���ÿһ������ֵ��PSNRͨ�������������ۣ�
//���Ǽ�������ͼ����ԭʼͼ��֮��Ĳ���Դ�����������ͼ���������
//��������õ���PSNRȡֵΪ26.693��PSNRȡֵͨ������¶���20-50�ķ�Χ�ڣ�ȡֵԽ�ߣ���������ͼ��Խ�ӽ�����ӳ������ͼ������Խ��
double yuv420_psnr(char *url1, char *url2, int w, int h, int num) {
	FILE *fp1 = fopen(url1, "rb+");
	FILE *fp2 = fopen(url2, "rb+");
	unsigned char *pic1 = (unsigned char *)malloc(w * h);
	unsigned char *pic2 = (unsigned char *)malloc(w * h);
	double p = 0.0;
	for (int i = 0; i < num; i++) {
		fread(pic1, 1, w * h, fp1);
		fread(pic2, 1, w * h, fp2);
		double mse_sum = 0, mse = 0, psnr = 0;
		for (int j = 0; j < w * h; j++) {
			mse_sum += pow((double)(pic1[j] - pic2[j]), 2);
		}
		mse = mse_sum / (w * h);
		psnr = 10 * log10(255.0 * 255.0 / mse);
		printf("%5.3f\n", psnr);
		fseek(fp1, w * h / 2, SEEK_CUR);
		fseek(fp2, w * h / 2, SEEK_CUR);
		p = psnr;
	}
	free(pic1);
	free(pic2);
	fclose(fp1);
	fclose(fp2);
	return p;
}

//(8) ����RGB24���������е�R��G��B����
int rgb24_split(char* url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("Media/material/output_r.y", "wb+");
	FILE *fp2 = fopen("Media/material/output_g.y", "wb+");
	FILE *fp3 = fopen("Media/material/output_b.y", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3);
	for (int i = 0; i < num; i++) {
		fread(pic, 1, w * h * 3, fp);
		for (int j = 0; j < w*h * 3; j += 3) {
			fwrite(pic + j, 1, 1, fp1);
			fwrite(pic + j + 1, 1, 1, fp2);
			fwrite(pic + j + 2, 1, 1, fp3);
		}
	}
	free(pic);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}
