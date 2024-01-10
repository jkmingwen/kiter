cmake_minimum_required(VERSION 2.8.12)

FIND_PACKAGE( Boost 1.69.0 COMPONENTS  graph  system  filesystem REQUIRED)

FILE(GLOB_RECURSE algorithms_cpp *.cpp */*.cpp)
FILE(GLOB_RECURSE algorithms_h   *.h )

SET(algorithms_sources      ${algorithms_cpp}   ${algorithms_h})


ADD_LIBRARY			   (kiter_algorithms SHARED  ${algorithms_sources})
TARGET_LINK_LIBRARIES  (kiter_algorithms PRIVATE kiter_commons kiter_models kiter_printers kiter_lp
							${Boost_FILESYSTEM_LIBRARY} 
 							${Boost_SYSTEM_LIBRARY})
TARGET_INCLUDE_DIRECTORIES(kiter_algorithms PRIVATE ${KITER_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
