# Define project name
project("Formatges")

# cmake needs this line
cmake_minimum_required(VERSION 3.3)

# Load CMake configuration flags. Those can be modified using `ccmake`
include(src/cmake/options.cmake REQUIRED)

# Load Compiler Flags and options
# include(src/cmake/compilerDefOpt.cmake REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -fpermissive")

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

set(REMOVE_BACKGROUND r)
add_executable(${REMOVE_BACKGROUND} ./src/apps/remove_background.cpp)
target_link_libraries(${REMOVE_BACKGROUND} ${APP_DEPS})
