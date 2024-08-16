#ifndef AUDIO_COMMON_H
#define AUDIO_COMMON_H

//获取声音和播放声音的组件是 Qt 自带的 QAudioInput 和 QAudioOutput ,
#include<QAudioInput>
#include<QAudioOutput>
#include<QAudioFormat>
#include<QAudioDeviceInfo>
#include<QMessageBox>
#include<qDebug>
#include "WebRtc_Vad/webrtc_vad.h"
#define USE_VAD (1)


enum ENUM_PLAY_STATE{stopped,playing,pausing};

//使用speex
#include <speex/include/speex.h>

#define USE_SPEEX 1

//设置压缩质量
#define SPEEX_QUALITY (8)

#endif // AUDIO_COMMON_H
