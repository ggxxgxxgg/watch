#ifndef __DATA_CENTER_LOG_H
#define __DATA_CENTER_LOG_H

#define DATA_CENTER_USE_LOG 1

#if !defined(ARDUINO) && DATA_CENTER_USE_LOG
#include <stdio.h>
#  define _DC_LOG(format, ...)      printf("[DC]" format "\r\n", ##__VA_ARGS__)
#  define DC_LOG_INFO(format, ...)  _DC_LOG("[Info] "format, ##__VA_ARGS__)
#  define DC_LOG_WARN(format, ...)  _DC_LOG("[Warn] " format, ##__VA_ARGS__)
#  define DC_LOG_ERROR(format, ...) _DC_LOG("[Error] " format, ##__VA_ARGS__)
#else
#  define DC_LOG_INFO(...)
#  define DC_LOG_WARN(...)
#  define DC_LOG_ERROR(...)
#endif

#endif
