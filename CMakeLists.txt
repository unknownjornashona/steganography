cmake_minimum_required(VERSION 3.10)

# Set the project name
project(XrayImageProcessor)

# Find the OpenCV package
find_package(OpenCV REQUIRED)

# Set the source file
set(SOURCES ImageProcessor.cpp)

# Create the executable
add_executable(XrayImageProcessor ${SOURCES})

# Link the OpenCV libraries to the executable
target_link_libraries(XrayImageProcessor ${OpenCV_LIBS})
