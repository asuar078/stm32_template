//
// Created by bigbywolf on 1/19/20.
//

#ifndef ETL_PROFILE_H
#define ETL_PROFILE_H

#ifdef LOCAL_TEST
#define ETL_TARGET_DEVICE_X86
#define ETL_TARGET_OS_LINUX
#else
#define ETL_TARGET_DEVICE_ARM
#define ETL_TARGET_OS_FREERTOS
#endif

#define ETL_COMPILER_GCC

#define ETL_CPP11_SUPPORTED                      (__cplusplus >= 201103L)
#define ETL_CPP14_SUPPORTED                      (__cplusplus >= 201402L)
#define ETL_CPP17_SUPPORTED                      (__cplusplus >= 201703L)
#define ETL_CPP20_SUPPORTED                      (__cplusplus >= 201703L)

#define ETL_NO_NULLPTR_SUPPORT                     !ETL_CPP11_SUPPORTED
#define ETL_NO_LARGE_CHAR_SUPPORT                  !ETL_CPP11_SUPPORTED
#define ETL_CPP11_TYPE_TRAITS_IS_TRIVIAL_SUPPORTED ETL_CPP14_SUPPORTED

#define ETL_VERBOSE_ERRORS 1
#define ETL_CHECK_PUSH_POP 1

#endif //ETL_PROFILE_H
