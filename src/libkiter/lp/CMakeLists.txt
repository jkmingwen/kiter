cmake_minimum_required(VERSION 2.8.12)
 
FIND_PACKAGE( GLPK   REQUIRED)
FIND_PACKAGE( GUROBI)

#
FILE(GLOB_RECURSE lp_cpp *.cpp)
FILE(GLOB_RECURSE lp_h   *.h )

SET(lp_sources      ${lp_cpp}   ${lp_h})

ADD_LIBRARY		        (kiter_lp SHARED  ${lp_sources})
TARGET_LINK_LIBRARIES   (kiter_lp PRIVATE  kiter_commons)
target_include_directories(kiter_lp PRIVATE ${KITER_INCLUDE_DIR})


if(GLPK_FOUND)
    message(STATUS "GLPK FOUND")
    target_include_directories(kiter_lp PRIVATE ${GLPK_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES     (kiter_lp  PRIVATE ${GLPK_LIBRARIES})
endif(GLPK_FOUND)


if(GUROBI_FOUND)
    message(STATUS "GUROBI FOUND")
    
	ADD_LIBRARY		          (kiter_gurobi_lp STATIC  ${lp_sources})
	TARGET_LINK_LIBRARIES     (kiter_gurobi_lp PRIVATE   kiter_commons )
    target_include_directories(kiter_gurobi_lp PRIVATE ${KITER_INCLUDE_DIR} ${GUROBI_INCLUDE_DIRS})

	if(GLPK_FOUND)
  	  message(STATUS "GLPK FOUND")
  	  target_include_directories(kiter_gurobi_lp PRIVATE ${KITER_INCLUDE_DIR} ${GLPK_INCLUDE_DIR})
   	 TARGET_LINK_LIBRARIES   (kiter_gurobi_lp  PRIVATE   kiter_commons     ${GLPK_LIBRARIES} ${GUROBI_LIBRARIES})
   	 else()
    TARGET_LINK_LIBRARIES(kiter_gurobi_lp kiter_commons ${GUROBI_LIBRARIES} )
   	 
	endif(GLPK_FOUND)


    target_compile_definitions(kiter_gurobi_lp PRIVATE __GUROBILIB__=1)

endif(GUROBI_FOUND)
