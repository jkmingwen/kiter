cmake_minimum_required(VERSION 2.6)
 
FIND_PACKAGE( GLPK    REQUIRED )
#FIND_PACKAGE( GUROBI )

#
#  ######### COMPILE FLAGS ##########
#
# -Wno-float-equal -Wno-switch-default ==> temp fix for Coin headers
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -Wconversion -Wno-float-equal -Wno-switch-default")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wno-float-equal -Wno-switch-default")

#
#  ######### INCLUDE PART ##########
#
INCLUDE_DIRECTORIES(${KITER_INCLUDE_DIR})
INCLUDE_DIRECTORIES (${LIBXML2_INCLUDE_DIR})
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIR})

#
#  ######### GENERAL SOURCES ##########
#

MESSAGE(STATUS "Generate libkiter source files")

FILE(GLOB_RECURSE commons_cpp commons/*.cpp)
FILE(GLOB_RECURSE commons_h   commons/*.h )

FILE(GLOB_RECURSE lp_cpp lp/*.cpp)
FILE(GLOB_RECURSE lp_h   lp/*.h )

FILE(GLOB_RECURSE models_cpp models/*.cpp)
FILE(GLOB_RECURSE models_h   models/*.h )

FILE(GLOB_RECURSE printers_cpp printers/*.cpp)
FILE(GLOB_RECURSE printers_h   printers/*.h )

FILE(GLOB_RECURSE generators_cpp generators/*.cpp)
FILE(GLOB_RECURSE generators_h   generators/*.h )

FILE(GLOB_RECURSE algorithms_cpp algorithms/*.cpp algorithms/*/*.cpp)
FILE(GLOB_RECURSE algorithms_h   algorithms/*.h )

SET(generators_sources      ${generators_cpp}   ${generators_h})
SET(commons_sources      ${commons_cpp}   ${commons_h})
SET(models_sources      ${models_cpp}   ${models_h})
SET(printers_sources      ${printers_cpp}   ${printers_h})
SET(algorithms_sources      ${algorithms_cpp}   ${algorithms_h})
SET(lp_sources      ${lp_cpp}   ${lp_h})


#
#  ######### TARGETS PART ##########
#


#          ######### GENERAL LIBS ##########

SET(LIBKITER_FLAGS "-Wconversion -Wfloat-equal  ${CMAKE_CXX_FLAGS_DEBUG} ")


#          ######### API         ##########

ADD_LIBRARY			   (kiter_commons SHARED  ${commons_sources})
TARGET_LINK_LIBRARIES              (kiter_commons PUBLIC  ${LIBXML2_LIBRARIES})

ADD_LIBRARY			   (kiter_models SHARED   ${models_sources})
TARGET_LINK_LIBRARIES              (kiter_models PUBLIC   kiter_commons )


ADD_LIBRARY		(kiter_lp SHARED  ${lp_sources})
TARGET_LINK_LIBRARIES   (kiter_lp PUBLIC   kiter_commons )

if(GLPK_FOUND)
    message(STATUS "GLPK FOUND")
    target_include_directories(kiter_lp PRIVATE ${GLPK_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES   (kiter_lp         ${GLPK_LIBRARIES})
endif(GLPK_FOUND)

if(GUROBI_FOUND)
    message(STATUS "GUROBI FOUND")
    target_include_directories(kiter_lp PRIVATE ${GUROBI_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(kiter_lp ${GUROBI_LIBRARIES} )
    target_compile_definitions(kiter_lp PRIVATE __GUROBILIB__=1)
endif(GUROBI_FOUND)




ADD_LIBRARY			   (kiter_generators SHARED ${generators_sources})
TARGET_LINK_LIBRARIES  (kiter_generators PUBLIC kiter_commons  kiter_models kiter_algorithms)

ADD_LIBRARY			   (kiter_printers   SHARED  ${printers_sources})
TARGET_LINK_LIBRARIES  (kiter_printers   PUBLIC kiter_commons kiter_models)


ADD_LIBRARY			   (kiter_algorithms SHARED  ${algorithms_sources})
TARGET_LINK_LIBRARIES  (kiter_algorithms PUBLIC kiter_commons kiter_models kiter_printers kiter_lp)


file(WRITE null.cpp "")
ADD_LIBRARY			   (libkiter SHARED   null.cpp )
TARGET_LINK_LIBRARIES  (libkiter PUBLIC
				kiter_commons 
				kiter_lp 
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
