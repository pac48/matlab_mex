set(TARGET parseDAEMex)

#if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
#    SET(CMAKE_BUILD_TYPE "RelWithDebInfo")
#endif()

#find_package(tinyxml2 REQUIRED)

#if (CMAKE_BUILD_TYPE STREQUAL "Debug")
#    message("debug mode")
#    set(mode Debug)
#else(CMAKE_BUILD_TYPE STREQUAL "Release")
#    message("release mode")
#    set(mode Release)
#endif (CMAKE_BUILD_TYPE STREQUAL "Debug")

set(mode Debug)
SET(CMAKE_BUILD_TYPE Debug)


find_path(TINYXML2_INCLUDE_DIR NAMES tinyxml2.h)
find_library(TINYXML2_LIBRARY tinyxml2)
message("found lib is " ${TINYXML2_LIBRARY})

#include_directories("C:/Program Files (x86)/tinyxml2-${mode}/include")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${MATLAB_INCLUDE_DIR})

add_library(${TARGET} SHARED parseDAEMex.cpp ${CMAKE_SOURCE_DIR}/Matlabdef.def)
target_link_libraries(${TARGET}
        ${MATLAB_LIBRARIES}
#        "C:/Program Files (x86)/tinyxml2-${mode}/lib/tinyxml2.lib"
        ${TINYXML2_LIBRARY}
        )
set_property(TARGET ${TARGET} PROPERTY CXX_STANDARD 17)


if(WIN32)
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexw64)
else(WIN32)
    if (CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexa64 PREFIX "")
    else(CMAKE_SIZEOF_VOID_P MATCHES "8")
        SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexglx PREFIX "")
    endif (CMAKE_SIZEOF_VOID_P MATCHES "8")
endif(WIN32)

install(TARGETS ${TARGET} DESTINATION ${CMAKE_SOURCE_DIR}/../mex)

message("Done!!!: " ${TARGET})

add_executable(parseDAETest parseDAETest.cpp)

target_link_libraries(parseDAETest
#        "C:/Program Files (x86)/tinyxml2-${mode}/lib/tinyxml2.lib"
        ${TINYXML2_LIBRARY}
        )