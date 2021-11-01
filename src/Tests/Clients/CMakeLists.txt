cmake_minimum_required(VERSION 3.16)
project(Test_Opencv)
# SET(CMAKE_BUILD_TYPE Release)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_FIND_LIBRARY_SUFFIXES "${CAMKE_STATIC_LIBRARY_SUFFIX}")
# set(BUILD_SHARED_LIBS 0)
# set(OpenCV_STATIC ON)

# set( CMAKE_VERBOSE_MAKEFILE on )
find_package(OpenCV REQUIRED)

# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories("${OpenCV_INCLUDE_DIRS}")
add_executable(Test_Opencv client_Ricky.cpp)
target_link_libraries(Test_Opencv "${OpenCV_LIBS}")

#g++ -L /home/username/Documents/BoxerUI/opencv-build/install/lib/ -lopencv_core -lopencv_videoio  -lopencv_highgui -I /home/username/Documents/BoxerUI/opencv-build/install/include/opencv4/ 'pkg-config --static --libs opencv' -o  "main"  Test_Opencv.cpp
# g++ -I /home/username/Documents/BoxerUI/opencv-build/install/include/opencv4/ Test_Opencv.cpp  -L /home/username/Documents/BoxerUI/opencv-build/install/lib/  -lopencv_core -lopencv_videoio  -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc 
