#include "game/Game.h"
#include "FreeImage.h"

void loadImage(AAssetManager* assetManager){
    AAsset* testAsset = AAssetManager_open(assetManager, "pig1.png", AASSET_MODE_UNKNOWN);
    if (testAsset != nullptr) {
        LOGI("ASSET PIG FOUND, TOTAL SIZE:%lli", AAsset_getLength64(testAsset));
        AAsset_close(testAsset);
    }
    else {
        LOGI("ASSET PIG NOT FOUND!");
    }

    FreeImage_Initialise(TRUE);

    //FIBITMAP* bitmap = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_PNG, "../../../assets/pig.png");

    FreeImage_DeInitialise();
}

//void getApkName() {
//    ANativeActivity* activity = state->activity;
//    JNIEnv* env=0;
//
//    activity->vm->AttachCurrentThread(&env, NULL);
//
//    jclass clazz = env->GetObjectClass(activity->clazz);
//    jmethodID methodID = env->GetMethodID(clazz, "getPackageCodePath", "()Ljava/lang/String;");
//    jobject result = env->CallObjectMethod(activity->clazz, methodID);
//
//    jboolean isCopy;
//    std::string res = env->GetStringUTFChars((jstring)result, &isCopy);
//    LOG_DEBUG("Looked up package code path: %s", res.c_str());
//
//    activity->vm->DetachCurrentThread();
//}

void android_main(struct android_app* app) {
    // TODO: remove test
    //loadImage(app->activity->assetManager);

    app_dummy();
    Game* game = new Game(app);
    game->startGameLoop();
    delete game;
}
