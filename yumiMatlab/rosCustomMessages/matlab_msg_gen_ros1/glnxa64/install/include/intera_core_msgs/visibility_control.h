#ifndef INTERA_CORE_MSGS__VISIBILITY_CONTROL_H_
#define INTERA_CORE_MSGS__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define INTERA_CORE_MSGS_EXPORT __attribute__ ((dllexport))
    #define INTERA_CORE_MSGS_IMPORT __attribute__ ((dllimport))
  #else
    #define INTERA_CORE_MSGS_EXPORT __declspec(dllexport)
    #define INTERA_CORE_MSGS_IMPORT __declspec(dllimport)
  #endif
  #ifdef INTERA_CORE_MSGS_BUILDING_LIBRARY
    #define INTERA_CORE_MSGS_PUBLIC INTERA_CORE_MSGS_EXPORT
  #else
    #define INTERA_CORE_MSGS_PUBLIC INTERA_CORE_MSGS_IMPORT
  #endif
  #define INTERA_CORE_MSGS_PUBLIC_TYPE INTERA_CORE_MSGS_PUBLIC
  #define INTERA_CORE_MSGS_LOCAL
#else
  #define INTERA_CORE_MSGS_EXPORT __attribute__ ((visibility("default")))
  #define INTERA_CORE_MSGS_IMPORT
  #if __GNUC__ >= 4
    #define INTERA_CORE_MSGS_PUBLIC __attribute__ ((visibility("default")))
    #define INTERA_CORE_MSGS_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define INTERA_CORE_MSGS_PUBLIC
    #define INTERA_CORE_MSGS_LOCAL
  #endif
  #define INTERA_CORE_MSGS_PUBLIC_TYPE
#endif
#endif  // INTERA_CORE_MSGS__VISIBILITY_CONTROL_H_
// Generated 22-Nov-2022 11:32:55
// Copyright 2019-2020 The MathWorks, Inc.
