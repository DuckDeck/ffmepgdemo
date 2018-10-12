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
    unsigned char *pic = (unsigned char *)malloc(w * h * 3 / 2); //pic �Ǹ�ָ��
    for(int i=0;i<num;i++){
        size_t count = fread(pic, 1, w * h * 3 / 2, fp);
        cout << "count " << count << endl;
		cout << "count " << pic << endl;
        fwrite(pic, 1, w * h, fp1);								//��pic��ʼ��ȡ w * h��������д���ļ�
        fwrite(pic + w * h, 1, w * h / 4, fp2);					//��pic+w*h��ʼ��ȡ w * h * 0.25��������д���ļ�
        fwrite(pic + w * h * 5 / 4, 1, w * h / 4, fp3);			//��pic+w*h+0.25*w*h��ʼ��ȡ w * h * 0.25��������д���ļ�
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
	FILE *fp1 = fopen("asset/output_444_y.y", "wb+");
	FILE *fp2 = fopen("asset/output_444_u.y", "wb+");
	FILE *fp3 = fopen("asset/output_444_v.y", "wb+");
	unsigned char *pic = (unsigned char *)malloc(w * h * 3);
	for (int i = 0; i<num; i++) {
		fread(pic, 1, w * h * 3, fp);
		fwrite(pic, 1, w * h, fp1);
		fwrite(pic + w * h, 1, w * h, fp2);
		fwrite(pic + w * h * 2, 1, w * h, fp3);
	}
	//��yuv420p�������ͼ򵥶��˸պ�һ��ͨ��һ������
	free(pic);
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	return 0;
}
//(3) ��YUV420P��������ȥ����ɫ����ɻҶ�ͼ��
//������YUV��ʽ�������ݱ�ɻҶ�ͼ��ֻ��Ҫ��U��V�������ó�128���ɡ�������ΪU��V��ͼ���еľ���ƫ�ô����ɫ�ȷ�����
//ɫ�ȷ�����ƫ�ô���ǰ��ȡֵ��Χ��-128��127��//��ʱ�����ɫ��Ӧ���ǡ�0��ֵ������ƫ�ú�ɫ�ȷ���ȡֵ�����0��255��
//�����ʱ����ɫ��Ӧ�ľ���128�ˡ��������ú����Ĵ������к�//�����һ�ŷֱ���Ϊ256x256������Ϊlena_256x256_yuv420p.
//yuv��YUV420P��ʽ�����������ļ����������Ϊoutput_gray.yuv��YUV420P��ʽ�����������ļ�
int yuv420_gray(char *url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("asset/output_yuv420_gray.yuv", "wb+");
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
//��ӦC�����е�unsigned char�������͡��������ú����Ĵ������к󣬽����һ�ŷֱ���Ϊ256x256������Ϊlena_256x256_yuv420p.yuv
//��YUV420P��ʽ�����������ļ����������Ϊoutput_half.yuv��YUV420P��ʽ�����������ļ�
int yuv420_harf_luminance(char *url, int w, int h, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("asset/output_yuv420_harf_luminance.yuv", "wb+");
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
//ͼ��ı߿�Ŀ��Ϊborder�������򽫾���ͼ���Եborder��Χ�ڵ����ص����ȷ���Y��ȡֵ���ó����������ֵ255��
//�������ú����Ĵ������к󣬽����һ�ŷֱ���Ϊ256x256������Ϊlena_256x256_yuv420p.yuv
//��YUV420P��ʽ�����������ļ����������Ϊoutput_border.yuv��YUV420P��ʽ�����������ļ�
int yuv420_add_Border(const char *url, int w, int h, int border, int num) {
	FILE *fp = fopen(url, "rb+");
	FILE *fp1 = fopen("asset/output_yuv420_add_border.yuv", "wb+");
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
int yuv420_graybar(int width, int height, int ymin, int ymax, int barnum, const char *url_output)
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
	FILE *fp1 = fopen("asset/output_r.y", "wb+");
	FILE *fp2 = fopen("asset/output_g.y", "wb+");
	FILE *fp3 = fopen("asset/output_b.y", "wb+");
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

//(9)��RGB24��ʽ��������ת��ΪYUV420P��ʽ��������
unsigned char clip_value(unsigned char x, unsigned char min_val, unsigned char max_val) {
	if (x > max_val)
		return max_val;
	else if (x < min_val)
		return min_val;
	else
		return x;
}

bool rgb24_to_yuv420p_buf(unsigned char *RgbBuf, int w, int h, unsigned char *yuvBuf) {
	unsigned char *ptrY, *ptrU, *ptrV, *ptrRGB;
	memset(yuvBuf, 0, w*h * 3 / 2);
	ptrY = yuvBuf;
	ptrU = yuvBuf + w * h;
	ptrV = ptrU + (w * h / 4);

	unsigned char y, u, v, r, g, b;
	for (int j = 0; j < h; j++) {
		ptrRGB = RgbBuf + w * j * 3;
		for (int i = 0; i < w; i++) {
			r = *(ptrRGB++);
			g = *(ptrRGB++);
			b = *(ptrRGB++);
			y = (unsigned char)((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
			u = (unsigned char)((-38 * r - 74 * g + 122 * b + 128) >> 8) + 128;
			v = (unsigned char)((122 * r -94 * g - 18 * b + 128) >> 8) + 128;
			*(ptrY++) = clip_value(y, 0, 255);
			if (j % 2 == 0 && i % 2 == 0) {
				*(ptrU++) = clip_value(u, 0, 255);
			}
			else {
				if (i % 2 == 0) 
					*(ptrV++) = clip_value(v, 0, 255);
			}
		}
	}
	return true;
}

int rgb24_to_yuv420p(const char *url_in, int w, int h, int num, const char* url_out) {
	FILE *fp_in = fopen(url_in, "rb+");
	FILE *fp_out = fopen(url_out, "wb+");
	unsigned char *pic_rgb24 = (unsigned char *)malloc(w*h * 3);
	unsigned char *pic_yuv420 = (unsigned char *)malloc(w*h * 3 / 2);
	for (int i = 0; i < num; i++) {
		fread(pic_rgb24, 1, w*h * 3, fp_in);
		rgb24_to_yuv420p_buf(pic_rgb24, w, h, pic_yuv420);
		fwrite(pic_yuv420, 1, w*h * 3 / 2, fp_out);
	}
	free(pic_rgb24);
	free(pic_yuv420);
	fclose(fp_in);
	fclose(fp_out);
	return 0;
}

//(11)����RGB24��ʽ�Ĳ�������ͼ
//��Դ������Կ�����������ѭ��������׻�����Ʒ�����ڡ�8����ɫ�Ĳ�������8����ɫ�Ĳ�����R��G��Bȡֵ������ʾ��
//��ɫ
//
//(R, G, B)
//
//��
//
//(255, 255, 255)
//
//��
//
//(255, 255, 0)
//
//��
//
//(0, 255, 255)
//
//��
//
//(0, 255, 0)
//
//Ʒ
//
//(255, 0, 255)
//
//��
//
//(255, 0, 0)
//
//��
//
//(0, 0, 255)
//
//��
//
//(0, 0, 0)

int rgb24_colorbar(int width, int height, const char *url_out) {
	unsigned char * data = NULL;
	int barWidth;
	char fileName[100] = { 0 };
	FILE *fp = NULL;
	int i = 0, j = 0;

	data = (unsigned char*)malloc(width * height * 3);
	barWidth = width / 8;
	if ((fp = fopen(url_out, "wb+")) == NULL) {
		printf("Error: Cannot create file!");
		return -1;
	}
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			int barNum = i / barWidth;
			switch (barNum)
			{
			case 0: {
				data[(j*width + i) * 3 + 0] = 255;
				data[(j*width + i) * 3 + 1] = 255;
				data[(j*width + i) * 3 + 2] = 255;
				break;
			}
			case 1: {
				data[(j*width + i) * 3 + 0] = 255;
				data[(j*width + i) * 3 + 1] = 255;
				data[(j*width + i) * 3 + 2] = 0;
				break;
			}
			case 2: {
				data[(j*width + i) * 3 + 0] = 0;
				data[(j*width + i) * 3 + 1] = 255;
				data[(j*width + i) * 3 + 2] = 0;
				break;
			}
			case 3: {
				data[(j*width + i) * 3 + 0] = 0;
				data[(j*width + i) * 3 + 1] = 255;
				data[(j*width + i) * 3 + 2] = 0;
				break;
			}
			case 4: {
				data[(j*width + i) * 3 + 0] = 255;
				data[(j*width + i) * 3 + 1] = 0;
				data[(j*width + i) * 3 + 2] = 255;
				break;
			}
			case 5: {
				data[(j*width + i) * 3 + 0] = 255;
				data[(j*width + i) * 3 + 1] = 0;
				data[(j*width + i) * 3 + 2] = 0;
				break;
			}
			case 6: {
				data[(j*width + i) * 3 + 0] = 0;
				data[(j*width + i) * 3 + 1] = 0;
				data[(j*width + i) * 3 + 2] = 255;
				break;
			}
			case 7: {
				data[(j*width + i) * 3 + 0] = 0;
				data[(j*width + i) * 3 + 1] = 0;
				data[(j*width + i) * 3 + 2] = 0;
				break;
			}
			default:
				break;
			}
		}
	}
	fwrite(data, width*height * 3, 1, fp);
	fclose(fp);
	free(data);
	return 0;
}

//(11)����PCM16LE˫������Ƶ�������ݵ���������������
//Test OK
int pcm16le_split_left_right(const char *url) {
	FILE *fp = fopen(url, "rb+");
	if (fp == NULL) {
		printf("Error: Cannot open file!");
		return -1;
	}
	FILE *fp_left = fopen("asset/left_music_output.pcm", "wb+");
	FILE *fp_right = fopen("asset/right_music_output.pcm", "wb+");

	unsigned char *sample = (unsigned char *)malloc(4);
	while (!feof(fp)) {
		fread(sample, 1, 4, fp);  //һ�ζ�ȡ4��Byte
		fwrite(sample, 1, 2, fp_left);  //ǰ�����������
		fwrite(sample + 2, 1, 2, fp_right);//�����������ұ�
	}
	free(sample);
	fclose(fp);
	fclose(fp_left);
	fclose(fp_right);
	return 0;
}

//(12)��PCM16LE˫������Ƶ������������������������һ��
//Test OK

int pcm16le_halfvolume_left_channel(const char *url) {
	FILE *fp = fopen(url, "rb+");
	if (fp == NULL) {
		printf("Error: Cannot open file!");
		return -1;
	}
	FILE *fp_half = fopen("asset/output_leftchannel_halfvolume.pcm", "wb+");
	int cnt = 0;
	unsigned char *sample = (unsigned char *)malloc(4);
	while (!feof(fp))
	{
		short * samplenum = NULL;
		fread(sample, 1, 4, fp);
		//printf("current fp location: %d\n", fp);
		samplenum = (short *)sample;
		*samplenum = *samplenum / 2;
		fwrite(sample, 1, 2, fp_half);
		fwrite(sample + 2, 1, 2, fp_half);
		cnt++;
	}
	printf("Sample Cnt:%d\n", cnt);
	free(sample);
	fclose(fp);
	fclose(fp_half);
	return 0;
}

//(13)��PCM16LE˫������Ƶ�������ݵ������ٶ����һ��
//Test OK
int pcm16le_speedX2(const char *url) {
	FILE *fp = fopen(url, "rb+");
	if (fp == NULL) {
		printf("Error: Cannot open file!");
		return -1;
	}
	int cnt = 0;
	FILE *fp_spped = fopen("asset/output_speedX2.pcm", "wb+");
	unsigned char *sample = (unsigned char *)malloc(4);
	while (!feof(fp))
	{
		fread(sample, 1, 4, fp);
		if (cnt % 2 != 0) {
			fwrite(sample, 1, 2, fp_spped);
			fwrite(sample + 2, 1, 2, fp_spped);
		}
		cnt++;
	}
	printf("Sample Cnt:%d\n", cnt);
	free(sample);
	fclose(fp);
	fclose(fp_spped);
	return 0;
}

//(14)��PCM16LE˫������Ƶ��������ת��ΪPCM8��Ƶ��������
//PCM16LE��ʽ�Ĳ������ݵ�ȡֵ��Χ�� - 32768��32767����PCM8��ʽ�Ĳ������ݵ�ȡֵ��Χ��0��255��
//����PCM16LEת����PCM8��Ҫ�����������裺��һ���ǽ� - 32768��32767��16bit�з�����ֵת��Ϊ
//- 128��127��8bit�з�����ֵ���ڶ����ǽ� - 128��127��8bit�з�����ֵת��Ϊ0��255��8bit�޷�����ֵ��
//�ڱ������У�16bit����������ͨ��short���ͱ����洢�ģ���8bit����������ͨ��unsigned char���ʹ洢��
//Test Fail
//����8bit ��PCM�ļ���������ʽ��һ�������� ��Ϊ0�ģ�����һ�������ĵ�Ϊ128�ġ�
//�������������Ǽ�����128�ģ��������ĵ���128�ģ���Ȼ��0Ϊ���ĵ�Ҳû����
//��fwrite(&samplenum8_u, 1, 1, fp8);�ĳ�fwrite(&samplenum8, 1, 1, fp8)����
//���ʱ���PCM�ļ�Ҫѡ��8bit PCM unsigned
int pcm16le_to_pcm8(const char *url) {
	FILE *fp = fopen(url, "rb+");
	if (fp == NULL) {
		printf("Error: Cannot open file!");
		return -1;
	}
	int cnt = 0;
	FILE *fp8 = fopen("asset/output_to_pcm8.pcm", "wb+");
	unsigned char *sample = (unsigned char *)malloc(4);
	while (!feof(fp))
	{
		short *samplenum16 = NULL;
		char samplenum8 = 0;
		unsigned char samplenum8_u = 0;
		fread(sample, 1, 4, fp);
		samplenum16 = (short *)sample;
		samplenum8 = (*samplenum16) >> 8;
		samplenum8_u = samplenum8 + 128;
		fwrite(&samplenum8_u, 1, 1, fp8);
		samplenum16 = (short *)(sample + 2);
		samplenum8 = (*samplenum16) >> 8;
		samplenum8_u = samplenum8 + 128;
		fwrite(&samplenum8_u, 1, 1, fp8);
		cnt++;
	}
	printf("Sample Cnt:%d\n", cnt);
	free(sample);
	fclose(fp);
	fclose(fp8);
	return 0;
}

//����PCM16LE��������Ƶ���������н�ȡһ��������
//Test OK
int pcm16le_cut_singlechanel(const char* url, int start_num, int dur_num) {
		FILE *fp = fopen(url, "rb+");
		if (fp == NULL) {
			printf("Error: Cannot open file!");
			return -1;
		}
		int cnt = 0;
		FILE * fp_cut = fopen("asset/output_cut_singlechanel.pcm", "wb+");
		FILE *fp_stat = fopen("asset/output_cut.txt", "wb+");
		unsigned char *sample = (unsigned char *)malloc(4);
		while (!feof(fp))
		{
			fread(sample, 1, 2, fp);
			if (cnt > start_num && cnt <= (start_num + dur_num)) {
				fwrite(sample, 1, 2, fp_cut);
				short samplenum = sample[1];
				samplenum = samplenum * 256;
				samplenum = samplenum + sample[0];
				fprintf(fp_stat, "%6d", samplenum);
				if (cnt % 10 == 0)
					fprintf(fp_stat, "\n", samplenum);
			}
			cnt++;
		}
		printf("Sample Cnt:%d\n", cnt);
		free(sample);
		fclose(fp);
		fclose(fp_cut);
		fclose(fp_stat);
		return 0;
}

//��PCM16LE˫������Ƶ��������ת��ΪWAVE��ʽ��Ƶ����
//WAVE�ļ���һ��RIFF��ʽ���ļ���������������ǡ�WAVE�������а����������ӿ顰fmt���͡�data����
//�ӱ�̵ĽǶȼ�˵��������WAVE_HEADER��WAVE_FMT��WAVE_DATA���������ݹ�4��������ɡ����Ľṹ������ʾ��
//WAVE_HEADER

//WAVE_FMT

//WAVE_DATA

//PCM����


//����ǰ3���ֵĽṹ������ʾ����д��WAVE�ļ�ͷ��ʱ������е�ÿ���ֶθ��Ϻ��ʵ�ֵ�Ϳ����ˡ�
//������һ����Ҫע�⣺WAVE_HEADER��WAVE_DATA�а�����һ���ļ�������Ϣ��dwSize�ֶΣ�
//���ֶε�ֵ������д������Ƶ��������֮����ܻ�á�����������ṹ������д��WAVE�ļ��С�
//�ļ���ʧ�ܣ�Ӧ������Ϣд������˴��󣬼��������ûʲô����
//memcpy(pcmFMT.fccID, "fmt ", strlen("fmt ")) fmt������Ͽո�����ˣ�������
int pcm16le_to_wav(const char* pcm_url, int channels, int sample_rate,const char *wav_url) {


	typedef struct WAVE_HEADER
	{
		char fccID[4];
		unsigned long dwSize;
		char fccType[4];
	}WAVE_HEADER;

	typedef struct WAVE_FMT
	{
		char fccID[4];
		unsigned long dwSize;
		unsigned short wFormatTag;
		unsigned short wChannels;
		unsigned long dwSamplesPerSec;
		unsigned long dwAvgBytesPerSec;
		unsigned short wBlockAlign;
		unsigned short uiBitsPerSample;
	}WAVE_FMT;

	typedef struct WAVE_DATA
	{
		char fccID[4];
		unsigned long dwSize;
	}WAVE_DATA;

	if (channels == 0 || sample_rate == 0) {
		channels = 2;
		sample_rate = 44100;
	}
	int bits = 16;
	WAVE_HEADER pcmHEADER;
	WAVE_FMT pcmFMT;
	WAVE_DATA pcmDATA;

	unsigned short m_pcmData;
	FILE *fp, *fp_out;

	fp = fopen(pcm_url, "rb+");
	if (fp == NULL) {
		printf("Error: Cannot open input PCM file!");
		return -1;
	}
	fp_out = fopen(wav_url, "wb+");
	if (fp_out == NULL) {
		printf("Error: Cannot create output file!");
		return -1;
	}

	//WAVE_HEADER
	memcpy(pcmHEADER.fccID, "RIFF", strlen("RIFF"));
	memcpy(pcmHEADER.fccType, "WAVE", strlen("WAVE"));
	fseek(fp_out, sizeof(WAVE_HEADER), 1);

	pcmFMT.dwSamplesPerSec = sample_rate;
	pcmFMT.dwAvgBytesPerSec = pcmFMT.dwSamplesPerSec * sizeof(m_pcmData);
	pcmFMT.uiBitsPerSample = bits;
	memcpy(pcmFMT.fccID, "fmt ", strlen("fmt ")); //ע��fmt������һ���ո�
	pcmFMT.dwSize = 16;
	pcmFMT.wBlockAlign = 2;
	pcmFMT.wChannels = channels;
	pcmFMT.wFormatTag = 1;

	fwrite(&pcmFMT, sizeof(WAVE_FMT), 1, fp_out);

	memcpy(pcmDATA.fccID,"data",strlen("data"));
	pcmDATA.dwSize = 0;
	fseek(fp_out, sizeof(WAVE_DATA), SEEK_CUR);

	fread(&m_pcmData, sizeof(unsigned short), 1, fp);
	while (!feof(fp))
	{
		pcmDATA.dwSize += 2;
		fwrite(&m_pcmData, sizeof(unsigned short), 1, fp_out);
		fread(&m_pcmData, sizeof(unsigned short), 1, fp);
	}
	pcmHEADER.dwSize = 44 + pcmDATA.dwSize;
	rewind(fp_out);
	fwrite(&pcmHEADER, sizeof(WAVE_HEADER), 1, fp_out);
	fseek(fp_out, sizeof(WAVE_FMT), SEEK_CUR);
	fwrite(&pcmDATA, sizeof(WAVE_DATA), 1, fp_out);
	fclose(fp);
	fclose(fp_out);

	return 0;
}