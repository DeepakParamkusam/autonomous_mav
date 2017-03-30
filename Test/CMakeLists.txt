cmake_minimum_required(VERSION 2.8)
project(test_opencv)
find_package(OpenCV REQUIRED)

#add_executable(test src/test.cpp)
#target_link_libraries(test ${OpenCV_LIBS})

add_executable(location src/location.cpp)
target_link_libraries(location ${OpenCV_LIBS})
