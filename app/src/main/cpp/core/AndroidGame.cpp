#include "AndroidGame.h"

const float AndroidGame::MAX_DELTA_TIME = 0.05f;

static void onAppCommand(struct android_app* app, int32_t cmd) {
    AndroidGame* game = (AndroidGame*) app->userData;
    game->handleCommand(cmd);
}

static int onAppInput(struct android_app* app, AInputEvent* event) {
    AndroidGame *game = (AndroidGame*) app->userData;
    return game->inputTouchLayer.processInputEvent(event) ? 1 : 0;
}

// TODO: Handle orientation changed (context destroyed)
AndroidGame::AndroidGame(struct android_app *app) :
        m_app(app),
        m_surfaceWidth(0),
        m_surfaceHeight(0),
        m_hasFocus(false),
        m_isVisible(false),
        m_hasWindow(false),
        m_hasGLErrors(false),
        m_surfaceSizeInitialized(false),
        m_eglDisplay(EGL_NO_DISPLAY),
        m_eglSurface(EGL_NO_SURFACE),
        m_eglContext(EGL_NO_CONTEXT),
        m_eglConfig(0)
{
    memset(&m_state, 0, sizeof(m_state));

    if (m_app->savedState != NULL) {
        m_state = *(struct AndroidGameSavedState*) m_app->savedState;
    }

    m_lastTime = getCurrentTime();
}

bool AndroidGame::getIsAnimating() {
    return m_hasFocus && m_isVisible && m_hasWindow;
}

void AndroidGame::startGameLoop() {
    m_app->userData = this;
    m_app->onAppCmd = onAppCommand;
    m_app->onInputEvent = onAppInput;

    while (1) {
        int ident;
        int events;

        struct android_poll_source* source;

        while ( (ident = ALooper_pollAll(getIsAnimating() ? 0 : -1, NULL, &events, (void**)&source)) > 0 ) {
            if (source != NULL) {
                source->process(m_app, source);
            }

            if (m_app->destroyRequested) {
                return;
            }
        }

        if (getIsAnimating()) {
            float deltaTime = getCurrentTime() - m_lastTime;
            m_lastTime = getCurrentTime();
            if (deltaTime > MAX_DELTA_TIME) {
                LOGD("Delta time %f", deltaTime);
            }
            deltaTime = (deltaTime < 0.0f) ? 0.0f : (deltaTime > MAX_DELTA_TIME) ? MAX_DELTA_TIME : deltaTime;
            update(deltaTime);
            if (preRender()) {
                render();
                postRender();
            }
        }
    }
}

static const char *_log_opengl_error(GLenum err) {
    switch (err) {
        case GL_NO_ERROR:
            LOGE("*** OpenGL error: GL_NO_ERROR");
            break;
        case GL_INVALID_ENUM:
            LOGE("*** OpenGL error: GL_INVALID_ENUM");
            break;
        case GL_INVALID_VALUE:
            LOGE("*** OpenGL error: GL_INVALID_VALUE");
            break;
        case GL_INVALID_OPERATION:
            LOGE("*** OpenGL error: GL_INVALID_OPERATION");
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            LOGE("*** OpenGL error: GL_INVALID_FRAMEBUFFER_OPERATION");
            break;
        case GL_OUT_OF_MEMORY:
            LOGE("*** OpenGL error: GL_OUT_OF_MEMORY");
            break;
        default:
            LOGE("*** OpenGL error: error %d", err);
            break;
    }
}

void AndroidGame::update(float deltaTime) {

}

bool AndroidGame::preRender() {

    if (m_eglDisplay == EGL_NO_DISPLAY ||
        m_eglSurface == EGL_NO_SURFACE ||
        m_eglContext == EGL_NO_CONTEXT) {

        // create display if needed
        if (!initDisplay()) {
            LOGE("Failed to create display.");
            return false;
        }

        // create surface if needed
        if (!initSurface()) {
            LOGE("Failed to create surface.");
            return false;
        }

        // create context if needed
        if (!initContext()) {
            LOGE("Failed to create context.");
            return false;
        }

        LOGD("Binding surface and context (display %p, surface %p, context %p)",
             m_eglDisplay, m_eglSurface, m_eglContext);

        // bind them
        if (EGL_FALSE == eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext)) {
            LOGE("eglMakeCurrent failed, EGL error %d", eglGetError());
            // TODO handle error
            m_hasGLErrors = true;
            return false;
        }

        // configure our global OpenGL settings
        configureOpenGL();

        if (!m_surfaceSizeInitialized) {
            int width;
            int height;
            eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_WIDTH, &width);
            eglQuerySurface(m_eglDisplay, m_eglSurface, EGL_HEIGHT, &height);
            if (m_surfaceWidth != width && m_surfaceHeight != height) {
                // notify scene manager that the surface has changed size
                LOGD("NativeEngine: surface size %dx%d --> %dx%d", m_surfaceWidth, m_surfaceHeight,
                     width, height);
                m_surfaceWidth = width;
                m_surfaceHeight = height;
                glViewport(0, 0, m_surfaceWidth, m_surfaceHeight);
                onResize();
                m_surfaceSizeInitialized = true;
            }
        }
    }

    return true;
}

void AndroidGame::configureOpenGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void AndroidGame::render() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void AndroidGame::postRender() {
    auto swapResult = eglSwapBuffers(m_eglDisplay, m_eglSurface);
    if (swapResult == EGL_FALSE) {
        LOGW("eglSwapBuffers failed, EGL error %d", eglGetError());
        // TODO handle error
        m_hasGLErrors = true;
        return;
    }

    GLenum error;
    static int errorsPrinted = 0;
    while ((error = glGetError()) != GL_NO_ERROR) {
        m_hasGLErrors = true;
        if (errorsPrinted < 10) {
            _log_opengl_error(error);
            ++errorsPrinted;
            if (errorsPrinted >= 10) {
                LOGE("*** TOO MANY OPENGL ERRORS. NO LONGER PRINTING.");
            }
        }
    }
}

void AndroidGame::handleCommand(int32_t cmd) {
    switch (cmd) {
        case APP_CMD_SAVE_STATE:
            // The system has asked us to save our current state.
            LOGD("APP_CMD_SAVE_STATE");
            m_app->savedState = malloc(sizeof(m_state));
            *((AndroidGameSavedState*)m_app->savedState) = m_state;
            m_app->savedStateSize = sizeof(m_state);
            break;
        case APP_CMD_INIT_WINDOW:
            // We have a window!
            LOGD("APP_CMD_INIT_WINDOW");
            if (m_app->window != NULL) {
                m_hasWindow = true;
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is going away -- kill the surface
            LOGD("APP_CMD_TERM_WINDOW");
            finalizeSurface();
            m_hasWindow = false;
            break;
        case APP_CMD_GAINED_FOCUS:
            LOGD("APP_CMD_GAINED_FOCUS");
            m_hasFocus = true;
            break;
        case APP_CMD_LOST_FOCUS:
            LOGD("APP_CMD_LOST_FOCUS");
            m_hasFocus = false;
            break;
        case APP_CMD_PAUSE:
            LOGD("APP_CMD_PAUSE");
            break;
        case APP_CMD_RESUME:
            LOGD("APP_CMD_RESUME");
            break;
        case APP_CMD_STOP:
            LOGD("APP_CMD_STOP");
            m_isVisible = false;
            break;
        case APP_CMD_START:
            LOGD("APP_CMD_START");
            m_isVisible = true;
            break;
        case APP_CMD_WINDOW_RESIZED:
        case APP_CMD_CONFIG_CHANGED:
            LOGD("%s", cmd == APP_CMD_WINDOW_RESIZED ?
                                     "APP_CMD_WINDOW_RESIZED" : "APP_CMD_CONFIG_CHANGED");
            // Window was resized or some other configuration changed.
            // Note: we don't handle this event because we check the surface dimensions
            // every frame, so that's how we know it was resized. If you are NOT doing that,
            // then you need to handle this event!
            break;
        case APP_CMD_LOW_MEMORY:
            LOGD("APP_CMD_LOW_MEMORY");
            // system told us we have low memory. So if we are not visible, let's
            // cooperate by deallocating all of our graphic resources.
            if (!m_hasWindow) {
                LOGD("trimming memory footprint (deleting GL objects).");
                // TODO kill gl objects
            }
            break;
        default:
            LOGD("(unknown command).");
            break;
    }
}

bool AndroidGame::initDisplay() {
    if (m_eglDisplay != EGL_NO_DISPLAY) {
        LOGD("No need to init display (already had one).");
        return true;
    }

    LOGD("Initializing display.");
    m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLBoolean eglInitResult = eglInitialize(m_eglDisplay, 0, 0);
    if (eglInitResult == EGL_FALSE) {
        LOGE("Failed to init display, error %d", eglGetError());
        return false;
    }
    return true;
}

bool AndroidGame::initSurface() {

    if (m_eglDisplay == EGL_NO_DISPLAY) {
        LOGE("Unable to init surface, there is no display");
        return false;
    }

    if (m_eglSurface != EGL_NO_SURFACE) {
        LOGD("No need to init surface (already had one).");
        return true;
    }

    LOGD("Initializing surface.");

    EGLint numConfigs;
    EGLint format;

    const EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, // request OpenGL ES 2.0
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
    };

    eglChooseConfig(m_eglDisplay, attribs, &m_eglConfig, 1, &numConfigs);

    // configure native window
    eglGetConfigAttrib(m_eglDisplay, m_eglConfig, EGL_NATIVE_VISUAL_ID, &format);
    ANativeWindow* window = m_app->window;
    bool hasWindow = (window != NULL);
    if (hasWindow == false) {
        LOGD("App has no window");
        return false;
    }

    ANativeWindow_setBuffersGeometry(m_app->window, 0, 0, format);

    // create EGL surface
    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_app->window, NULL);
    if (m_eglSurface == EGL_NO_SURFACE) {
        LOGE("Failed to create EGL surface, EGL error %d", eglGetError());
        return false;
    }

    LOGD("Successfully initialized surface.");
    return true;
}

bool AndroidGame::initContext() {
    if (m_eglDisplay == EGL_NO_DISPLAY) {
        LOGE("Unable to init context, there us no display");
        return false;
    }

    EGLint attribList[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE }; // OpenGL 3.0

    if (m_eglContext != EGL_NO_CONTEXT) {
        // nothing to do
        LOGD("No need to init context (already had one).");
        return true;
    }

    LOGD("Initializing context.");

    // create EGL context
    m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, NULL, attribList);
    if (m_eglContext == EGL_NO_CONTEXT) {
        LOGE("Failed to create EGL context, EGL error %d", eglGetError());
        return false;
    }

    LOGD("Successfull initialized context.");

    return true;
}

void AndroidGame::finalizeDisplay() {
    // causes context and surface to go away too, if they are there
    LOGD("Killing display.");
    finalizeContext();
    finalizeSurface();

    if (m_eglDisplay != EGL_NO_DISPLAY) {
        LOGD("Terminating display now.");
        eglTerminate(m_eglDisplay);
        m_eglDisplay = EGL_NO_DISPLAY;
    }
    LOGD("Display killed successfully.");
}

void AndroidGame::finalizeSurface() {
    LOGD("Killing surface.");
    eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    if (m_eglSurface != EGL_NO_SURFACE) {
        eglDestroySurface(m_eglDisplay, m_eglSurface);
        m_eglSurface = EGL_NO_SURFACE;
    }
    LOGD("Surface killed successfully.");
}

void AndroidGame::finalizeContext() {
    LOGD("Killing context.");

    // since the context is going away, we have to kill the GL objects
    //KillGLObjects();

    eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    if (m_eglContext != EGL_NO_CONTEXT) {
        eglDestroyContext(m_eglDisplay, m_eglContext);
        m_eglContext = EGL_NO_CONTEXT;
    }
    LOGD("Context killed successfully.");
}

float AndroidGame::getCurrentTime() {
    static struct timespec startTime;
    static bool firstCall = true;

    if (firstCall) {
        clock_gettime(CLOCK_MONOTONIC, &startTime);
        firstCall = false;
    }

    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    float secDiff = (float)(time.tv_sec - startTime.tv_sec);
    float msecDiff = (float)((time.tv_nsec - startTime.tv_nsec) / 1000000);
    return secDiff + 0.001f * msecDiff;
}

void AndroidGame::onResize() {

}

AndroidGame::~AndroidGame() {
    finalizeContext();
}