#include <amtest.h>
#include <amdev.h>
#include <klib.h>
void rtc_test() {
  _DEV_TIMER_DATE_t rtc;
  int sec = 1;
  printf("we are in rtc_test()\n");
  while (1) {
    while(uptime() < 1000 * sec) ;
    get_timeofday(&rtc);
    printf("%d-%d-%d %02d:%02d:%02d GMT (", rtc.year, rtc.month, rtc.day, rtc.hour, rtc.minute, rtc.second);
    if (sec == 1) {
      printf("%d second).\n", sec);
    } else {
      printf("%d seconds).\n", sec);
    }
    sec ++;
  }
}
