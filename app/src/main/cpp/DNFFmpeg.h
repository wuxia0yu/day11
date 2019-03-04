//
// Created by administered on 2019/2/21.
//

#ifndef DAY11_DNFFMPEG_H
#define DAY11_DNFFMPEG_H
extern "C" {
#include <libavformat/avformat.h>
}

#include "JavaCallHelper.h"
#include "AudioChannel.h"
#include "VideoChannel.h"

class DNFFmpeg {
public:
    DNFFmpeg(JavaCallHelper *helper, const char *dataSource);

    ~DNFFmpeg();

    void prepare();

    void _prepare();

    void start();

    void _start();

    void stop();

    void setRenderFrameCallback(RenderFrameCallback callback);

private:
    char *dataSource;
    JavaCallHelper *callHelper = 0;
    RenderFrameCallback callback;
    bool isPlaying;
public:
    pthread_t pid;
    pthread_t pid_play;
    pthread_t pid_stop;
    AudioChannel *audioChannel = 0;
    VideoChannel *videoChannel = 0;
    //1.打开媒体地址（文件地址和直播地址）
    //2.AVFormatContext 包含了视屏的信息（宽高等）
    AVFormatContext *avFormatContext = 0;
};


#endif //DAY11_DNFFMPEG_H
