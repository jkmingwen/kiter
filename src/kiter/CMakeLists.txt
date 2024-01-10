cmake_minimum_required(VERSION 2.8.12)


ADD_EXECUTABLE         (kiter  kiter.cpp)
TARGET_LINK_LIBRARIES  (kiter  PRIVATE ${KITER_LIBRARIES} -lz)
TARGET_INCLUDE_DIRECTORIES(kiter  PRIVATE ${KITER_INCLUDE_DIR} )
INSTALL(TARGETS kiter DESTINATION bin)

ADD_EXECUTABLE         (dataset_generator  dataset_generator.cpp)
TARGET_LINK_LIBRARIES  (dataset_generator  PRIVATE ${KITER_LIBRARIES} -lz)
TARGET_INCLUDE_DIRECTORIES(dataset_generator  PRIVATE ${KITER_INCLUDE_DIR} )
INSTALL(TARGETS dataset_generator DESTINATION bin)
