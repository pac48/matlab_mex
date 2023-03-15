project(matlab_mex)
cmake_minimum_required(VERSION 3.18.2)

SET(CMAKE_BUILD_TYPE Release)
#SET(MATLAB_ROOT ${MATLAB_ROOT} CACHE FILEPATH "directory")
#set(MATLAB_ROOT /media/paul/MATLAB2022b/MATLAB/R2022b)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_definitions(/DMATLAB_MEX_FILE)
#add_definitions(/DMX_COMPAT_32)

find_package(Matlab REQUIRED)

IF(MATLAB_FOUND)
    message(STATUS "MATLAB Found, MATLAB MEX will be compiled.")
    add_subdirectory(robotMex)
    add_subdirectory(objLoaderMex)
    add_subdirectory(parser)
    add_subdirectory(ZED)
    add_subdirectory(ZMQ)
#    enable_language(CUDA)
    add_subdirectory(tiny_cuda)

ELSE(MATLAB_FOUND)
    message( FATAL_ERROR "MATLAB not found...nothing will be built." )
ENDIF(MATLAB_FOUND)
