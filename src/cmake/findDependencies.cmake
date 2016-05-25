## Find OpenCV,
#
# you may need to set OpenCV_DIR variable to the absolute path to the directory
# containing OpenCVConfig.cmake file via the command line or GUI
#find_package(OpenCV REQUIRED)
find_package(OpenCV 3.1.0 EXACT REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")


  # Add OpenCV headers location to your include paths
#include_directories(${OpenCV_INCLUDE_DIRS})


## Find Google Test
#
find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
