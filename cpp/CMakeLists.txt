project(matlab_mex)
cmake_minimum_required(VERSION 3.21.1)

SET(CMAKE_BUILD_TYPE Release)
if(NOT DEFINED MATLAB_ROOT)
  message(FATAL_ERROR "ERROR: MATLAB_ROOT is not set. Pass in valid a value, e.g. -DMATLAB_ROOT=/media/paul/MATLAB2022b/MATLAB/R2022b")
endif()

if(NOT BUILD_ROBOT 
   AND NOT BUILD_OBJ_LOADER
   AND NOT BUILD_PARSER
   AND NOT BUILD_ZED
   AND NOT BUILD_ZMQ
   AND NOT BUILD_TINY_CUDA
   )
  message(FATAL_ERROR "ERROR: No build values set. Specify values =1, valid options are -DBUILD_ROBOT -DBUILD_OBJ_LOADER -DBUILD_PARSER -DBUILD_ZED -BUILD_ZMQ -DBUILD_TINY_CUDA")
endif()


set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

add_definitions(/DMATLAB_MEX_FILE)
find_package(Matlab REQUIRED)

IF(MATLAB_FOUND)
    message(STATUS "MATLAB Found, MATLAB MEX will be compiled.")
    if(DEFINED BUILD_ROBOT)
    	add_subdirectory(robotMex)
    endif()
    if(DEFINED BUILD_OBJ_LOADER)
    	add_subdirectory(objLoaderMex)	
    endif()
    if(DEFINED BUILD_PARSER)
    	add_subdirectory(parser)
    endif()
    if(DEFINED BUILD_ZED)
    	add_subdirectory(ZED)
    endif()
    if(DEFINED BUILD_ZMQ)
    	add_subdirectory(ZMQ)
    endif()
    if(DEFINED BUILD_TINY_CUDA)
    	add_subdirectory(tiny_cuda)
    endif()
    
    
    
    
    

ELSE(MATLAB_FOUND)
    message( FATAL_ERROR "MATLAB not found...nothing will be built." )
ENDIF(MATLAB_FOUND)
