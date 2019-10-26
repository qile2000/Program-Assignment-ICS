#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if ( ref_r->eax != cpu.eax){printf("ref.eax: %d, \ncpu.eax: %d\n",ref_r->eax,cpu.eax); return false;}
  if ( ref_r->ecx != cpu.ecx){printf("ref.ecx: %d, \ncpu.ecx: %d\n",ref_r->ecx,cpu.ecx); return false;}
  if ( ref_r->edx != cpu.edx){printf("ref.edx: %d, \ncpu.edx: %d\n",ref_r->edx,cpu.edx); return false;}
  if ( ref_r->ebx != cpu.ebx){printf("ref.ebx: %d, \ncpu.ebx: %d\n",ref_r->ebx,cpu.ebx); return false;}
  if ( ref_r->esp != cpu.esp){printf("ref.esp: %d, \ncpu.esp: %d\n",ref_r->esp,cpu.esp); return false;}
  if ( ref_r->ebp != cpu.ebp){printf("ref.ebp: %d, \ncpu.ebp: %d\n",ref_r->ebp,cpu.ebp); return false;}
  if ( ref_r->esi != cpu.esi){printf("ref.esi: %d, \ncpu.esi: %d\n",ref_r->esi,cpu.esi); return false;}
  if ( ref_r->edi != cpu.edi){printf("ref.edi: %d, \ncpu.edi: %d\n",ref_r->edi,cpu.edi); return false;}
  if ( pc != cpu.pc){printf("ref's pc: %x, \ncpu.pc: %x\n",pc,cpu.pc); return false;}
  printf("temporary, regs is the same\n");
  return true;
}

void isa_difftest_attach(void) {
}
