#include "klib.h"
#include <stdarg.h>
/*
va_list 是一个字符指针，可以理解为指向当前参数的一个指针，取参必须通过这个指针进行。
<Step 1> 在调用参数表之前，定义一个 va_list 类型的变量，(假设va_list 类型变量被定义为ap)；
<Step 2> 然后应该对ap 进行初始化，让它指向可变参数表里面的第一个参数，这是通过 va_start 来实现的，
          第一个参数是 ap 本身，第二个参数是在变参表前面紧挨着的一个变量,即“...”之前的那个参数；
<Step 3> 然后是获取参数，调用va_arg，它的第一个参数是ap，第二个参数是要获取的参数的指定类型，
        然后返回这个指定类型的值，并且把 ap 的位置指向变参表的下一个变量位置； 
<Step 4> 获取所有的参数之后，我们有必要将这个 ap 指针关掉，以免发生危险，方法是调用 va_end，
        它是输入的参数 ap 置为 NULL，应该养成获取完参数表之后关闭指针的习惯,让我们的程序具有健壮性。
        通常va_start和va_end是成对出现。
*/
#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

//arg 一个表示可变参数列表的对象。这应被 <stdarg> 中定义的 va_start 宏初始化

static char * itoa(int num, char *str, int base) {
  assert(num >= 0);
  int count = 0;
  while (num != 0){
    str[count] = (char)(num%base+'0');
    num /= base;
    count++;
  }
  str[count] = '\0';
  for (int i = 0; i < count / 2; ++i) {
    char tmp = str[i];
    str[i] = str[count - i - 1];
    str[count - i - 1] = tmp;
  }
  return str;
}

int printf(const char *fmt, ...) {
  va_list args;
  char string[1000];
  va_start(args,fmt);
  int ret = vsprintf(string,fmt,args);
  va_end(args);
  for(int i = 0;i < strlen(string);i ++){
      _putc(string[i]);	  
  }
  return ret;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  char *begin = out;
  while (*fmt != '\0') {
    if (*fmt == '%') {
      _Bool exit = 0;
      while (!exit) {
        switch (*++fmt) {
          case 'd': {
            int i = va_arg(ap, int);
            itoa(i, out, 10);
            while (*out != '\0') {
              ++out;
            }
            exit = 1;
            break;
          }
          case 's': {
            char *s = va_arg(ap, char *);
            while (*s != '\0') {
              *out++ = *s++;
            }
            exit = 1;
            break;
          }
          default: 
            assert(0);
        }
      }
      ++fmt;
    }
    else {
      *out++ = *fmt++;
    }
  }
  *out = '\0';
  return out - begin;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  int ret;
  va_start(args,fmt);
  ret = vsprintf(out,fmt,args);
  va_end(args);
  return ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

#endif
