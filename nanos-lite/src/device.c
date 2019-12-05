#include "common.h"
#include <amdev.h>

extern void draw_rect(uint32_t *pixels, int x, int y, int w, int h);

size_t serial_write(const void *buf, size_t offset, size_t len) {
  char* _buf=(char*)buf;
	for(int i=0;i<len;i++){
		_putc(_buf[i]);
  }
  return len;
  //return 0;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
  int key = read_key();
  bool down = false;
  if(key & 0x8000) {
	  key ^= 0x8000;
	  down = true;
  }
  
  if(key != _KEY_NONE) {
    if(down){
      sprintf(buf, "%s %s\n" , "kd", keyname[key]);
    }
	  else{
      sprintf(buf, "%s %s\n" , "ku", keyname[key]);
    }
  }
  else{
   uint32_t time=uptime();
	 sprintf(buf, "t %d\n", time);
  } 
  return strlen(buf);
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  /*
  memcpy(buf,dispinfo+offset,len);
  return strlen(buf);
  */
  return sprintf(buf,(void*)(&dispinfo[0]+offset),len);
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  offset /= 4;
  size_t draw_len = len / 4;
  draw_rect((void *) buf, offset%screen_width(), offset/screen_width(), draw_len, 1);
  return len;
  //return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  draw_sync();
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();
  sprintf(dispinfo,"WIDTH:%d\nHEIGHT:%d\n",screen_width(),screen_height());
  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
