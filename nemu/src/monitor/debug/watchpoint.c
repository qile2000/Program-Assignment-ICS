#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32


WP* new_wp();
void free_wp(int NO, bool* suc);
bool init=false;
void print_watchpoint();
bool check_watchpoint();

int wp_num=0;

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
  wp_num=0;
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
    wp_num++;
    return nw;
  }
  else {
    free_ = free_->next;
    nw->next = head;
    head = nw;
    wp_num++;
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
    *suc=true;
    wp_num--;
    return;
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
  wp_num--;
  return;
}

void print_watchpoint(){
  WP* print=head;
  if (wp_num == 0){
    printf("empty watchpoint pool!!!\n");
    return;
  }
  printf("NO   EXPRESSION              VALUE\n");
  for (int i= 0; i < wp_num;i++){
    printf("%d    %s                    %d\n",print->NO,print->expression,print->value);
    print=print->next;
  }
  return;
}

bool check_watchpoint(){
  WP* check=head;
  bool diff_value= false;
  if (wp_num == 0){
    return false;
  }
  for (int i= 0; i < wp_num;i++){
    bool suc = true;
    int now_value = expr(check->expression,&suc);
    if (!suc){
      printf("fail to calculate the new expression!!!\n");
      assert(0);
    }
    else {
      if(now_value == check->value){
        check=check->next;
        return false;
      }
      else{
        printf("paused for watchpoint NO.%d, expr: %s\n",check->NO,check->expression);
        printf("the old value is %d\n",check->value);
        printf("the present value is %d\n",now_value);
        check->value=now_value;
        return true;
      }
    }
  }
  return diff_value;
}