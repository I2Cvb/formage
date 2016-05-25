file(GLOB_RECURSE common_sources *.cpp *.cc)
file(GLOB_RECURSE common_headers *.h *.hpp)

add_library(common STATIC
        ${common_sources}
        ${common_headers}
)

target_include_directories(common PUBLIC ${OpenCV_INCLUDE_DIRS})
target_link_libraries(common PUBLIC ${OpenCV_LIBS})
