#CMakeLists.txt

#要求するcmakeのバージョン（ここで指定したのより古いバージョンのcmakeではエラー）
cmake_minimum_required(VERSION 3.0)

#プロジェクト名
PROJECT(glboy_glfw)

#add_executable(sample01 sample01.cpp fwplayer.cpp)
#target_link_libraries(sample01
#	${ALL_LIBS}
#)

include_directories(/usr/local/include)

#静的ライブラリの生成
ADD_LIBRARY(glboy_glfw STATIC fwplayer.cpp)

INSTALL(TARGETS glboy_glfw ARCHIVE DESTINATION lib)

#静的ライブラリ用ヘッダーファイルのインストール指定
INSTALL(FILES glboy.hpp DESTINATION include)