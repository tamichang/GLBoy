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

include_directories(/usr/local/include)

#静的ライブラリの生成
ADD_LIBRARY(glboy STATIC glboy.cpp Object.cpp Color.cpp Shader.cpp SimpleLightShader.cpp SimpleTextureShader.cpp texture.cpp DefaultColorShader.cpp)

INSTALL(TARGETS glboy ARCHIVE DESTINATION lib)

#静的ライブラリ用ヘッダーファイルのインストール指定
INSTALL(FILES glboy.hpp DESTINATION include)