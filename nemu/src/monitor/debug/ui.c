#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "monitor/watchpoint.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
void isa_reg_display();
uint32_t paddr_read(paddr_t addr, int len);
uint32_t expr(char *e, bool *success);
WP* new_wp();
void free_wp(int NO, bool* suc);
void print_watchpoint();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
//
static int cmd_si(char *args);
//
static int cmd_info(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Let program execute N instructions for one step and suspend", cmd_si },
  { "info", "Print register status or print monitoring point information", cmd_info },
  { "x", "Calculate the value of the expression EXPR and use the result as the address of starting memory", cmd_x },
  { "p", "Calculate the value of the expression", cmd_p },
  { "w", "create watchpoint with expression", cmd_w},
  { "d", "delete a watchpoint with NO", cmd_d},
  /* TODO: Add more commands */
//
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}
//

static int cmd_si(char *args){
  char *arg = strtok(NULL, " ");
  if (arg == NULL) {
    cpu_exec(1);
  }
  else{
    int N;
    N = atoi(arg);
    if (N<1){
      printf("there must be more than 1 instructions!!!\n");
    }
    else cpu_exec(N);
  }
  return 0;
}

static int cmd_info(char *args){
  char *arg = strtok(NULL, " ");
  if (arg == NULL) {
    printf("lack of arg!!!\n");
  }
  else if (strcmp(arg, "r") == 0){
    isa_reg_display();
  }
  else if (strcmp(arg, "w")==0){
    print_watchpoint();
  }
  else{
    printf("error arg name,use r / w!!!\n");
  }
  return 0;
}

static int cmd_x(char *args){
  char *arg_1 = strtok(NULL, " ");
  char *arg_2 = strtok(NULL, " ");
  if (arg_1 == NULL) {
    printf("need more 2 args!!!\n");
  }

  else if (arg_2 == NULL) {
    printf("need another arg!!!\n");
  }
  else {
    int N;
    bool suc=true;
    int value = expr(arg_2, &suc);
    if (suc){
      paddr_t addr;
      sscanf(arg_1,"%d",&N);
      sscanf(value,"%x",&addr);
      printf("十六进制,4字节/输出\n");
      for (int i=0; i<N; i++){
        printf("%#x:   %#x\n",addr,paddr_read(addr, 4));
        addr = addr+4;
      } 
    }
    else {
      printf("wrong expression!!!\n");
    }
    
  }
  return 0;
}
//
static int cmd_p(char *args){
  char *arg = strtok(NULL, "^");
  if (arg == NULL) {
    printf("need expression!!!\n");
  }
  else {
    bool suc=true;
    int value = expr(arg, &suc);
    if (suc){
      printf("the value of expression is : %x\n", value);
    }
    else {
      printf("wrong expression!!!\n");
    }
   
  }
  return 0;
}

static int cmd_w(char *args){
  char *arg = strtok(NULL, "^");
  if (arg == NULL) {
    printf("need another arg!!!\n");
  }
  else{
    WP* new_watchpoint = new_wp();
    strcpy(new_watchpoint->expression, arg);
    bool suc = true;
    new_watchpoint->value = expr(args, &suc);
    if(!suc){
      printf("fail to calculate expr!!!\n");
      assert(0);
    }
    else {
      printf("now, the value of watchpoint expr is %d\n",new_watchpoint->value);
      printf("the NO of this watchpoint is %x\n",new_watchpoint->NO);
    }
  }
  return 0;
}

static int cmd_d(char *args){
  char *arg = strtok(NULL, " ");
  if(arg == NULL){
    printf("need another arg!!!\n");
  }
  else{
    int N;
    sscanf(arg,"%d",&N);
    bool suc = false;
    free_wp(N, &suc);
    if(!suc){
      printf("fail to delete watchpoint!!!\n");
    }
  }
  return 0;
}
/*monitor的核心功能，用户界面主循环*/
void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif
    
    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
