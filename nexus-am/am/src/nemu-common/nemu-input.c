#include <am.h>
#include <amdev.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000
//keydown = 1为按下按键, keydown = 0为释放按键. 
//keycode为按键的断码, 没有按键时, keycode为_KEY_NONE.
size_t __am_input_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_INPUT_KBD: {
      _DEV_INPUT_KBD_t *kbd = (_DEV_INPUT_KBD_t *)buf;
      kbd->keycode = inl(0x60);
      if(kbd->keycode!=_KEY_NONE){
          kbd->keydown = !(kbd->keydown);
      }
      return sizeof(_DEV_INPUT_KBD_t);
    }
  }
  return 0;
}
