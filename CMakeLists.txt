
project("Formatges")
cmake_minimum_required(VERSION 3.3)

#compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#targets
set(EXEC_NAME main)
add_executable(${EXEC_NAME} ./src/main.cpp)

find_package(OpenCV REQUIRED)
find_package(GTest REQUIRED)

set(
    APP_DEPS
    ${OpenCV_LIBS}
    )

target_link_libraries(${EXEC_NAME} ${APP_DEPS})


add_executable(gtest_test ./src/test_test.cpp)

set(
    TESTS_DEPS
    ${APP_DEPS}
    ${GTEST_INCLUDE_DIRS}
    )

target_link_libraries(gtest_test ${TESTS_DEPS})
