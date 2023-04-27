cmake_minimum_required(VERSION 2.8.12)


#
#  ######### SOURCES ##########
#

SET ( kiter_sources  main.cpp )

#  ######### EXEC ##########

ADD_EXECUTABLE         (kiter  ${kiter_sources})
TARGET_LINK_LIBRARIES  (kiter  PRIVATE ${KITER_LIBRARIES} -lz)
TARGET_INCLUDE_DIRECTORIES(kiter  PRIVATE ${KITER_INCLUDE_DIR} )
INSTALL(TARGETS kiter DESTINATION bin)
