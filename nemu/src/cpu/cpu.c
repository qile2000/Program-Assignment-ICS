#include "cpu/exec.h"

CPU_state cpu;

rtlreg_t s0, s1, t0, t1, ir;

/* shared by all helper functions */
DecodeInfo decinfo;

void decinfo_set_jmp(bool is_jmp) {
  decinfo.is_jmp = is_jmp;
}

void isa_exec(vaddr_t *pc);
/*让CPU执行当前PC指向的一条指令，然后更新PC，已经执行的指令输出到日志文件中*/
vaddr_t exec_once(void) {
  decinfo.seq_pc = cpu.pc;
  isa_exec(&decinfo.seq_pc);
  update_pc();

  return decinfo.seq_pc;
}
