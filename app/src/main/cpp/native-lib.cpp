#include <jni.h>
#include <string>
#include "DNFFmpeg.h"
#include "android/native_window_jni.h"
#include "macro.h"

extern "C" {
#include "libavcodec/avcodec.h"
}
JavaVM *javaVm = 0;
ANativeWindow *window = 0;
JavaCallHelper *helper=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int JNI_OnLoad(JavaVM *vm, void *r) {
    javaVm = vm;
    return JNI_VERSION_1_6;
}

DNFFmpeg *ffmpeg;

//画画
void render(uint8_t *data, int linesize, int w, int h) {
    pthread_mutex_lock(&mutex);
    if (!window) {
        pthread_mutex_unlock(&mutex);
        return;
    }

    ANativeWindow_setBuffersGeometry(window, w, h, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer window_buffer;
    if (ANativeWindow_lock(window, &window_buffer, 0)) {
        ANativeWindow_release(window);
        window = 0;
        pthread_mutex_unlock(&mutex);
        return;
    }

    uint8_t *dst_data = static_cast<uint8_t *>(window_buffer.bits);
    int dst_linesize = window_buffer.stride * 4;
    for (int i = 0; i < window_buffer.height; ++i) {
        memcpy(dst_data + i * dst_linesize, data + i * linesize, dst_linesize);
    }
    ANativeWindow_unlockAndPost(window);
    pthread_mutex_unlock(&mutex);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_day11_DNPlayer_native_1prepare(JNIEnv *env, jobject instance,
                                                jstring dataSource_) {
    const char *dataSource = env->GetStringUTFChars(dataSource_, 0);
    //创建播放器
    helper = new JavaCallHelper(javaVm, env, instance);
    ffmpeg = new DNFFmpeg(helper, dataSource);
    ffmpeg->setRenderFrameCallback(render);
    ffmpeg->prepare();
    env->ReleaseStringUTFChars(dataSource_, dataSource);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_day11_DNPlayer_native_1start(JNIEnv *env, jobject instance) {

    ffmpeg->start();

}extern "C"
JNIEXPORT void JNICALL
Java_com_example_day11_DNPlayer_native_1setSurface(JNIEnv *env, jobject instance, jobject surface) {
    pthread_mutex_lock(&mutex);
    if (window) {
        ANativeWindow_release(window);
        window = 0;
    }

    window = ANativeWindow_fromSurface(env, surface);
    pthread_mutex_unlock(&mutex);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_day11_DNPlayer_native_1release(JNIEnv *env, jobject instance) {

    pthread_mutex_lock(&mutex);
    if (window) {
        //把老的释放
        ANativeWindow_release(window);
        window = 0;
    }
    pthread_mutex_unlock(&mutex);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_day11_DNPlayer_native_1stop(JNIEnv *env, jobject instance) {

    if (ffmpeg) {
        ffmpeg->stop();
    }
    DELETE(helper);

}