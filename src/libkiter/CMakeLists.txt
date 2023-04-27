cmake_minimum_required(VERSION 2.8.12)

SET(LIBKITER_FLAGS "-Wconversion -Wfloat-equal  ${CMAKE_CXX_FLAGS_DEBUG} ")

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/algorithms algorithms)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/commons commons)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/generators generators)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/lp lp)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/ml ml)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/models models)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/printers printers)


file(WRITE null.cpp "")
ADD_LIBRARY			   (libkiter SHARED   null.cpp )

TARGET_LINK_LIBRARIES  (libkiter PUBLIC
		kiter_commons
		kiter_lp
		kiter_ml
		kiter_models
		kiter_printers
		kiter_algorithms
		kiter_generators)

set_property(TARGET libkiter PROPERTY FRAMEWORK ON)

INSTALL(TARGETS libkiter
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    FRAMEWORK DESTINATION "Frameworks"
    PUBLIC_HEADER DESTINATION include/
    )
