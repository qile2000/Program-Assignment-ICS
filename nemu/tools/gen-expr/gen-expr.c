#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

uint32_t choose(uint32_t choose_rand);

void gen_num();
void gen(char gen_str);
void gen_rand_op();

int indx = 0;

// this should be enough
static char buf[65536];
static inline void gen_rand_expr() {
  switch (choose(3)) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(); gen(')');  break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }
}


uint32_t choose(uint32_t choose_rand){
  uint32_t rand_int = rand() % choose_rand;
  return rand_int;
}

void gen_num(){

  int randint = rand()+1;
  int t=0,mask=1;
	t=randint;
	while(t>9){
		t = t/10;
		mask = mask*10;
	}
	while(mask>0){
	 	buf[indx]=((randint/mask)+'0');
    indx++; 
		randint = randint % mask;
		mask = mask/10;
	} 
}

void gen(char gen_str){
  buf[indx] = gen_str;
  indx++;
}

void gen_rand_op(){
  int choice = choose (4);
  char op;
  switch(choice) {
    case 0: op = '+'; break;
    case 1: op = '-'; break;
    case 2: op = '/'; break;
    case 3: op = '*'; break; 
  }
  buf[indx] = op;
  indx++;
}
static char code_buf[65536];
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    indx=0;
    memset(buf,'\0',sizeof(buf));
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    /*fscanf可以从一个文件流中格式化读出数据，遇到空格或回车就停止*/
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
