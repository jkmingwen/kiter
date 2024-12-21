cmake_minimum_required(VERSION 3.5)

FILE(GLOB_RECURSE commons_cpp *.cpp)
FILE(GLOB_RECURSE commons_h   *.h )
SET(commons_sources      ${commons_cpp}   ${commons_h})


#          ######### API         ##########

ADD_LIBRARY			   (kiter_commons SHARED  ${commons_sources})
TARGET_INCLUDE_DIRECTORIES(kiter_commons PRIVATE ${KITER_INCLUDE_DIR})
