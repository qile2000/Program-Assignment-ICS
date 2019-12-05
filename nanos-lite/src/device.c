#include "common.h"
#include <amdev.h>


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
  /*
  int key = read_key();
  if(key!=_KEY_NONE){
    bool down = false;
	  if((key&0x8000)==0){ 
      down = true;
		}
    if(down){
      snprintf(buf,len,"up: %s\n",keyname[key]);
    }
	  else{    
		  snprintf(buf,len,"down %s\n",keyname[key&0x7fff]);
	  }
  }
  else{
	  uint32_t the_time = uptime();
	  snprintf(buf,len,"the time is: %d\n",the_time);
  }
  return strlen(buf);
  */
  int key = read_key();
  bool down = false;
  if(key & 0x8000) {
	  key ^= 0x8000;
	  down = true;
  }
  uint32_t time=uptime();
  if(key != _KEY_NONE) {
	 return sprintf(buf, "%s %s\n" , down ? "kd" : "ku", keyname[key]);
  }
  else{
	 return  sprintf(buf, "t %d\n", time);
  } 
  assert(0);
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
