ビルド方法
cd build
cmake ../src
make

デバッグ方法
lldb 対象の実行ファイル（./bin/sample01）
run



Android memo
ln -s ~/Projects/OpenGL_C++/GLBoy/andro ./app/src/main/jni/andro
javah -cp /Applications/adt-bundle-mac-x86_64/sdk/platforms/android-23/android.jar:./app/build/intermediates/classes/arm7/debug -d ./app/src/main/jni/andro glboy.AndroPlayer