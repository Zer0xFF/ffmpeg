/*
 * Copyright (c) 2001 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * libavcodec API use example.
 *
 * Note that libavcodec only handles codecs (mpeg, mpeg4, etc...),
 * not file formats (avi, vob, mp4, mov, mkv, mxf, flv, mpegts, mpegps, etc...). See library 'libavformat' for the
 * format handling
 * @example doc/examples/decoding_encoding.c
 */

#include <math.h>

#include <kernel_ex.h>
#include <sysmodule_ex.h>
#include <system_service_ex.h>
#include <shellcore_util.h>
#include <lnc_util.h>

#include <usb_storage.h>
#include <fios2.h>

#include "./orbis2d.h"
#include "./debugnet.h"


#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavformat/avformat.h>

#include "swscale.h"


#define MAX_PATH_LENGTH 256

size_t sceLibcHeapInitialSize = 1024 * 1024;	// Set up initial allocation size as 1MiB
size_t sceLibcHeapSize = 20 * 1024 * 1024;
unsigned int sceLibcHeapExtendedAlloc = 1;
size_t sceUserMainThreadStackSize = 512 * 1024 / 2;

#define INBUF_SIZE 4096
#define AUDIO_INBUF_SIZE 20480
#define AUDIO_REFILL_THRESH 4096

/* check that a given sample format is supported by the encoder */
static int check_sample_fmt(AVCodec *codec, enum AVSampleFormat sample_fmt)
{
    const enum AVSampleFormat *p = codec->sample_fmts;

    while (*p != AV_SAMPLE_FMT_NONE) {
        if (*p == sample_fmt)
            return 1;
        p++;
    }
    return 0;
}

/*
 * Video decoding example
 */

static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize)
{
    static int flipArg = 0;

    orbis2dStartDrawing();
    orbis2dClearBuffer();
    for(int y = 0 ; y < ysize; y++)
    {
        // for(int x = 0 ; x < xsize; x++)
        // {
        // int pixel =  (y * wrap) + x;
        // uint8_t * rgb = (uint8_t *)buf;
        // uint8_t * rgb_seek = &rgb[pixel * 4];
        // uint8_t rgb_buff[4] = {
        //     rgb_seek[0],
        //     rgb_seek[1],
        //     rgb_seek[2],
        //     0x80,
        //     };
        //     uint32_t * rgb_push = (uint32_t *)rgb_buff;

        // orbis2dDrawPixelColor(x, y, (uint32_t *)rgb_seek); // print to screen?
        // //orbis2dDrawPixelRGB(x, y, buf[pixel][0], buf[pixel][1], buf[pixel][2]); // print to screen?
        // }
    }
    orbis2dPutImage(buf, 0, 0, xsize, ysize);
    orbis2dFinishDrawing(flipArg);
    //swap buffers
    orbis2dSwapBuffers();
    flipArg++;
    return;

    //orbis2dDrawPixelColor(int x, int y, uint32_t pixelColor)
    if(false)
    {
        orbis2dStartDrawing();

		orbis2dClearBuffer();

        // orbis2dPutImage1(0, 0, buf, xsize, ysize);
        // orbis2dPutImage2(0, 0, buf, xsize, ysize);
        // orbis2dPutImage3(0, 0, buf, xsize, ysize);
        // orbis2dPutImage4(0, 0, buf, xsize, ysize);
        // orbis2dPutImage5(0, 0, buf, xsize, ysize); 

		orbis2dFinishDrawing(flipArg);
		orbis2dSwapBuffers();
        flipArg++;
    }
}
static struct SwsContext * m_sws_context = NULL;

#define FMTFMT AV_PIX_FMT_BGRA
//#define FMTFMT AV_PIX_FMT_RGB24
//#define FMTFMT AV_PIX_FMT_ABGR
//#define FMTFMT AV_PIX_FMT_BGRA
//#define FMTFMT AV_PIX_FMT_ARGB
//#define FMTFMT AV_PIX_FMT_RGBA

static int decode_write_frame(AVCodecContext *avctx, AVFrame *frame, AVFrame *frameRGB, int *frame_count, AVPacket *pkt, int last)
{
    static int frames = 0, failed_f = 0;
    int len, got_frame;

    len = avcodec_decode_video2(avctx, frame, &got_frame, pkt);
    if (len < 0)
    {
        frames++;
        (*frame_count)++;
        failed_f++;
        if(failed_f > 5)
        {
            send_notify("5 frames failed, exiting\n");
            return len;
        }
        else if(failed_f == 1)
        {
            send_notify("Error while decoding frame %d\n", (*frame_count) - 1);
        }
        
        return 0;
    }
    if (got_frame)
    {
        
        int sws = -1;
        sws = sws_scale(m_sws_context, frame->data, frame->linesize, 0, avctx->height, frameRGB->data, frameRGB->linesize);

        //ffmpeg_encoder_set_frame_rgb_from_yuv(avctx->width, avctx->height, frame, rgb24, rgb24_stride);
        pgm_save(frameRGB->data[0], frameRGB->linesize[0], avctx->width, avctx->height);
        //send_notify("%d frame drawn\n", frames);
        (*frame_count)++;
    }
    if (pkt->data)
    {
        pkt->size -= len;
        pkt->data += len;
    }
    frames++;
    return 0;
}

static int init_usb()
{
	// TODO find which one is automount 0x103, 0x104, 0xf7, 0xd0, 0xcd, 0xcc
	int ret = sceSysmoduleLoadModule(0xD5);
	if (ret != SCE_OK) {
		send_notify("Error: sceSysmoduleLoadModule(SCE_SYSMODULE_USB_STORAGE), ret 0x%08x\n", ret);
		return ret;
	}


	int res = 0;

	typedef int32_t (*__sceUsbStorageRequestMap2)(uint32_t id, const char * directory, const char * mountpt);
	res = sceUsbStorageInit(0);
	int32_t device_count = 0;
	uint32_t device_ids[10] = {0};
	sceUsbStorageGetDeviceList(device_ids, &device_count);
	for(int i = 0; i < device_count; ++i )
	{
//		SceUsbStorageDeviceInfo info = {0};
//		if((res = sceUsbStorageGetDeviceInfo(device_ids[i], &info)) == 0)
		{
			__sceUsbStorageRequestMap2 _sceUsbStorageRequestMap2 = (__sceUsbStorageRequestMap2)(((uint64_t)&(*sceUsbStorageRequestMap)) - (0xF0 - 0x50));
			res = _sceUsbStorageRequestMap2(device_ids[i],"/", "/usb_0");
		}
	}

	return 0;
}

static void cleanup(void) {
	int ret;

	ret = sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE);
}

static void init()
{
    atexit(&cleanup);

	int ret = sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_SYSTEM_SERVICE);
	if (ret)
    {
		exit(-255);
	}
    const char * g_sandbox_word = sceKernelGetFsSandboxRandomWord();

    init_usb();
    orbis2dInit();
    sceSystemServiceHideSplashScreen();

}

void catchReturnFromMain(int exit_code) {
	/* dummy */
}

AVFormatContext * m_fmtCtx = NULL;

int OpenCodecContext(const char *filename, int *stream_idx, enum AVMediaType type, int threadCount)
{
    AVCodecContext *dec_ctx = NULL;
    AVCodec *dec = NULL;
    int ret;
    AVStream *st;

    ret = av_find_best_stream(m_fmtCtx, type, -1, -1, NULL, 0);
    if (ret < 0) {
        send_notify("ERROR: Could not find %s stream in input file '%s'\n", av_get_media_type_string(type), filename);
        return ret;
    } else {
        *stream_idx = ret;
		st = m_fmtCtx->streams[*stream_idx];

        /* find decoder for the stream */
        dec_ctx = st->codec;
        dec = avcodec_find_decoder(dec_ctx->codec_id);
        if (!dec) {
            send_notify("ERROR: Failed to find %s codec\n", av_get_media_type_string(type));
            return ret;
        }

		dec_ctx->thread_count = threadCount;
        if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0)
        {
            send_notify("ERROR: Failed to open %s codec\n", av_get_media_type_string(type));
            return ret;
        }

    }

    return 0;
}


static void video_decode_example(const char *filename, int codec_id)
{

    int res;
    if ((res = avformat_open_input(&m_fmtCtx, filename, NULL, NULL)) < 0)
	{
		send_notify("ERROR: (%d) %s\n", res, av_err2str(res));
		return;
	} 

    if (avformat_find_stream_info(m_fmtCtx, NULL) < 0)
	{
		if (m_fmtCtx != NULL)
			avformat_close_input(&m_fmtCtx);

		send_notify("ERROR: Could not find stream information for file: %s\n", filename);
		return;
    }

    AVCodec *codec;
    AVCodecContext * c = NULL;
    int frame_count;
    AVPacket avpkt;

    av_init_packet(&avpkt);

    int stream_idx = -1;
    if(OpenCodecContext(filename, &stream_idx, AVMEDIA_TYPE_VIDEO, 8) < 0)
    {
        send_notify("Failed to OpenCodecContext");
        exit(-32);
    }
    AVStream* m_videoStream = m_fmtCtx->streams[stream_idx];
    c = m_videoStream->codec;
    //video_refresh_rate = 1 / m_videoStream->avg_frame_rate;
    codec = avcodec_find_decoder(c->codec_id);


    m_sws_context = sws_getContext(
        c->width, c->height, c->pix_fmt,
        c->width, c->height, FMTFMT,
        SWS_BILINEAR, 0, 0, 0);
        

    AVFrame *frame = avcodec_alloc_frame();
    AVFrame *frameRGB = avcodec_alloc_frame();
    if (
        !frame
        || !frameRGB
    )
    {
        send_notify("Could not allocate video frame\n");
        exit(1);
    }
    av_image_fill_linesizes(frameRGB->linesize, FMTFMT, c->width);
    for (int i = 0; i < 4; i++)
    {
        frameRGB->linesize[i] = FFALIGN(frameRGB->linesize[i], 16);
        if (frameRGB->linesize[i])
        {
            frameRGB->data[i] = (uint8_t*)av_mallocz(frameRGB->linesize[i] * c->height);
        }

        if (frameRGB->linesize[i] && !frameRGB->data[i])
        {
            send_notify("Malloc i:%d line:%d buffer:%d", i, frameRGB->linesize[i], frameRGB->data[i], c->height);
            exit(-101);
        }
    }
    int frame_size = frameRGB->linesize[0] * c->height;
    send_notify("r_frame_rate den %d\n", m_videoStream->r_frame_rate.den);
    send_notify("r_frame_rate num %d\n", m_videoStream->r_frame_rate.num);
    send_notify("nano %d\n", (m_videoStream->r_frame_rate.den * 1000 * 1000 * 1000) / m_videoStream->r_frame_rate.num);
    frame_count = 0;
    while(av_read_frame(m_fmtCtx, &avpkt) >= 0)
    {
        if(avpkt.stream_index == stream_idx)
        {
            //memset(frameRGB->data[0], 0, frame_size);
            if (decode_write_frame(c, frame, frameRGB, &frame_count, &avpkt, 0) < 0)
            {
                send_notify("decode_write_frame exit?");
                exit(1);
            }
            //sceKernelSleep(1);
            SceKernelTimespec wait_time;
			wait_time.tv_sec = 0;
			wait_time.tv_nsec= (1 * 1000 * 1000 * 1000) / 60;	//16ms OK
            //send_notify("nano sleep %d\n", (m_videoStream->r_frame_rate.den * 1000 * 1000 * 1000) / m_videoStream->r_frame_rate.num);
			//wait_time.tv_nsec= 30000000;	//30ms OK
			sceKernelNanosleep(&wait_time, NULL);
        }
        // decode packet and other stuff
        av_packet_unref(&avpkt); 
    }
    send_notify("decoding finished?");
    avpkt.data = NULL;
    avpkt.size = 0;
    decode_write_frame(c, frame, frameRGB, &frame_count, &avpkt, 1);


    avcodec_close(c);
    av_free(c);
    avcodec_free_frame(&frame);
    printf("\n");
}

/* 64 ops: */
int64_t g_OpStorage[SCE_FIOS_DIVIDE_ROUNDING_UP(SCE_FIOS_OP_STORAGE_SIZE(64, MAX_PATH_LENGTH), sizeof(int64_t))];
/* 1024 chunks, 64KiB: */
int64_t g_ChunkStorage[SCE_FIOS_DIVIDE_ROUNDING_UP(SCE_FIOS_CHUNK_STORAGE_SIZE(1024), sizeof(int64_t))];
/* 16 file handles: */
int64_t g_FHStorage[SCE_FIOS_DIVIDE_ROUNDING_UP(SCE_FIOS_FH_STORAGE_SIZE(16, MAX_PATH_LENGTH), sizeof(int64_t))];
/* 4 directory handle: */
int64_t g_DHStorage[SCE_FIOS_DIVIDE_ROUNDING_UP(SCE_FIOS_DH_STORAGE_SIZE(4, MAX_PATH_LENGTH), sizeof(int64_t))];

int main(int argc, char **argv)
{
    init();


    // /*E Provide required storage buffers. */
    SceFiosParams params = SCE_FIOS_PARAMS_INITIALIZER;
    params.opStorage.pPtr = g_OpStorage;
    params.opStorage.length = sizeof(g_OpStorage);
    params.chunkStorage.pPtr = g_ChunkStorage;
    params.chunkStorage.length = sizeof(g_ChunkStorage);
    params.fhStorage.pPtr = g_FHStorage;
    params.fhStorage.length = sizeof(g_FHStorage);
    params.dhStorage.pPtr = g_DHStorage;
    params.dhStorage.length = sizeof(g_DHStorage);

    params.pathMax = MAX_PATH_LENGTH;

    int err = sceFiosInitialize(&params);
    if (err != SCE_FIOS_OK)
        send_notify("failed init fios");
   
    /* register all the codecs */
    //av_log_set_level(AV_LOG_DEBUG);
    av_register_all();
    avcodec_register_all();
    //av_log_set_callback(send_notify2);
    //send_notify("registed all");

    debug_init();
    for(int i = 0; i < 100; i++)
    {
        printf("Hello socket bitch\n");
    }

    //void * mems = av_buffer_alloc(13 * 1024 * 1024);
    // for(int i = 13; i < 35; i++)
    // {
    //     posix_memalign((void **)&mems, 32, 1024 * 1024 * i);
    //     if(!mems) {send_notify("failed %d", i); break;} else {send_notify("pass %d", i);free(mems);}

    // }
    //    posix_memalign((void **)&mems, 32, 1010271);
    //if(!mems) {send_notify("failed 1sds");} else {send_notify("pass 1asd");av_free(mems);}
    // posix_memalign((void **)&mems, 32, 1024 * 2);
    // if(!mems) {send_notify("failed 2");} else {send_notify("pass 2");free(mems);}
    // posix_memalign((void **)&mems, 32, 1024 * 3);
    // if(!mems) {send_notify("failed 3");} else {send_notify("pass 3");free(mems);}
    // posix_memalign((void **)&mems, 32, 1024 * 4);
    // if(!mems) {send_notify("failed 4");} else {send_notify("pass 4");free(mems);}
    // posix_memalign((void **)&mems, 32, 1024 * 5);
    // if(!mems) {send_notify("failed 5");} else {send_notify("pass 5");free(mems);}
    // posix_memalign((void **)&mems, 32, 1024 * 6);
    // if(!mems) {send_notify("failed 6");} else {send_notify("pass 6");free(mems);}
    // posix_memalign((void **)&mems, 32, 1024 * 7);
    // if(!mems) {send_notify("failed 7");} else {send_notify("pass 7");free(mems);}
    // posix_memalign((void **)&mems, 32, 1024 * 8);
    // if(!mems) {send_notify("failed 8");} else {send_notify("pass 8");free(mems);}
    // posix_memalign((void **)&mems, 32, 1024 * 9);
    // if(!mems) {send_notify("failed 9");} else {send_notify("pass 9");free(mems);}
    //video_decode_example("https://192-168-0-38.8555ba773a01466fad10115c2741683d.plex.direct:32400/video/:/transcode/universal/dash/spwl2jrx898sfvkk77m06qcx/1/56.m4s", AV_CODEC_ID_H264);
    //video_decode_example("/usb_0/big_buck_bunny_480p_h264.mov", AV_CODEC_ID_H264);
    //video_decode_example("/usb_0/big_buck_bunny_720p_h264.mov", AV_CODEC_ID_H264);
    video_decode_example("/usb_0/big_buck_bunny_1080p_h264.mov", AV_CODEC_ID_H264);


    return 0;
}
