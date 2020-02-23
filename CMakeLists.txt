cmake_minimum_required(VERSION 3.15)
project(ImageProduce)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include/opencv4/opencv2)
set(OpenCV_LIBS /usr/local/lib)
find_package(OpenCV REQUIRED)
add_executable(ImageProduce main.cpp)
target_link_libraries(ImageProduce ${OpenCV_LIBS})