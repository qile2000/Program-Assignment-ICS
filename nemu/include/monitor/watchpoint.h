#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  //监视点的序号
  int NO;
  struct watchpoint *next;
  char expression[128];
  int value;
  /* TODO: Add more members if necessary */
} WP;

WP* new_wp();
void free_wp(int NO,bool* suc);
#endif
