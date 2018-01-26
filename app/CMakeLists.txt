# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# OpenCV stuff
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/distribution/opencv_native/jni/include)
#add_library( lib_opencv SHARED IMPORTED )
#set_target_properties(lib_opencv PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/distribution/opencv_native/libs/${ANDROID_ABI}/libopencv_java3.so)


#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/distribution/FacialLandmarkDetector)
#add_library(FacialLandmarkDetector SHARED IMPORTED )
#set_target_properties(FacialLandmarkDetector PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/distribution/FacialLandmarkDetector/${ANDROID_ABI}/libFacialLandmarkDetector.so)

#huawei lib x 6
add_library(arcamera SHARED IMPORTED )
set_target_properties(arcamera
                      PROPERTIES IMPORTED_LOCATION
                     ${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/${ANDROID_ABI}/libarcamera.so
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/include)

add_library(dfu SHARED IMPORTED )
set_target_properties(dfu
                      PROPERTIES IMPORTED_LOCATION
                     ${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/${ANDROID_ABI}/libdfu.so
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/include)

add_library(HSLPreProcessing SHARED IMPORTED )
set_target_properties(HSLPreProcessing
                      PROPERTIES IMPORTED_LOCATION
                     ${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/${ANDROID_ABI}/libHSLPreProcessing.so
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/include)

add_library(Idepthservice SHARED IMPORTED )
set_target_properties(Idepthservice
                      PROPERTIES IMPORTED_LOCATION
                     ${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/${ANDROID_ABI}/libIdepthservice.so
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/include)

add_library(slaccuracy SHARED IMPORTED )
set_target_properties(slaccuracy
                      PROPERTIES IMPORTED_LOCATION
                     ${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/${ANDROID_ABI}/libslaccuracy.so
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/include)

add_library(usb100 SHARED IMPORTED )
set_target_properties(usb100
                      PROPERTIES IMPORTED_LOCATION
                     ${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/${ANDROID_ABI}/libusb100.so
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/distribution/huaweiAR/include)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
file(GLOB CPP_SOURCES "src/main/cpp/*.cpp")
file(GLOB CPP_INCLUDE "src/main/cpp/*.h")
#file(GLOB FaceTracker_SOURCES "src/main/cpp/FaceTracker/*.cpp")
#file(GLOB FaceTracker_INCLUDE "src/main/cpp/FaceTracker/*.h")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})
add_library( # Sets the name of the library.
             engine
             SHARED
             ${CPP_SOURCES}
             ${CPP_INCLUDE}
)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       engine
                       arcamera
                       dfu
                       HSLPreProcessing
                       Idepthservice
                       slaccuracy
                       usb100
 #                      FacialLandmarkDetector
 #                      lib_opencv
                       GLESv2
                       android
                       EGL
                       OpenSLES
                       jnigraphics
                       z
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )