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
  printf("bug in sar: %x\n", id_dest->val);
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
  rtl_shr(&s0, &id_dest->val, &id_src->val);
  operand_write(id_dest, &s0);
  rtl_update_ZFSF(&s0,id_dest->width);
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
  rtl_not(&s1, &(id_dest->val));
  operand_write(id_dest, &s1);

  print_asm_template1(not);
}

make_EHelper(rol) {
  s0 = id_src->val;
  rtlreg_t s2 = 1;
  rtlreg_t s3,s4,s5;
  while(s0!=0){
    rtl_msb(&s1,&id_dest->val,id_dest->width);
    rtl_shl(&s4,&id_dest->val,&s2);
	  rtl_add(&s5,&s4,&s1);
    s0--;
  }
  if(id_src->val == 1){
    if(s1!=cpu.eflags.CF){
      cpu.eflags.OF = 1;
    }
    else{
      cpu.eflags.OF = 0;
    }
  }
  operand_write(id_dest,&s5);
  s3=id_dest->val;
  rtl_update_ZFSF(&s3,id_dest->width);
  
  print_asm_template2(shl);
}