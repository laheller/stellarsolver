#! /bin/bash

# Check all the below lines and adjust all the paths starting with "${HOME}/" to the correct values

NDK_PATH=${HOME}/android-ndk-r26c
TOOLCHAIN="clang"
ANDROID_VERSION="23"

cmake -G"Unix Makefiles" \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-${ANDROID_VERSION} \
    -DANDROID_TOOLCHAIN=${TOOLCHAIN} \
    -DCMAKE_ASM_FLAGS="--target=aarch64-linux-android${ANDROID_VERSION}" \
    -DCMAKE_TOOLCHAIN_FILE=${NDK_PATH}/build/cmake/android.toolchain.cmake \
    -DCMAKE_INSTALL_PREFIX=$(pwd)/../_deploy_aarch64 \
    -DCMAKE_C_FLAGS="-std=gnu17 -fPIE -fPIC" \
    -DCMAKE_CXX_FLAGS="-std=gnu++17 -fPIE -fPIC" \
    -DRUN_RESULT_2="" \
    -DQt5_DIR=${HOME}/Qt5.14.2/5.14.2/android/lib/cmake/Qt5 \
    -DQt5Gui_DIR=${HOME}/Qt5.14.2/5.14.2/android/lib/cmake/Qt5Gui \
    -DQt5Core_DIR=${HOME}/Qt5.14.2/5.14.2/android/lib/cmake/Qt5Core \
    -DQt5Widgets_DIR=${HOME}/Qt5.14.2/5.14.2/android/lib/cmake/Qt5Widgets \
    -DQt5Concurrent_DIR=${HOME}/Qt5.14.2/5.14.2/android/lib/cmake/Qt5Concurrent \
    -DQt5Network_DIR=${HOME}/Qt5.14.2/5.14.2/android/lib/cmake/Qt5Network \
    -DGSL_INCLUDE_DIR=${HOME}/repos/gsl/_deploy_aarch64/include \
    -DGSL_LIBRARY=${HOME}/repos/gsl/_deploy_aarch64/lib/libgsl.so \
    -DGSL_CBLAS_LIBRARY=${HOME}/repos/gsl/_deploy_aarch64/lib/libgslcblas.so \
    -DWCSLIB_INCLUDE_DIR=${HOME}/repos/wcslib-7.7/_deploy_aarch64/include/wcslib \
    -DWCSLIB_LIBRARIES=${HOME}/repos/wcslib-7.7/_deploy_aarch64/lib/libwcs.so \
    -DCFITSIO_INCLUDE_DIR=${HOME}/repos/cfitsio-4.0.0/_deploy_aarch64/include \
    -DCFITSIO_LIBRARIES=${HOME}/repos/cfitsio-4.0.0/_deploy_aarch64/lib/libcfitsio.so \
    -DBUILD_TESTS=1 \
    -DBUILD_DEMOS=1 \
    ..

echo -e "\nBefore calling make, edit the file\n$(pwd)/../stellarsolver/astrometry/include/astrometry/os-features-config.h\n"
echo "Change the line:"
echo "#define NEED_QSORT_R 0"
echo -e "\nto:"
echo "#define NEED_QSORT_R 1"

#make | ccze -A
#make install | ccze -A
