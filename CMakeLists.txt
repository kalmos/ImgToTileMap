###
# #
###

# defini la version minimale de cmake necessaire
cmake_minimum_required(VERSION 2.6.0)

# inclusion des parametres persos
include ("${CMAKE_SOURCE_DIR}/config.cmake")

set(PROJECT_SOURCE_DIR "./")
set(PROJECT_NAME ImgToTileMap)
project(${PROJECT_NAME} CXX C)

include(${CMAKE_SOURCE_DIR}/Rule/CMakeLists.txt)