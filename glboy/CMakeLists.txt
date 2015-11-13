#CMakeLists.txt

#要求するcmakeのバージョン（ここで指定したのより古いバージョンのcmakeではエラー）
cmake_minimum_required(VERSION 3.0)

#プロジェクト名
PROJECT(glboy)

#add_executable(embedfile embedfile.cpp)
#add_custom_command(
#  OUTPUT  ${CMAKE_SOURCE_DIR}/DefaultColorVertexShader.h
#  COMMAND embedfile DefaultColorVertexShader ${CMAKE_SOURCE_DIR}/shaders/DefaultColorVertexShader.vert ${CMAKE_SOURCE_DIR}/DefaultColorVertexShader.h
#  DEPENDS ${CMAKE_SOURCE_DIR}/shaders/DefaultColorVertexShader.vert
#  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

add_definitions("-g -Wall -std=c++11")
include_directories(/usr/local/include)

file(GLOB sources "*.cpp")

#静的ライブラリの生成
ADD_LIBRARY(glboy STATIC ${sources})

INSTALL(TARGETS glboy ARCHIVE DESTINATION lib)

#静的ライブラリ用ヘッダーファイルのインストール指定
INSTALL(DIRECTORY ./ DESTINATION include/GLBoy FILES_MATCHING PATTERN "*.hpp")