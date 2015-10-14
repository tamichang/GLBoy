#CMakeLists.txt

#要求するcmakeのバージョン（ここで指定したのより古いバージョンのcmakeではエラー）
cmake_minimum_required(VERSION 3.0)

#プロジェクト名
PROJECT(glfw)

add_definitions("-g -Wall -std=c++11")
include_directories(../glboy)
include_directories(/usr/local/include)

#静的ライブラリの生成
ADD_LIBRARY(glfw STATIC fwplayer.cpp)

INSTALL(TARGETS glfw ARCHIVE DESTINATION lib)

#静的ライブラリ用ヘッダーファイルのインストール指定
INSTALL(FILES fwplayer.hpp DESTINATION include)