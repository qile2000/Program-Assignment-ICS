#include "nemu.h"
#include <stdlib.h>
#include <time.h>

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  uint32_t sample[8];
  uint32_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
  printf("32位寄存器:\n");
  printf("%s:  %o\n",regsl[0],cpu.eax);
  printf("%s:  %o\n",regsl[1],cpu.ecx);
  printf("%s:  %o\n",regsl[2],cpu.edx);
  printf("%s:  %o\n",regsl[3],cpu.ebx);
  printf("%s:  %o\n",regsl[4],cpu.esp);
  printf("%s:  %o\n",regsl[5],cpu.ebp);
  printf("%s:  %o\n",regsl[6],cpu.esi);
  printf("%s:  %o\n",regsl[7],cpu.edi);
  printf("16位寄存器:\n");
  for (int i=0; i<=7; i++){
    printf("%s:  %o\n",regsw[i],cpu.gpr[i]._16);
  }
  printf("8位寄存器：\n");
  for (int i=0; i<=7; i++){
    printf("%s:  %o\n",regsb[i],cpu.gpr[i]._8[0]);
    printf("     %o\n",cpu.gpr[i]._8[1]);
  }
}

uint32_t isa_reg_str2val(const char *s, bool *success) {
  return 0;
}
