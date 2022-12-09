cmake_minimum_required(VERSION 3.5)
project(ZED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZED 3 REQUIRED)
find_package(CUDA ${ZED_CUDA_VERSION} REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(include)

add_subdirectory(mex)

