# Define project name
project("Formatges" CXX)

# cmake needs this line
cmake_minimum_required(VERSION 3.3)
# Load CMake configuration flags. Those can be modified using `ccmake`
include(src/cmake/options.cmake REQUIRED)

# Load Compiler Flags and options
set(CMAKE_BUILD_TYPE "Debug")
include(src/cmake/compilerDefOpt.cmake REQUIRED)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Load dependencies and their configuration
include(src/cmake/findDependencies.cmake REQUIRED)

# Add project libraries
add_subdirectory(src/common/)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/src/")

#targets
set(EXEC_NAME main)
add_executable(${EXEC_NAME} ./src/main.cpp)


set(APP_DEPS
    common
    )

target_link_libraries(${EXEC_NAME} common)

set(IMAGE_PROCESSING image_processing)
add_executable(${IMAGE_PROCESSING} ./src/apps/image_processing.cpp)
target_link_libraries(${IMAGE_PROCESSING} ${APP_DEPS})

set(EXAMPLE example)
add_executable(${EXAMPLE} ./src/apps/example.cpp)
target_link_libraries(${EXAMPLE} ${APP_DEPS})

set(EXAMPLE_ALL example_all)
add_executable(${EXAMPLE_ALL} ./src/apps/example_all.cpp)
target_link_libraries(${EXAMPLE_ALL} ${APP_DEPS})

add_executable(gtest_test ./src/test_test.cpp)

set(
    TESTS_DEPS
    ${APP_DEPS}
    ${GTEST_LIBRARIES}
    )

target_link_libraries(gtest_test ${TESTS_DEPS})

# verbose cmake variable status
include(src/cmake/verbose_config.cmake)
