cmake_minimum_required(VERSION 2.8.12)

#FIND_PACKAGE( cppflow )

FILE(GLOB_RECURSE ml_cpp *.cpp */*.cpp)
FILE(GLOB_RECURSE ml_h   *.h )

SET(ml_sources      ${ml_cpp}   ${ml_h})

IF(cppflow_FOUND)
	ADD_LIBRARY			   (kiter_ml SHARED  ${ml_sources})
	TARGET_LINK_LIBRARIES  (kiter_ml PRIVATE kiter_commons kiter_models cppflow::cppflow)
	TARGET_INCLUDE_DIRECTORIES(kiter_ml PRIVATE ${KITER_INCLUDE_DIR}  ${CPPFLOW_INCLUDE_DIR})
	TARGET_COMPILE_DEFINITIONS(kiter_ml PRIVATE CPPFLOW_FOUND)
ELSE()
	ADD_LIBRARY			   (kiter_ml SHARED  ${ml_sources})
	TARGET_LINK_LIBRARIES  (kiter_ml PRIVATE kiter_commons kiter_models)
	TARGET_INCLUDE_DIRECTORIES(kiter_ml PRIVATE ${KITER_INCLUDE_DIR})
ENDIF(cppflow_FOUND)

