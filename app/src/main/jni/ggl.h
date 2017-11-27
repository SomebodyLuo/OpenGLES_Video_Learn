//
// Created by pc on 2017/11/24.
//

#ifndef GLDEMO_GGL_H
#define GLDEMO_GGL_H


#include <jni.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <android/log.h>

#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <map>

#include "Glm/glm.hpp"
#include "Glm/ext.hpp"

#define TAG "luoyouren" // 这个是自定义TAG标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

#endif //GLDEMO_GGL_H
