#include "proc.h"
#include <elf.h>

#define DEFAULT_ENTRY 0x8048000

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

extern size_t ramdisk_read(void* buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();
extern size_t ramdisk_write(const void* buf, size_t offset, size_t len);
extern int fs_open(const char *pathname, int flags, int mode);
extern size_t  fs_read(int fd, void *buf, size_t len);
extern size_t fs_filesz(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {
  
  ramdisk_read((void*)DEFAULT_ENTRY,0,get_ramdisk_size());
  return (uintptr_t)DEFAULT_ENTRY;
  /*
 int fd =fs_open(filename,0,0);
  size_t len = fs_filesz(fd);
  uint32_t my_ab=0,my_pages;
  my_ab = len&0xfff;
  my_pages = len>>12;
  void *my_phy,*my_vir;
  for(uint32_t i=0; i<my_pages; i++){
	  my_phy = new_page(1);
	  my_vir = (void *)(uintptr_t)(DEFAULT_ENTRY+(i<<12));
	  _map(&pcb->as,my_vir,my_phy,1);
	  fs_read(fd,my_phy,1<<12);
  }
  if(my_ab){
          my_phy = new_page(1);
	  my_vir = (void *)(uintptr_t)(DEFAULT_ENTRY+(my_pages<<12));
	  _map(&pcb->as,my_vir,my_phy,1);
	  fs_read(fd,my_phy,my_ab);
  }
  pcb -> max_brk = DEFAULT_ENTRY + len;
  pcb -> cur_brk = DEFAULT_ENTRY + len;
  return DEFAULT_ENTRY;
  */
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
