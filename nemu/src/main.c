#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
int init_monitor(int, char *[]);
void ui_mainloop(int);
typedef __UINT32_TYPE__ uint32_t;
uint32_t expr(char *e, bool *success);

int main(int argc, char *argv[]) {

  
  /* Initialize the monitor. */
  
  int is_batch_mode = init_monitor(argc, argv);
  char the_expr[1000];
  FILE *fp=fopen("../nemu/tools/gen-expr/input","r");
  if(!fp){
    printf("can't open file\n");
    return -1;
  }
  while(!feof(fp))
  {
    fgets(the_expr,1000, fp);
    char *answer = strtok(the_expr, " ");
    char *expression = strtok(NULL, "\n");
    bool suc=true;
    printf(expression);
    int my_result = expr(expression,&suc);
    if (suc){
      printf("the result is %d\n", my_result);
    }
    else{
      printf("fail to calculate the expression!!!");
    }
    printf("the right answer is %s\n", answer);
    
  }
  fclose(fp);

    

  /* Receive commands from user. */
  ui_mainloop(is_batch_mode);

  return 0;
}
//
////


