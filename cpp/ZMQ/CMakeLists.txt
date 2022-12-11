set(TARGET mexZMQ)

SET(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)

include_directories(proto)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(ZMQ)
if (ZMQ_FOUND)
  message(STATUS "ZeroMQ found.")
  add_definitions(-DZMQ_FOUND)

  set(APP_CPPS ${APP_CPPS} ./bt_editor/sidepanel_monitor.cpp)
  set(FORMS_UI ${FORMS_UI} ./bt_editor/sidepanel_monitor.ui)

else ()
  message(WARNING "ZeroMQ NOT found. Real-time monitoring disabled.")
endif ()


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${ZMQ_INCLUDE_DIRS})
include_directories(${MATLAB_INCLUDE_DIR})

add_library(${TARGET} SHARED server.cpp proto/msgs.pb.cc ${CMAKE_SOURCE_DIR}/Matlabdef.def)
target_link_libraries(${TARGET}
    ${MATLAB_LIBRARIES}
    ${ZMQ_LIBRARIES}
    ${PROTOBUF_LIBRARY}
    )

if (WIN32)
  SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexw64)
else (WIN32)
  if (CMAKE_SIZEOF_VOID_P MATCHES "8")
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexa64 PREFIX "")
  else (CMAKE_SIZEOF_VOID_P MATCHES "8")
    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES SUFFIX .mexglx PREFIX "")
  endif (CMAKE_SIZEOF_VOID_P MATCHES "8")
endif (WIN32)


install(TARGETS ${TARGET} DESTINATION ${CMAKE_SOURCE_DIR}/../mex)
message("Done!!!: " ${TARGET})

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
  message(STATUS "${_variableName}=${${_variableName}}")
endforeach ()