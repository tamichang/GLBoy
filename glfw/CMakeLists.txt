#CMakeLists.txt

#要求するcmakeのバージョン（ここで指定したのより古いバージョンのcmakeではエラー）
cmake_minimum_required(VERSION 3.0)

#プロジェクト名
PROJECT(glboyfwplayer)

add_definitions("-g -Wall -std=c++11")
include_directories(../)
include_directories(/usr/local/include)

#静的ライブラリの生成
ADD_LIBRARY(glboyfwplayer STATIC fwplayer.cpp)

INSTALL(TARGETS glboyfwplayer ARCHIVE DESTINATION lib)

#静的ライブラリ用ヘッダーファイルのインストール指定
INSTALL(FILES FWPlayer.hpp DESTINATION include/GLBoy)
