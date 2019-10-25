#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  if ( r.eax != cpu.eax
    || r.ecx != cpu.ecx
    || r.edx != cpu.edx
    || r.ebx != cpu.ebx
    || r.esp != cpu.esp
    || r.ebp != cpu.ebp
    || r.esi != cpu.esi
    || r.edi != cpu.edi
    || r.eip != cpu.eip)
  {
    return false;
  }
  return true;
}

void isa_difftest_attach(void) {
}
