#include <am.h>
#include <amdev.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000
unsigned long long fetch;
unsigned long long temp;
//keydown = 1为按下按键, keydown = 0为释放按键. 
//keycode为按键的断码, 没有按键时, keycode为_KEY_NONE.
size_t __am_input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _DEV_INPUT_KBD_t *kbd = (_DEV_INPUT_KBD_t *)buf;
      kbd->keydown = 0;
      fetch = inl(0x60);
      kbd->keycode = fetch;
      if(fetch!=temp){
	      kbd->keydown=1-kbd->keydown;
      }
      temp = fetch;
      return sizeof(_DEV_INPUT_KBD_t);
    }
  }
  return 0;
}
