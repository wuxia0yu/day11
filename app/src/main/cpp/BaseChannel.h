//
// Created by administered on 2019/2/23.
//

#ifndef DAY11_BASECHANNEL_H
#define DAY11_BASECHANNEL_H

#include "safe_queue.h"
#include "macro.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include <libavformat/avformat.h>
};

class BaseChannel {
public:
    BaseChannel(int id, AVCodecContext *avCodecContext, AVRational time_base) : id(id),
                                                                                avCodecContext(
                                                                                        avCodecContext),
                                                                                time_base(
                                                                                        time_base) {}

    //虚函数
    virtual ~BaseChannel() {
        packets.setReleaseCallback(BaseChannel::releaseAvPacket);
        packets.clear();
    }

    //纯虚方法  相当于  抽象方法
    virtual void play() = 0;

    static void releaseAvPacket(AVPacket **avPacket) {
        if (avPacket) {
            av_packet_free(avPacket);
            //指针的指针能够修改指针的指向
            *avPacket = 0;
        }
    }

    static void releaseAvFrame(AVFrame **avFrame) {
        if (avFrame) {
            av_frame_free(avFrame);
            //指针的指针能够修改指针的指向
            *avFrame = 0;
        }
    }

    int id;
    bool isPlaying;
    SafeQueue<AVPacket *> packets;
    //解码数据包队列
    SafeQueue<AVFrame *> frames;
    AVCodecContext *avCodecContext = 0;
    AVRational time_base;
public:
    double clock;
};

#endif //DAY11_BASECHANNEL_H
