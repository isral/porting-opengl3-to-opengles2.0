@echo off
set NDK_PATH=C:\Users\tomcz\AppData\Local\Android\Sdk\ndk\23.1.7779620
set BUILD_TOOLS_PATH=C:\Users\tomcz\AppData\Local\Android\Sdk/build-tools/32.1.0-rc1
set ANDROID_PATH=C:\Users\tomcz\AppData\Local\Android\Sdk/platforms/android-28
set INCLUDE_DIR=C:/code/libs/include

set APP_NAME=App

mkdir build\makecapk\lib\arm64-v8a
call %NDK_PATH%\toolchains\llvm\prebuilt\windows-x86_64\bin\aarch64-linux-android28-clang -Os -DGLEW_NO_GLU -DANDROID -DAPPNAME=\"%APP_NAME%\" -I%INCLUDE_DIR% -I%NDK_PATH%/sources/android/native_app_glue -I%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include -I%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/android  -fPIC -DANDROIDVERSION=28 -m64 -o build/makecapk/lib/arm64-v8a/libApp.so %NDK_PATH%/sources/android/native_app_glue/android_native_app_glue.c extern/glfm/glfm_platform_android.c main.c -L%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/lib/aarch64-linux-android/28 -Wl,--gc-sections -s -lm -lGLESv3 -lEGL -landroid -llog -lOpenSLES -lz -shared -uANativeActivity_onCreate

mkdir build\makecapk\lib\armeabi-v7a
call %NDK_PATH%\toolchains\llvm\prebuilt\windows-x86_64\bin\armv7a-linux-androideabi28-clang -Os -DGLEW_NO_GLU -DANDROID -DAPPNAME=\"%APP_NAME%\" -I%INCLUDE_DIR% -I%NDK_PATH%/sources/android/native_app_glue -I%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include -I%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/android  -fPIC -DANDROIDVERSION=28 -mfloat-abi=softfp -m32 -o build/makecapk/lib/armeabi-v7a/libApp.so %NDK_PATH%/sources/android/native_app_glue/android_native_app_glue.c extern/glfm/glfm_platform_android.c main.c -L%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/lib/arm-linux-androideabi/28 -Wl,--gc-sections -s -lm -lGLESv3 -lEGL -landroid -llog -lOpenSLES -lz -shared -uANativeActivity_onCreate

mkdir build\makecapk\lib\x86
call %NDK_PATH%\toolchains\llvm\prebuilt\windows-x86_64\bin\i686-linux-android28-clang -Os -DGLEW_NO_GLU -DANDROID -DAPPNAME=\"%APP_NAME%\" -I%INCLUDE_DIR% -I%NDK_PATH%/sources/android/native_app_glue -I%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include -I%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/android  -fPIC -DANDROIDVERSION=28 -march=i686 -mtune=x86-64 -mssse3 -mfpmath=sse -m32 -o build/makecapk/lib/x86/libApp.so %NDK_PATH%/sources/android/native_app_glue/android_native_app_glue.c extern/glfm/glfm_platform_android.c main.c -L%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/lib/i686-linux-android/28 -Wl,--gc-sections -s -lm -lGLESv3 -lEGL -landroid -llog -lOpenSLES -lz -shared -uANativeActivity_onCreate

mkdir build\makecapk\lib\x86_64
call %NDK_PATH%\toolchains\llvm\prebuilt\windows-x86_64\bin\i686-linux-android28-clang -Os -DGLEW_NO_GLU -DANDROID -DAPPNAME=\"%APP_NAME%\" -I%INCLUDE_DIR% -I%NDK_PATH%/sources/android/native_app_glue -I%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include -I%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/include/android  -fPIC -DANDROIDVERSION=28 -march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=x86-64 -o build/makecapk/lib/x86_64/libApp.so %NDK_PATH%/sources/android/native_app_glue/android_native_app_glue.c extern/glfm/glfm_platform_android.c main.c -L%NDK_PATH%/toolchains/llvm/prebuilt/windows-x86_64/sysroot/usr/lib/x86_64-linux-android/28 -Wl,--gc-sections -s -lm -lGLESv3 -lEGL -landroid -llog -lOpenSLES -lz -shared -uANativeActivity_onCreate

mkdir build\makecapk\assets
cp -r assets/* build/makecapk/assets
rm -rf build/temp.apk
%BUILD_TOOLS_PATH%/aapt package -f -F build/temp.apk -I %ANDROID_PATH%/android.jar -M AndroidManifest.xml -S res -A build/makecapk/assets -v --min-sdk-version 28 --target-sdk-version 28
unzip -o build/temp.apk -d build/makecapk
rm -rf build/makecapk.apk

pushd build\makecapk
zip -D9r ../makecapk.apk . && zip -D0r ../makecapk.apk ./resources.arsc ./AndroidManifest.xml
popd

jarsigner -sigalg SHA1withRSA -digestalg SHA1 -verbose -keystore my-release-key.keystore -storepass password build/makecapk.apk standkey
rm -rf build/%APP_NAME%.apk
%BUILD_TOOLS_PATH%/zipalign -v 4 build/makecapk.apk build/%APP_NAME%.apk
@REM #Using the apksigner in this way is only required on Android 30+
%BUILD_TOOLS_PATH%/apksigner sign --key-pass pass:password --ks-pass pass:password --ks my-release-key.keystore build/%APP_NAME%.apk
rm -rf build/temp.apk
rm -rf build/makecapk.apk
