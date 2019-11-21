#include "common.h"
#include "syscall.h"

int sys_write(int fd,const void *buf,size_t len);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case SYS_yield: _yield(); c->GPRx=0; break;
    case SYS_exit: _halt(a[1]); break;
    case SYS_brk: c->GPRx=sys_brk(c); break;
    case SYS_write: c->GPRx=sys_write((int)a[1],(const void*)a[2],(size_t)a[3]);/*Log("SYS_WRITE");*/break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}

int sys_write(int fd,const void *buf,size_t len){
  if(fd==1||fd==2){
    char* _buf=(char*)buf;
		for(int i=0;i<len;i++){
			_putc(_buf[i]);
    }
	}
	return len;
}
int sys_brk(_Context *c){
	return 0;
}