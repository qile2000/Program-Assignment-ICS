#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32


WP* new_wp();
void free_wp(int NO, bool* suc);
bool init=false;

//使用“池”的数据结构来管理监视点结构体
//监视点结构的池
static WP wp_pool[NR_WP] = {};

//head用于组织使用中的监视点结构, free_用于组织空闲的监视点结构
static WP *head = NULL, *free_ = NULL;

//对两个链表进行了初始化
void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
  init = true;
}

/* TODO: Implement the functionality of watchpoint */
//从free_链表中返回一个空闲的监视点结构
//调用new_wp()时可能会出现没有空闲监视点结构的情况, 为了简单起见, 此时可以通过assert(0)马上终止程序
WP* new_wp(){
  if (!init){
    init_wp_pool();
  }
  WP* nw=free_;
  if (nw == NULL){
    printf("no free watchpoint left!!!");
    assert(0);
  }
  if (head == NULL){
    free_ = free_->next;
    head = nw;
    return nw;
  }
  else {
    free_ = free_->next;
    nw->next = head;
    head = nw;
    return nw;
  }
}

//将wp归还到free_链表中
void free_wp(int N, bool* suc){
  if (head->NO == N){
    head->next = free_;
    free_ = head;
    head = NULL;
    init = false;
    printf("666\n");

  }
  WP* find=head;
  while(find->next->NO!=N){
    find = find->next;
  }
  WP* goal=find->next;
  find->next = goal->next;
  goal->next=free_;
  free_=goal;
  
  *suc = true;
}