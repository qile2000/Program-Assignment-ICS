#include "nemu.h"
#include "monitor/diff-test.h"
 
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if ( ref_r->eax != cpu.eax){printf("pc at %x ref %x: ref.eax: %d, cpu.eax: %d\n",cpu.pc,ref_r->pc,ref_r->eax,cpu.eax); return false;}
  if ( ref_r->ecx != cpu.ecx){printf("pc at %x ref %x: ref.ecx: %d, cpu.ecx: %d\n",cpu.pc,ref_r->pc,ref_r->ecx,cpu.ecx); return false;}
  if ( ref_r->edx != cpu.edx){printf("pc at %x ref %x: ref.edx: %d, cpu.edx: %d\n",cpu.pc,ref_r->pc,ref_r->edx,cpu.edx); return false;}
  if ( ref_r->ebx != cpu.ebx){printf("pc at %x ref %x: ref.ebx: %d, cpu.ebx: %d\n",cpu.pc,ref_r->pc,ref_r->ebx,cpu.ebx); return false;}
  if ( ref_r->esp != cpu.esp){printf("pc at %x ref %x: ref.esp: %d, cpu.esp: %d\n",cpu.pc,ref_r->pc,ref_r->esp,cpu.esp); return false;}
  if ( ref_r->ebp != cpu.ebp){printf("pc at %x ref %x: ref.ebp: %d, cpu.ebp: %d\n",cpu.pc,ref_r->pc,ref_r->ebp,cpu.ebp); return false;}
  if ( ref_r->esi != cpu.esi){printf("pc at %x ref %x: ref.esi: %d, cpu.esi: %d\n",cpu.pc,ref_r->pc,ref_r->esi,cpu.esi); return false;}
  if ( ref_r->edi != cpu.edi){printf("pc at %x ref %x: ref.edi: %d, cpu.edi: %d\n",cpu.pc,ref_r->pc,ref_r->edi,cpu.edi); return false;}
  printf("pc at %x ref %x:\n",cpu.pc,ref_r->pc);
  return true;
}

void isa_difftest_attach(void) {
}
