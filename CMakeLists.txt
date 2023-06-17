cmake_minimum_required(VERSION 3.0)
project(Kiter)
enable_testing()

#
#  ######### GENERATOR PART ##########
#

IF (NOT CMAKE_GENERATOR)
    MESSAGE(STATUS "Generate Unix Makefile by defaut !")
    SET(CMAKE_GENERATOR "Unix Makefiles" CACHE PATH "generator")
ENDIF(NOT CMAKE_GENERATOR)

#
#  ######### BUILDTYPE PART ##########
#

# Only Debug, or Release mode by default !
IF(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    SET (CMAKE_BUILD_TYPE Release)
ENDIF()

IF(CMAKE_BUILD_TYPE MATCHES Debug)
add_definitions( -D__DEBUG_MODE__ )
ENDIF()

IF(CMAKE_BUILD_TYPE MATCHES Release)
add_definitions( -D__RELEASE_MODE__ )
ENDIF()

#
#  ######### MODULES PART ##########
#

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules/")


#
#  ######### C++ FLAGS ##########
#


# Clear all flags
SET(CMAKE_CXX_FLAGS "")
SET(CMAKE_CXX_FLAGS_DEBUG "")
SET(CMAKE_CXX_FLAGS_RELEASE "")

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")

  # errors checking in all mode
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic-errors -ansi  -Wextra    -Wno-deprecated -Wno-deprecated-declarations   ")

  # some errors checking in debug mode
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -pedantic-errors -ansi  -Wextra   -Wunused  -Wno-error=unused-result -Wno-unused-but-set-variable ")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wformat=2 -Winit-self -Wmissing-include-dirs -Wswitch-default    -Wswitch-enum       -Wno-endif-labels ")
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wcast-qual    -Wcast-align      -Wno-packed   -Wmissing-format-attribute -Wredundant-decls")

  # fix 64bit warning failure
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wno-long-long")

  # not work with Qt
  #SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wconversion -Wfloat-equal ")
  # not work on CentOS
  #SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}   -Wlogical-op    -Wundef  ")
  # not work in fc10
  #SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wlarger-than=1024   -Wsync-nand ")
  # don't add profiling helper
  #SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DGLIBCXX_FORCE_NEW")
  # DON'TRAP OVERFLOW
  # SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ftrapv")
  
endif()

# -g in debug mode
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -coverage -std=c++17 -fno-inline  -fno-default-inline -Wextra -fno-inline-small-functions")
# -fno-inline  -fno-default-inline -Wextra -fno-inline-small-functions

# -O3 in normal mode
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -std=c++17")
#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fno-signed-zeros -mtune=native -fno-signaling-nans -fno-trapping-math -fno-math-errno")


#
#  ######### OUTPUT DEF  ##########
#
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/)
set(LIBRARY_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/lib/)
set(INCLUDE_OUTPUT_PATH    ${PROJECT_BINARY_DIR}/include/)


#
#  ######### SUBDIR PART ##########
#

SET(KITER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/libkiter)
#SET(KITER_LIBRARIES kiter_generators kiter_algorithms kiter_models kiter_ml kiter_lp kiter_printers kiter_commons)
SET(KITER_LIBRARIES libkiter)


ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/src/ build_results/src)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/tests/ build_results/tests)

