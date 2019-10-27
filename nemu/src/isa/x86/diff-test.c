#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if ( ref_r->eax != cpu.eax){printf("pc at %x: ref.eax: %d, \ncpu.eax: %d\n",cpu.pc,ref_r->eax,cpu.eax); return false;}
  if ( ref_r->ecx != cpu.ecx){printf("pc at %x: ref.ecx: %d, \ncpu.ecx: %d\n",cpu.pc,ref_r->ecx,cpu.ecx); return false;}
  if ( ref_r->edx != cpu.edx){printf("pc at %x: ref.edx: %d, \ncpu.edx: %d\n",cpu.pc,ref_r->edx,cpu.edx); return false;}
  if ( ref_r->ebx != cpu.ebx){printf("pc at %x: ref.ebx: %d, \ncpu.ebx: %d\n",cpu.pc,ref_r->ebx,cpu.ebx); return false;}
  if ( ref_r->esp != cpu.esp){printf("pc at %x: ref.esp: %d, \ncpu.esp: %d\n",cpu.pc,ref_r->esp,cpu.esp); return false;}
  if ( ref_r->ebp != cpu.ebp){printf("pc at %x: ref.ebp: %d, \ncpu.ebp: %d\n",cpu.pc,ref_r->ebp,cpu.ebp); return false;}
  if ( ref_r->esi != cpu.esi){printf("pc at %x: ref.esi: %d, \ncpu.esi: %d\n",cpu.pc,ref_r->esi,cpu.esi); return false;}
  if ( ref_r->edi != cpu.edi){printf("pc at %x: ref.edi: %d, \ncpu.edi: %d\n",cpu.pc,ref_r->edi,cpu.edi); return false;}
  //if ( pc != cpu.pc){printf("ref's pc: %x, \ncpu.pc: %x\n",pc,cpu.pc); return false;}
  printf("pc at %x: temporary, regs is the same\n",cpu.pc);
  return true;
}

void isa_difftest_attach(void) {
}
