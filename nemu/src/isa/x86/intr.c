#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  rtl_push(&cpu.eflags.efl_val);
  cpu.eflags.IF = 0;
  cpu.eflags.TF = 0;
  rtl_push(&cpu.cs);
  rtl_push(&ret_addr);
  vaddr_t jump=vaddr_read(cpu.idtr.base + NO * 8, 2)+(vaddr_read(cpu.idtr.base + NO * 8 + 6, 2) << 16);
  rtl_j(jump);
}

bool isa_query_intr(void) {
  return false;
}
