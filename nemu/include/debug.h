#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "common.h"
#include <stdio.h>
#include <assert.h>
#include "monitor/log.h"
/*printf的升级版，专门用来输出调试信息，同时还会输出使用Log（）所在的源文件，行号和函数*/
#define Log(format, ...) \
    _Log("\33[1;34m[%s,%d,%s] " format "\33[0m\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__)
/*assert()的升级版，当测试条件为假时，在assertion fail之前可以输出一些信息*/
#define Assert(cond, ...) \
  do { \
    if (!(cond)) { \
      fflush(stdout); \
      fprintf(stderr, "\33[1;31m"); \
      fprintf(stderr, __VA_ARGS__); \
      fprintf(stderr, "\33[0m\n"); \
      assert(cond); \
    } \
  } while (0)
/*用于输出信息并结束程序，相当于无条件的assertion fail*/
#define panic(...) Assert(0, __VA_ARGS__)

#define TODO() panic("please implement me")

#endif
