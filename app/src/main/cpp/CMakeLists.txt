project(awa)

cmake_minimum_required(VERSION 3.4.1)

#instead of calling app_dummy
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

set(LIBS_DIR ${CMAKE_SOURCE_DIR}/../../../libs)

add_library(physx STATIC IMPORTED)
set_target_properties(physx PROPERTIES IMPORTED_LOCATION ${LIBS_DIR}/physx/${ANDROID_ABI}/libPhysX3.a)

add_library(physx_common STATIC IMPORTED)
set_target_properties(physx_common PROPERTIES IMPORTED_LOCATION ${LIBS_DIR}/physx/${ANDROID_ABI}/libPhysX3Common.a)

add_library(physx_extensions STATIC IMPORTED)
set_target_properties(physx_extensions PROPERTIES IMPORTED_LOCATION ${LIBS_DIR}/physx/${ANDROID_ABI}/libPhysX3Extensions.a)

add_library(physx_foundation STATIC IMPORTED)
set_target_properties(physx_foundation PROPERTIES IMPORTED_LOCATION ${LIBS_DIR}/physx/${ANDROID_ABI}/libPxFoundation.a)

add_library(physx_task STATIC IMPORTED)
set_target_properties(physx_task PROPERTIES IMPORTED_LOCATION ${LIBS_DIR}/physx/${ANDROID_ABI}/libPxTask.a)

add_library(app-glue STATIC ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

add_library(free_image STATIC IMPORTED)
set_target_properties(free_image PROPERTIES IMPORTED_LOCATION ${LIBS_DIR}/freeimage/${ANDROID_ABI}/libFreeImage.a)

# now build app's shared lib
#-DNDEBUG for physx
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DNDEBUG")
add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")

set(SOURCE main.cpp)
add_subdirectory(core)
add_subdirectory(game)
add_subdirectory(glm)

set(SOURCE ${SOURCE} ${GAME_SRC} ${CORE_SRC})

#message( FATAL_ERROR ${SOURCE} )

add_library(native-activity SHARED ${SOURCE})

#physx
target_include_directories(native-activity PRIVATE ${LIBS_DIR}/physx/)
target_include_directories(native-activity PRIVATE ${LIBS_DIR}/physx/foundation)
target_include_directories(native-activity PRIVATE ${LIBS_DIR}/physx/extensions)
target_include_directories(native-activity PRIVATE ${LIBS_DIR}/physx/common)

#native glue
target_include_directories(native-activity PRIVATE ${ANDROID_NDK}/sources/android/native_app_glue)

#free image
target_include_directories(native-activity PRIVATE ${LIBS_DIR}/freeimage/)

# add lib dependencies, physx order matters!
target_link_libraries(native-activity
                      android
                      app-glue
                      EGL
                      GLESv2
                      glm
                      log
                      free_image
                      physx_extensions
                      physx
                      physx_foundation
                      physx_task
                      physx_common
                      )


