set(TARGET tiny_cuda_mex)

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE RelWithDebInfo)

#set(CUDA_NVCC_EXECUTABLE "/usr/local/cuda-12.0/bin/nvcc")
#set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} --expt-relaxed-constexpr")

find_package(CUDA REQUIRED)
find_package(nlohmann_json REQUIRED)
find_library(TINY_CUDA_LIBRARY tiny-cuda-nn)
find_library(FMT_LIBRARIES fmt)

option(TCNN_ALLOW_CUBLAS_CUSOLVER "Allows tiny-cuda-nn to use cuBLAS and cuSolver. Only required for the Shampoo optimizer." OFF)
#set(MIN_GPU_ARCH 89)
set(MIN_GPU_ARCH 52)

list(APPEND TCNN_LIBRARIES cuda)
list(APPEND TCNN_DEFINITIONS -DTCNN_MIN_GPU_ARCH=${MIN_GPU_ARCH})
if (TCNN_ALLOW_CUBLAS_CUSOLVER AND CUDA_VERSION VERSION_GREATER_EQUAL 11.0)
    list(APPEND TCNN_LIBRARIES cublas)
    list(APPEND TCNN_DEFINITIONS -DTCNN_SHAMPOO)
endif()

#add_library(${TARGET} SHARED tiny_cuda_mex.cpp ${CMAKE_SOURCE_DIR}/Matlabdef.def)
#set_source_files_properties(tiny_cuda_mex.cpp PROPERTIES LANGUAGE CUDA)
add_executable(${TARGET} tiny_cuda_mex.cpp ${CMAKE_SOURCE_DIR}/Matlabdef.def)

add_definitions(-std=c++17)

target_link_libraries(${TARGET}
        ${MATLAB_LIBRARIES}
        ${TINY_CUDA_LIBRARY}
        ${CUDA_LIBRARIES}
        ${TCNN_LIBRARIES}
        ${FMT_LIBRARIES}
        )
target_compile_definitions(${TARGET} PUBLIC ${TCNN_DEFINITIONS})
set_target_properties(${TARGET} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${MATLAB_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)


if(WIN32)
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexw64)
else(WIN32)
    if (CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexa64 PREFIX "")
    else(CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexglx PREFIX "")
    endif (CMAKE_SIZEOF_VOID_P MATCHES "8")
endif(WIN32)

#find_package(GTest REQUIRED)
#enable_testing()
#add_executable(
#        ${TARGET}_test
#        test/robot_mex_test.cpp
#)
#target_link_libraries(
#        ${TARGET}_test
#        ${TARGET}
#        GTest::gtest_main
#)
#include(GoogleTest)
#gtest_discover_tests(${TARGET}_test)

install(TARGETS ${TARGET} DESTINATION ${CMAKE_SOURCE_DIR}/../mex)
message(Done!!! ${TARGET})


get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach ()