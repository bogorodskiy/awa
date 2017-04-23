#ifndef IRONBALLS_COMMON_H
#define IRONBALLS_COMMON_H

extern "C" {
    #include <EGL/egl.h>
    #include <GLES3/gl3.h>
    #include <jni.h>
    #include <errno.h>
    #include <android/log.h>
    #include <android_native_app_glue.h>
};

#define DEBUG_TAG "Awa:Native"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, DEBUG_TAG, "%s", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, DEBUG_TAG, "%s", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, DEBUG_TAG, "%s", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, DEBUG_TAG, "%s", __VA_ARGS__))

#include "../glm/glm.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#endif //IRONBALLS_COMMON_H