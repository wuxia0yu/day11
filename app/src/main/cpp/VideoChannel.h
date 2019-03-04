//
// Created by administered on 2019/2/22.
//

#ifndef DAY11_VIDEOCHANNEL_H
#define DAY11_VIDEOCHANNEL_H


#include "BaseChannel.h"
#include "AudioChannel.h"

extern "C"{
#include <libswscale/swscale.h>
#include <libavcodec/avcodec.h>
}

typedef void  (*RenderFrameCallback)(uint8_t *, int,int , int);
class   VideoChannel : public  BaseChannel{
public:
    VideoChannel(int id,AVCodecContext *context,AVRational time_base ,int fps);
    ~VideoChannel();
    void  play();

    void  decode();

    void  rend();

    void  stop();

    void setAudioChannel(AudioChannel *audioChannel);

    void  setRenderFrameCallback(RenderFrameCallback callback);

private:
    pthread_t   pid_decode;
    pthread_t   pid_render;
    SwsContext *swsContext=0;
    int  fps;
    RenderFrameCallback callback;
    AudioChannel *audioChannel=0;
};




#endif //DAY11_VIDEOCHANNEL_H
