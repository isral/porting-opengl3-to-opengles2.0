@echo "Installing" App.apk
adb install -r build/App.apk
adb shell am start -n com.example.App/android.app.NativeActivity