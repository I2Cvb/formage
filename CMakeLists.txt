# Define project name
project("Formatges")

# cmake needs this line
cmake_minimum_required(VERSION 3.3)

# Load CMake configuration flags. Those can be modified using `ccmake`
include(src/cmake/options.cmake REQUIRED)

# Load Compiler Flags and options
# include(src/cmake/compilerDefOpt.cmake REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fpermissive")

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

set(REMOVE_BACKGROUND r)
add_executable(${REMOVE_BACKGROUND} ./src/apps/remove_background.cpp)
target_link_libraries(${REMOVE_BACKGROUND} ${APP_DEPS})

set(DUMMY d)
add_executable(${DUMMY} ./src/apps/dummy.cpp)
target_link_libraries(${DUMMY} ${APP_DEPS})

# verbose cmake variable status
include(src/cmake/verbose_config.cmake)
