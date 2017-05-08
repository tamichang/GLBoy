# build
```
cd build
cmake ../src
make
```

# debug
```
lldb target file（./bin/sample01）
run
```

# Android memo
```
ln -s ~/Projects/OpenGL_C++/GLBoy/andro ./app/src/main/jni/andro
javah -cp /Applications/adt-bundle-mac-x86_64/sdk/platforms/android-23/android.jar:./app/build/intermediates/classes/arm7/debug -d ./app/src/main/jni/andro glboy.AndroPlayer
```
