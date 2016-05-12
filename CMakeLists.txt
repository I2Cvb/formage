# Define project name
project("Formatges")

# cmake needs this line
cmake_minimum_required(VERSION 3.3)

# Load CMake configuration flags. Those can be modified using `ccmake`
include(src/cmake/options.cmake REQUIRED)

# Load Compiler Flags and options
# include(src/cmake/compilerDefOpt.cmake REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Load dependencies and their configuration
include(src/cmake/findDependencies.cmake REQUIRED)

#targets
set(EXEC_NAME main)
add_executable(${EXEC_NAME} ./src/main.cpp)


set(
    APP_DEPS
    ${OpenCV_LIBS}
    )

target_link_libraries(${EXEC_NAME} ${APP_DEPS})


# add_executable(gtest_test ./src/test_test.cpp)
#
# set(
#     TESTS_DEPS
#     ${APP_DEPS}
#     ${GTEST_INCLUDE_DIRS}
#     )
#
# target_link_libraries(gtest_test ${TESTS_DEPS})

# add apps
set(GRABCUT g_cut)
add_executable(${GRABCUT} ./src/apps/grabcut.cpp)
target_link_libraries(${GRABCUT} ${APP_DEPS})

set(PRE p)
add_executable(${PRE} ./src/apps/prepro.cpp)
target_link_libraries(${PRE} ${APP_DEPS})

