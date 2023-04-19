cmake_minimum_required(VERSION 2.8.12)

find_package(cppflow REQUIRED)
if(cppflow_FOUND)
    # Add the sources of your class and header file
    add_library(minimizing_prediction minimizing_prediction.cpp minimizing_prediction.h)

    # Link the dependencies to your class
    target_link_libraries(minimizing_prediction PRIVATE cppflow::cppflow)
    target_compile_definitions(minimizing_prediction PUBLIC
            MODEL_PATH="${CMAKE_CURRENT_SOURCE_DIR}/model")
endif()
