
project("Formatges")
cmake_minimum_required(VERSION 3.3)

set(EXEC_NAME main)
add_executable(${EXEC_NAME} ./src/main.cpp)

find_package(OpenCV REQUIRED)
set(
    APP_DEPS
    ${OpenCV_LIBS}
    )

target_link_libraries(${EXEC_NAME} ${APP_DEPS})








add_executable(gtest_test ./src/test_test.cpp)


find_package(GTest REQUIRED)

set(
    TESTS_DEPS
    ${GTEST_BOTH_LIBRARIES}
    )

target_link_libraries(gtest_test ${TESTS_DEPS})
