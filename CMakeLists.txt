cmake_minimum_required(VERSION 2.6)
project(svm_test)


# The library prefix
SET(LIB_PREFIX _svm_test)

set(CMAKE_CXX_FLAGS "-g -Wall")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${svm_test_SOURCE_DIR}/bin)

ADD_LIBRARY(svm_lib
	svm.h svm.cpp
)
ADD_EXECUTABLE(svm_test svm_test.cpp)

SET_TARGET_PROPERTIES(svm_lib PROPERTIES OUTPUT_NAME ${LIB_PREFIX}_svm_lib)
SET_TARGET_PROPERTIES(svm_test PROPERTIES OUTPUT_NAME svm_test)

# link the library to the executable
TARGET_LINK_LIBRARIES(svm_test svm_lib)