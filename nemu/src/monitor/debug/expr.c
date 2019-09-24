#include "nemu.h"
#include "stdlib.h"
#include "string.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

bool check_parentheses(int p, int q);
int dominant_operator(int p, int q);
uint32_t eval(int p,int q);
uint32_t paddr_read(paddr_t addr, int len);

const char *regsl_copy[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw_copy[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
//const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

enum {
  TK_NOTYPE = 256, TK_EQ = 257, TK_TEN_NUM = 258, TK_UEQ = 259, TK_SIXTEEN_NUM = 260, TK_REG = 261, TK_POINTER = 262,

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {"\\s+", TK_NOTYPE},        // spaces
  {"\\+", '+'},             // plus
  {"==", TK_EQ},            // equal
  {"!=", TK_UEQ},           // unequal
  {"\\-", '-'},             // minus
  {"\\*", '*'},             // multiply
  {"\\/", '/'},             // devide
  {"0x[0-9a-f]+", TK_SIXTEEN_NUM},  //十六进制数
  {"[0-9]+", TK_TEN_NUM},   // 十进制整数
  {"\\(", '('},             // 左括号
  {"\\)", ')'},             // 右括号
  {"&&", '&'},              // 且
  {"\\$[abcdehilpsx]{2,3}", TK_REG}, //寄存器


};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

/*token结构体记录token的信息*/
typedef struct token {
  /*type成员用于记录token的类型*/
  int type;
  /*str将token相应的子串记录下来*/
  char str[128];
} Token;

/*tokens数组用于按顺序存放已经被识别出来的token信息*/
static Token tokens[256] __attribute__((used)) = {};
/*nr_token指示已经被识别出来的token数目*/
static int nr_token __attribute__((used))  = 0;

/*给出待求值表达式后，此函数用于识别其中的token*/
static bool make_token(char *e) {
  /*position来指示当前处理到的位置*/
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  /*按顺序尝试用不同的规则来匹配当前位置的字符串，当一条规则匹配成功，并且匹配出的子串正好是position所在位置的时候，
   *我们就成功识别出一个token
   */
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
  /*Log()宏会识别成功的信息，使用Token结构体来记录token的信息
   */
        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */
        if(rules[i].token_type==256) {
          continue;
        }
        switch (rules[i].token_type) {
          case '+':case '-':case '*':case '/':case '(':case ')':case TK_EQ:case '&':case TK_UEQ:{
            tokens[nr_token].type=rules[i].token_type;
            nr_token++;
          }break;
          case TK_TEN_NUM:case TK_REG:case TK_SIXTEEN_NUM:{
            tokens[nr_token].type=rules[i].token_type;
            strcpy(tokens[nr_token].str, substr_start);
            nr_token++;
          }break;
          default: {
            printf("error rules switch!!!\n");
            assert(0);
          }
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

uint32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  for(int i=0; i < nr_token; i++){
    if(tokens[i].type == '*' && (i==0 || tokens[i-1].type == '+' || tokens[i-1].type =='*' || \
       tokens[i-1].type == '/' || tokens[i-1].type =='-' || tokens[i-1].type == TK_UEQ|| tokens[i-1].type == TK_EQ)){
      tokens[i].type = TK_POINTER;
    }
  }
  
  return eval(0, nr_token-1);
}
//
bool check_parentheses(int p, int q){
  int count = 0;
  bool flag = true;
  for (int i=p; i<=q; i++){
    if(tokens[i].type=='('){
      count++;
    }
    if(tokens[i].type==')'){
      count--;
    }
    if(count<0){
      printf("bad expression!!!\n");
      assert(0);
    }
    if(count==0 && i!=q){
      flag=false;
    }
  }
  if (count!=0){
    printf("bad expression!!!\n");
    assert(0);
  }
  else if(tokens[p].type=='(' && tokens[q].type==')' && flag){
    return true;
  }
  else {
    return false;
  } 
}
//
int dominant_operator(int p, int q){
  int count=0;
  int dm_op=p;
  for(int i = p; i<=q; i++){
    if(tokens[i].type=='('){
      count++;
    }
    else if(tokens[i].type==')'){
      count--;
    }
    if(count==0){
      if (tokens[i].type==TK_UEQ || tokens[i].type==TK_EQ || tokens[i].type=='&'){
        dm_op=i;
        return dm_op;
      }
      if ((tokens[i].type=='*' || tokens[i].type=='/') && (tokens[dm_op].type!='+' && tokens[dm_op].type!='-')) {
        dm_op=i;
      }
      else if (tokens[i].type=='+'||tokens[i].type=='-'){
        dm_op=i;   
      }
    } 
  }
  return dm_op;
}
//
uint32_t eval(int p, int q){
  if(p>q){
    return 0;
  }
  else if (p==q) {
    int value = 0;
    if (tokens[p].type == TK_TEN_NUM ){
      sscanf(tokens[p].str, "%d", &value);
      return value;
    }
    else if (tokens[p].type == TK_SIXTEEN_NUM){
      sscanf(tokens[p].str, "%x", &value);
      return value;
    }
    else if (tokens[p].type == TK_REG){
      for (int j=0; j<strlen(tokens[p].str); j++){
        tokens[p].str[j]=tokens[p].str[j+1];
      }
      for (int i=0; i<=7; i++){
        if (strcmp(tokens[p].str,regsl_copy[i])==0){
          return cpu.gpr[i]._32;
        }
      } 
      for (int i=0; i<=7; i++){
        if (strcmp(tokens[p].str,regsw_copy[i])==0){
          return cpu.gpr[i]._16;
        }
      }
      printf("error reg name!!!\n");
      assert(0);
      
    }
  }
  else if (check_parentheses(p, q) == true) {
    return eval(p+1, q-1);
  }
  else {
    int op = dominant_operator(p, q);
    if(tokens[op].type == TK_POINTER){
      return paddr_read(eval(p+1,q),4);
    }
    int val1=eval(p, op-1);
    int val2=eval(op+1, q);

    switch(tokens[op].type){
      case '+': {
        return val1 + val2;
      }
      case '-': {
        return val1 - val2;
      }
      case '*': {
        return val1 * val2;
      }
      case '/': {
        return val1 / val2;
      }
      case TK_UEQ: {
        return val1 != val2;
      }
      case TK_EQ: {
        return val1 == val2;
      }
      case '&': {
        return val1 && val2;
      }
      default: assert(0);
    }
  }
  return 0;
}
//