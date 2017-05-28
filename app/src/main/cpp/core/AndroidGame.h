#ifndef IRONBALLS_ANDROIDGAME_H
#define IRONBALLS_ANDROIDGAME_H

extern "C" {
    #include <jni.h>
    #include <android_native_app_glue.h>
    #include <EGL/egl.h>
    #include <EGL/eglext.h>

    void android_main(struct android_app* state);
};

#include <time.h>
#include "common.h"
#include "../core/input/TouchLayer.h"

struct AndroidGameSavedState {
    int32_t x;
    int32_t y;
};

class AndroidGame {
public:
    // core systems
    TouchLayer inputTouchLayer;

    AndroidGame(struct android_app *app);
    ~AndroidGame();

    bool getIsAnimating();

    void startGameLoop();
    virtual void update(float deltaTime);
    virtual void render();

    void handleCommand(int32_t cmd);

protected:
    struct android_app* m_app;
    int m_surfaceWidth;
    int m_surfaceHeight;

    virtual void configureOpenGL();
    virtual void onResize();

private:
    static const float MAX_DELTA_TIME;
    struct AndroidGameSavedState m_state;

    bool m_hasFocus;
    bool m_isVisible;
    bool m_hasWindow;
    bool m_hasGLErrors;
    bool m_surfaceSizeInitialized;

    // EGL stuff
    EGLDisplay m_eglDisplay;
    EGLSurface m_eglSurface;
    EGLContext m_eglContext;
    EGLConfig m_eglConfig;

    float m_lastTime;

    bool initDisplay();
    bool initSurface();
    bool initContext();

    void finalizeContext();
    void finalizeSurface();
    void finalizeDisplay();

    bool preRender();
    void postRender();
    float getCurrentTime();
};


#endif //IRONBALLS_ANDROIDGAME_H
