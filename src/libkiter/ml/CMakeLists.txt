cmake_minimum_required(VERSION 2.8.12)

FILE(GLOB_RECURSE ml_cpp *.cpp */*.cpp)
FILE(GLOB_RECURSE ml_h   *.h )

SET(ml_sources      ${ml_cpp}   ${ml_h})

ADD_LIBRARY			   (kiter_ml SHARED  ${ml_sources})
TARGET_LINK_LIBRARIES  (kiter_ml PRIVATE kiter_commons kiter_models)
TARGET_INCLUDE_DIRECTORIES(kiter_ml PRIVATE ${KITER_INCLUDE_DIR})

