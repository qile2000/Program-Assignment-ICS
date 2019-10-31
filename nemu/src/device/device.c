#include "common.h"

#ifdef HAS_IOE

#include <sys/time.h>
#include <signal.h>
#include <SDL2/SDL.h>

#define TIMER_HZ 100
#define VGA_HZ 50
/*
含有和SDL库相关的代码. init_device()函数首先对四个设备进行初始化, 
其中在初始化VGA时还会进行一些和SDL相关的初始化工作, 包括创建窗口, 设置显示模式等. 
最后还会注册一个100Hz的定时器, 每隔0.01秒就会调用一次device_update()函数. 
device_update()函数主要检测是否有按键按下/释放, 以及是否点击了窗口的X按钮. 
需要说明的是, 代码中注册的定时器是虚拟定时器, 它只会在NEMU处于用户态的时候进行计时: 
如果NEMU在ui_mainloop()中等待用户输入, 定时器将不会计时; 如果NEMU进行大量的输出, 定时器的计时将会变得缓慢. 
因此除非你在进行调试, 否则尽量避免大量输出的情况
*/
static struct itimerval it = {};
static int device_update_flag = false;

void init_serial();
void init_timer();
void init_vga();
void init_i8042();

void timer_intr();
void send_key(uint8_t, bool);

static void timer_sig_handler(int signum) {
  timer_intr();

  device_update_flag = true;

  int ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
  Assert(ret == 0, "Can not set timer");
}

void device_update() {
  if (!device_update_flag) {
    return;
  }
  device_update_flag = false;

  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: {
                       void monitor_statistic();
                       monitor_statistic();
                       exit(0);
                     }

                     // If a key was pressed
      case SDL_KEYDOWN:
      case SDL_KEYUP: {
                        uint8_t k = event.key.keysym.scancode;
                        bool is_keydown = (event.key.type == SDL_KEYDOWN);
                        send_key(k, is_keydown);
                        break;
                      }
      default: break;
    }
  }
}

void sdl_clear_event_queue() {
  SDL_Event event;
  while (SDL_PollEvent(&event));
}

void init_device() {
  init_serial();
  init_timer();
  init_vga();
  init_i8042();

  struct sigaction s;
  memset(&s, 0, sizeof(s));
  s.sa_handler = timer_sig_handler;
  int ret = sigaction(SIGVTALRM, &s, NULL);
  Assert(ret == 0, "Can not set signal handler");

  it.it_value.tv_sec = 0;
  it.it_value.tv_usec = 1000000 / TIMER_HZ;
  ret = setitimer(ITIMER_VIRTUAL, &it, NULL);
  Assert(ret == 0, "Can not set timer");
}
#else

void init_device() {
}

#endif	/* HAS_IOE */
