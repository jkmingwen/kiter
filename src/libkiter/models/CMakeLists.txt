cmake_minimum_required(VERSION 2.8.12)

FIND_PACKAGE( Boost 1.69.0 COMPONENTS  graph  REQUIRED)


FILE(GLOB_RECURSE models_cpp *.cpp)
FILE(GLOB_RECURSE models_h   *.h )

SET(models_sources      ${models_cpp}   ${models_h})

ADD_LIBRARY			   (kiter_models SHARED   ${models_sources})
TARGET_LINK_LIBRARIES  (kiter_models PRIVATE   kiter_commons )
TARGET_INCLUDE_DIRECTORIES(kiter_models PRIVATE ${KITER_INCLUDE_DIR}  ${Boost_INCLUDE_DIR})
