
project("Formatges")
cmake_minimum_required(VERSION 3.3)

set(EXEC_NAME main)
add_executable(${EXEC_NAME} ./src/main.cpp)

find_package(OpenCV REQUIRED)
target_link_libraries(${EXEC_NAME} ${OpenCV_LIBS})
