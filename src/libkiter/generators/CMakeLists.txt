cmake_minimum_required(VERSION 2.8.12)
#
#  ######### COMPILE FLAGS ##########
#
# -Wno-float-equal -Wno-switch-default ==> temp fix for Coin headers
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -Wconversion -Wno-float-equal -Wno-switch-default")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wno-float-equal -Wno-switch-default")

#
#  ######### GENERAL SOURCES ##########
#

FILE(GLOB_RECURSE generators_cpp *.cpp)
FILE(GLOB_RECURSE generators_h   *.h )

SET(generators_sources      ${generators_cpp}   ${generators_h})


ADD_LIBRARY			   (kiter_generators SHARED ${generators_sources})
TARGET_LINK_LIBRARIES  (kiter_generators PRIVATE kiter_commons  kiter_models kiter_algorithms)
TARGET_INCLUDE_DIRECTORIES(kiter_generators PRIVATE ${KITER_INCLUDE_DIR})