#ifndef IRONBALLS_ANDROIDGAME_H
#define IRONBALLS_ANDROIDGAME_H

extern "C" {
    #include <jni.h>
    #include <android_native_app_glue.h>
    #include <EGL/egl.h>
    #include <EGL/eglext.h>

    void android_main(struct android_app* state);
};

#include "common.h"
#include "stdlib.h"

struct AndroidGameSavedState {
    int32_t x;
    int32_t y;
};

class AndroidGame {
public:
    AndroidGame(struct android_app *app);
    virtual ~AndroidGame();

    bool getIsAnimating();

    void startGameLoop();
    virtual void update(float dt) = 0;
    virtual void render();

    void handleCommand(int32_t cmd);
    virtual int onInputEvent(AInputEvent* event) = 0;

protected:
    struct android_app* m_app;
    int m_surfaceWidth;
    int m_surfaceHeight;

    double getCurrentTime();
    virtual void onResize();
    virtual void onPause() = 0;
    virtual void onResume() = 0;
    virtual void startGraphics() = 0;
    virtual void killGraphics() = 0;
private:
    static const float SIMULATION_TIME;
    struct AndroidGameSavedState m_state;

    bool m_hasFocus;
    bool m_isVisible;
    bool m_hasWindow;
    bool m_surfaceSizeInitialized;
    bool m_graphicsStarted;

    EGLDisplay m_eglDisplay;
    EGLSurface m_eglSurface;
    EGLContext m_eglContext;
    EGLConfig m_eglConfig;

    double m_lastUpdateTime;

    bool initDisplay();
    bool initSurface();
    bool initContext();

    void finalizeContext();
    void finalizeSurface();
    void finalizeDisplay();

    bool handleEglError(EGLint error);

    bool preRender();
    void postRender();
};


#endif //IRONBALLS_ANDROIDGAME_H
