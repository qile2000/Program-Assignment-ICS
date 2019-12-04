#include <stdio.h>
#include <assert.h>
int main() {
  //printf("999999999999999999\n");
  FILE *fp = fopen("/dev/events", "r");
  assert(fp!=NULL);
  int time = 0;
  //printf("Start to receive events...\n");
  while (1) {
    char buf[256];
    char *p = buf, ch;
    
    /*
    if(fgetc(fp)==-1){
      printf("8888888888888\n");
    }
    */
    while ((ch = fgetc(fp)) != -1) {
      //printf("8888888888888\n");
      *p ++ = ch;
      if(ch == '\n') {
        *p = '\0';
        break;
      }
    }

    int is_time = buf[0] == 't';
    time += is_time;
    if (!is_time) {
      //printf("receive event: %s", buf);
    }
    else if (time % 1024 == 0) {
      printf("receive time event for the %dth time: %s", time, buf);
    }
  }

  fclose(fp);
  return 0;
}

