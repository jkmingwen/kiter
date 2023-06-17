cmake_minimum_required(VERSION 2.8.12)

FIND_PACKAGE (LibXml2 REQUIRED)

FILE(GLOB_RECURSE printers_cpp *.cpp)
FILE(GLOB_RECURSE printers_h   *.h )

SET(printers_sources      ${printers_cpp}   ${printers_h})

ADD_LIBRARY			   (kiter_printers   SHARED  ${printers_sources})
TARGET_LINK_LIBRARIES  (kiter_printers   PRIVATE kiter_commons kiter_models  ${LIBXML2_LIBRARIES})
TARGET_INCLUDE_DIRECTORIES(kiter_printers PRIVATE ${KITER_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR})

