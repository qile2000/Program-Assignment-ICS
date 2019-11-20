#include "proc.h"
#include <elf.h>



#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif
//extern void isa_paddr_write(uint32_t addr, uint32_t src, int len);
extern size_t ramdisk_read(void* buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();
extern size_t ramdisk_write(const void* buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr elf_header;

  ramdisk_read(&elf_header,0,sizeof(Elf_Ehdr));
/* Program header table entry count */
  Elf_Phdr pro_seg_header[elf_header.e_phnum];
  for(int i=0;i<elf_header.e_phnum;i++){
    if(pro_seg_header[i].p_type==PT_LOAD){
      ramdisk_read(&pro_seg_header[i].p_vaddr,pro_seg_header[i].p_offset,pro_seg_header[i].p_memsz);
      memset((void*)(pro_seg_header[i].p_vaddr+pro_seg_header[i].p_filesz),0,(pro_seg_header[i].p_memsz-pro_seg_header[i].p_filesz));
    }
  }
  return elf_header.e_entry;

  /*
  ramdisk_read((void*)0x00100000,0x001000,0x09afc);
  ramdisk_read((void*)0x0010a000,0x00b000,0x29828);
  memset((void*)(0x0010a000+0x00000),0,(0x29828-0x00000));
  return (uintptr_t)0x3000000;
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
