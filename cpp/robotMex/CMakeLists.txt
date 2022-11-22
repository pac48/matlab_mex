set(TARGET robot_mex)

SET(CMAKE_BUILD_TYPE Debug)
#SET(CMAKE_BUILD_TYPE RelWithDebInfo)

find_package(RL REQUIRED)
find_path(TINYXML2_INCLUDE_DIR NAMES tinyxml2.h)
find_library(TINYXML2_LIBRARY tinyxml2)

set(RL_INCLUDE_DIR /usr/include/rl-0.7.0)

add_library(${TARGET} SHARED robot_mex.cpp ${CMAKE_SOURCE_DIR}/Matlabdef.def)
add_definitions(-std=c++17)
target_link_libraries(${TARGET}
					${MATLAB_LIBRARIES}
                    ${RL_LIBRARIES}
                    ${TINYXML2_LIBRARY}
)
#target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${MATLAB_INCLUDE_DIR})
include_directories(${RL_INCLUDE_DIR})

if(WIN32)
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexw64)
else(WIN32)
    if (CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexa64 PREFIX "")
    else(CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexglx PREFIX "")
    endif (CMAKE_SIZEOF_VOID_P MATCHES "8")
endif(WIN32)

#SET_TARGET_PROPERTIES(${TARGET}
#        PROPERTIES
#        CUDA_SEPERABLE_COMPILATION ON
#LINKER_LANGUAGE CUDA)


#if(BUILD_TESTING)
#    find_package(ament_cmake_gmock REQUIRED)
#    find_package(controller_manager REQUIRED)
#    find_package(ros2_control_test_assets REQUIRED)
#
#    ament_add_gmock(${TARGET}_test test/robot_mex_test.cpp)
#    target_link_libraries(${TARGET}_test ${TARGET})
#
#endif()

find_package(GTest REQUIRED)

enable_testing()

add_executable(
        ${TARGET}_test
        test/robot_mex_test.cpp
)
target_link_libraries(
        ${TARGET}_test
        ${TARGET}
        GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(${TARGET}_test)

install(TARGETS ${TARGET} DESTINATION ${CMAKE_SOURCE_DIR}/../mex)
message(Done!!! ${TARGET})