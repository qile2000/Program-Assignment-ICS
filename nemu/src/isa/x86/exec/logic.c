#include "cpu/exec.h"
#include "cc.h"

make_EHelper(test) {
  rtl_and(&s0,&id_dest->val,&id_src->val);
  cpu.eflags.CF = 0;
  cpu.eflags.OF = 0;
  rtl_update_ZFSF(&s0,id_dest->width);
  print_asm_template2(test);
}

make_EHelper(and) {
  rtl_and(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);

  rtl_update_ZFSF(&s0, id_dest->width);

  cpu.eflags.CF = 0;
  cpu.eflags.OF = 0;

  print_asm_template2(and);
}

make_EHelper(xor) {
  rtl_xor(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest,&s0);
  
  rtl_update_ZFSF(&id_dest->val, id_dest->width);

  cpu.eflags.CF = 0;
  cpu.eflags.OF = 0;

  print_asm_template2(xor);
}

make_EHelper(or) {
  rtl_or(&s0, &id_dest->val, &id_src->val);
  

  operand_write(id_dest, &s0);
  rtl_update_ZFSF(&s0, id_dest->width);
  cpu.eflags.CF = 0;
  cpu.eflags.OF = 0;

  print_asm_template2(or);
}

make_EHelper(sar) {
  rtl_sar(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);
  rtl_update_ZFSF(&s0,id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(sar);
}

make_EHelper(shl) {
  rtl_shl(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);
  rtl_update_ZFSF(&s0,id_dest->width);
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shl);
}

make_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;

  rtl_setcc(&s0, cc);
  operand_write(id_dest, &s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  rtl_mv(&s0, &id_dest->val);
  rtl_not(&s0);
  operand_write(id_dest, &s0);

  print_asm_template1(not);
}
