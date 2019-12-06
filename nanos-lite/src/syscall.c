#include "common.h"
#include "syscall.h"
#include "proc.h"
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_read(int fd, void *buf, size_t len);
extern int fs_close(int fd);
extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_lseek(int fd,size_t offset,int whence);
extern void naive_uload(PCB *pcb,const char *filename);
int sys_write(int fd,const void *buf,size_t len);
int sys_brk(uintptr_t brk, intptr_t increment);
int	sys_execve(const char *filename,char *const argv[],char *const envp[]);

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case SYS_yield: {
      _yield(); 
      c->GPRx=0; 
      Log("SYS_YIELD");
      break;
    }
    case SYS_exit: {
      printf("SYS_exit\n");
      //_halt(a[1]);
      
      naive_uload(NULL, "/bin/init");
      c->GPRx = 0;
      Log("SYS_EXIT");
      break;
    }
    case SYS_brk: {
      c->GPRx = sys_brk(a[1],0);
      Log("SYS_BRK");
      break;
    }
    case SYS_open: {
      c->GPRx=fs_open((const char *)a[1],a[2],a[3]); 
      Log("SYS_OPEN");
      break;
    }
    case SYS_read: {
      c->GPRx=fs_read(a[1],(void *)a[2],a[3]);
      Log("SYS_READ");
      break;
    }
    case SYS_write: {
      c->GPRx=fs_write((int)a[1],(const void*)a[2],(size_t)a[3]);
      Log("SYS_WRITE");
      break;
    }
    case SYS_lseek: {
      c-> GPRx = fs_lseek((int) a[1], (size_t)a[2], (int) a[3]); 
      Log("SYS_LSEEK");
      break; 
    }
    case SYS_close: {
      c->GPRx=fs_close(a[1]);
      Log("SYS_CLOSE");
      break;
    }
    case SYS_execve: {
      printf("EXECVE\n");
      
      
      naive_uload(NULL, (const char *)a[1]);
      c->GPRx = 0;
      Log("SYS_EXECVE");
      break;
      //c->GPRx=sys_execve((const char *)a[1],(char *const*)a[2],(char *const*)a[3]); 
    }
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
/*
int	sys_execve(const char *filename,char *const argv[],char *const envp[]){
	naive_uload(NULL,filename);
	return -1;
}
*/
int sys_brk(uintptr_t brk, intptr_t increment){
  return 0;
}