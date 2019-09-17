#include "nemu.h"
#include "stdlib.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
enum {
  TK_NOTYPE = 256, TK_EQ = 1111, TK_TEN_NUM = 2222

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},        // spaces
  {"\\+", '+'},             // plus
  {"==", TK_EQ},            // equal
  {"\\-", '-'},             // minus
  {"\\*", '*'},             // multiply
  {"\\/", '/'},             // devide
  {"[0-9]+", TK_TEN_NUM},   // 十进制整数
  {"\\(", '('},             // 左括号
  {"\\)", ')'},             // 右括号

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
static Token tokens[32] __attribute__((used)) = {};
/*nr_token指示已经被识别出来的token数目*/
static int nr_token __attribute__((used))  = 0;

/*给出待求值表达式后，此函数用于识别其中的token*/
static bool make_token(char *e) {
  /*position来指示当前处理到的位置*/
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  /*按顺序尝试用不同的规则来匹配当前位置的字符串，当一条规则匹配成功，并且匹配出的子串正好时position所在位置的时候，
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

        switch (rules[i].token_type) {
          case '+':case '-':case '*':case '/':case '(':case ')':{
            tokens[nr_token].type=rules[i].token_type;
            nr_token++;
          }
          break;
          case TK_TEN_NUM:{
            tokens[nr_token].type=rules[i].token_type;
            strcpy(tokens[nr_token].str, substr_start);
            nr_token++;
          }
          break;
          case TK_EQ:{
            tokens[nr_token].type=rules[i].token_type;
            nr_token++;
          }
          break;
          default: continue;
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
  TODO();

  return 0;
}
