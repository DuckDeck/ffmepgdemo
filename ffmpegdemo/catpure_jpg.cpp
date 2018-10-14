#include "stdafx.h"
#include <opencv2/opencv.hpp>
#define __STDC_CONSTANT_MACROS
#ifdef _WIN32
//Windows
extern "C"
{
#include "libavutil/opt.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#ifdef __cplusplus
};
#endif
#endif
#define MAX_PATH 128

int WriteJPEG(AVFrame* pFrame, int width, int height, int iIndex)
{
	// ����ļ�·��
	char out_file[MAX_PATH] = { 0 };
	//   sprintf_s(out_file, sizeof(out_file), "./%d.jpg",  iIndex);

	sprintf(out_file, "asset/%d.jpg", iIndex);

	// ����AVFormatContext����
	AVFormatContext* pFormatCtx = avformat_alloc_context();

	// ��������ļ���ʽ
	pFormatCtx->oformat = av_guess_format("mjpeg", NULL, NULL);

	// ��������ʼ��һ���͸�url��ص�AVIOContext
	if (avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_READ_WRITE) < 0)
	{
		printf("Couldn't open output file.");
		return -1;
	}

	// ����һ����stream
	AVStream* pAVStream = avformat_new_stream(pFormatCtx, 0);
	if (pAVStream == NULL)
	{
		return -1;
	}
	// ���ø�stream����Ϣ
	AVCodecContext* pCodecCtx = pAVStream->codec;
	pCodecCtx->codec_id = pFormatCtx->oformat->video_codec;
	pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
	pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
	pCodecCtx->width = width;
	pCodecCtx->height = height;
	pCodecCtx->time_base.num = 1;
	pCodecCtx->time_base.den = 25;

	// Begin Output some information
	av_dump_format(pFormatCtx, 0, out_file, 1);
	// End Output some information
	// Begin Output some information
	av_dump_format(pFormatCtx, 0, out_file, 1);
	// End Output some information

	// ���ҽ�����
	AVCodec* pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
	if (!pCodec)
	{
		printf("Codec not found.");
		return -1;
	}
	// ����pCodecCtx�Ľ�����ΪpCodec
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("Could not open codec.");
		return -1;
	}

	//Write Header
	avformat_write_header(pFormatCtx, NULL);
	int y_size = pCodecCtx->width * pCodecCtx->height;
	//Encode
	// ��AVPacket�����㹻��Ŀռ�
	AVPacket pkt;
	av_new_packet(&pkt, y_size * 3);
	// 
	int got_picture = 0;
	int ret = avcodec_encode_video2(pCodecCtx, &pkt, pFrame, &got_picture);
	if (ret < 0)
	{
		printf("Encode Error.\n");
		return -1;
	}
	printf("got_picture %d \n", got_picture);
	if (got_picture == 1)
	{
		//pkt.stream_index = pAVStream->index;
		ret = av_write_frame(pFormatCtx, &pkt);
	}
	av_free_packet(&pkt);
	//Write Trailer
	av_write_trailer(pFormatCtx);
	printf("Encode Successful.\n");
	if (pAVStream)
	{
		avcodec_close(pAVStream->codec);
	}
	avio_close(pFormatCtx->pb);
	avformat_free_context(pFormatCtx);
	return 0;
}



int capture_jpg(const char* videoPath) {
	int videoStream = -1;
	AVCodecContext *pCodecCtx;
	AVFormatContext *pFormatCtx;
	AVCodec *pCodec;
	AVFrame *pFrame, *pFrameRGB;
	struct SwsContext *pSwsCtx;
	
	AVPacket* packet;
	int frameFinished;
	int PictureSize;
	uint8_t *outBuff;
	//ע��������
	av_register_all();
	// ����AVFormatContext
	pFormatCtx = avformat_alloc_context();

	//����Ƶ�ļ�
	if (avformat_open_input(&pFormatCtx, videoPath, NULL, NULL) != 0)
	{
		printf("av open input file failed!\n");
		exit(1);
	}


	//��ȡ����Ϣ
	if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
	{
		printf("av find stream info failed!\n");
		exit(1);
	}

	printf(" pFormatCtx->nb_streams %d \n", pFormatCtx->nb_streams);


	//��ȡ��Ƶ��
	for (int i = 0; i < pFormatCtx->nb_streams; i++)
	{
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}
	if (videoStream == -1)
	{
		printf("find video stream failed!\n");
		exit(1);
	}


	// Ѱ�ҽ�����
	pCodecCtx = pFormatCtx->streams[videoStream]->codec;

	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

	if (pCodec == NULL)
	{
		printf("avcode find decoder failed!\n");
		exit(1);
	}


	//�򿪽�����
	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
	{
		printf("avcode open failed!\n");
		exit(1);
	}


	//Ϊÿ֡ͼ������ڴ�
	pFrame = av_frame_alloc();

	pFrameRGB = av_frame_alloc(); //�洢�����ת����RGB����

	int size = avpicture_get_size(AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);
	uint8_t *out_buffer = (uint8_t *)av_malloc(size);
	avpicture_fill((AVPicture *)pFrameRGB, out_buffer, AV_PIX_FMT_BGR24, pCodecCtx->width, pCodecCtx->height);
	 packet = (AVPacket *)av_malloc(sizeof(AVPacket));
	struct SwsContext *img_convert_ctx;
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);
	cv::Mat pCvMat;
	pCvMat.create(cv::Size(pCodecCtx->width, pCodecCtx->height), CV_8UC3);

	int i = 0;

	while (av_read_frame(pFormatCtx, packet) >= 0)
	{
		if (packet->stream_index == videoStream)
		{
			avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, packet);


			if (frameFinished)
			{
				//sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize,0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize);
				//memcpy(pCvMat.data, out_buffer, size);
				//cv::imshow("RGB", pCvMat);
				//testface(pCvMat);
				
				printf("the frame num is %d",i);
				//break;
				//cvWaitKey(1);
				//������������ʶ��
				//break;
				if (i % 10 == 0) { //ÿ10֡��һ��ͼƬ����Ȼ̫����
					WriteJPEG(pFrame, pCodecCtx->width, pCodecCtx->height, i);
				}
				i++;
			}
		}
		else
		{
			int a = 2;
			int b = a;
		}

		
		av_free_packet(packet);		
	}


	//sws_freeContext(pSwsCtx);

	//getchar();
	av_free(pFrame);
	av_free(pFrameRGB);
	av_free(out_buffer);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
	sws_freeContext(img_convert_ctx);
	return 0;

}

