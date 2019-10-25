#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if ( ref_r->eax != cpu.eax){printf("ref.eax: %d, cpu.eax: %d\n",ref_r->eax,cpu.eax); return false;}
  if ( ref_r->ecx != cpu.ecx){printf("ref.ecx: %d, cpu.ecx: %d\n",ref_r->ecx,cpu.ecx); return false;}
  if ( ref_r->edx != cpu.edx){printf("ref.edx: %d, cpu.edx: %d\n",ref_r->edx,cpu.edx); return false;}
  if ( ref_r->ebx != cpu.ebx){printf("ref.ebx: %d, cpu.ebx: %d\n",ref_r->ebx,cpu.ebx); return false;}
  if ( ref_r->esp != cpu.esp){printf("ref.esp: %d, cpu.esp: %d\n",ref_r->esp,cpu.esp); return false;}
  if ( ref_r->ebp != cpu.ebp){printf("ref.ebp: %d, cpu.ebp: %d\n",ref_r->ebp,cpu.ebp); return false;}
  if ( ref_r->esi != cpu.esi){printf("ref.esi: %d, cpu.esi: %d\n",ref_r->esi,cpu.esi); return false;}
  if ( ref_r->edi != cpu.edi){printf("ref.edi: %d, cpu.edi: %d\n",ref_r->edi,cpu.edi); return false;}
  if ( pc != cpu.pc){printf("ref's pc: %x, cpu.pc: %x\n",pc,cpu.pc); return false;}
  return true;
}

void isa_difftest_attach(void) {
}
