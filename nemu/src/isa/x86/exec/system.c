#include "cpu/exec.h"
extern void raise_intr(uint8_t NO, vaddr_t ret_addr);

make_EHelper(lidt) {
  cpu.idtr.limit = vaddr_read(id_dest->addr, 2);
  if (decinfo.isa.is_operand_size_16) {
    cpu.idtr.base = vaddr_read(id_dest->addr + 2, 3);
  }
  else {
    cpu.idtr.base = vaddr_read(id_dest->addr + 2, 4);
  }

  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  //TODO();
  switch(id_dest->reg){
	  case 0:rtl_lr(&s1,id_src->reg,4);cpu.cr0.val=s1;break;
	  case 3:rtl_lr(&s1,id_src->reg,4);cpu.cr3.val=s1; break;
	  default : assert(0);
  }
  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  //TODO();
  switch(id_src->reg){
	  case 0: s0=cpu.cr0.val; rtl_sr(id_dest->reg,&s0,4); break;
	  case 3: s0=cpu.cr3.val; rtl_sr(id_dest->reg,&s0,4); break;
	  default: assert(0);
  }
  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

  difftest_skip_ref();
}

make_EHelper(int) {
  
  raise_intr(id_dest->val, decinfo.seq_pc);
  
  print_asm("int %s", id_dest->str);

  difftest_skip_dut(1, 2);
}

make_EHelper(iret) {
  
  rtl_pop(&decinfo.jmp_pc);
  rtl_pop(&cpu.cs);
  rtl_pop(&cpu.eflags.efl_val);
  rtl_j(decinfo.jmp_pc);
  print_asm("iret");
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

make_EHelper(in) {
  if(id_src->width == 4){
    s0=pio_read_l(id_src->val);
  }
  else if(id_src->width == 2){
    s0=pio_read_w(id_src->val);
  }
  else{
    s0=pio_read_b(id_src->val);
  }
  operand_write(id_dest,&s0);

#if defined(DIFF_TEST)
	difftest_skip_ref();
#endif

  print_asm_template2(in);
}

make_EHelper(out) {
  if(id_src->width == 4){
    pio_write_l(id_dest->val,id_src->val);
  }
  else if(id_src->width == 2){
    pio_write_w(id_dest->val,id_src->val);
  }
  else{
    pio_write_b(id_dest->val,id_src->val);
  }

#if defined(DIFF_TEST)
	difftest_skip_ref();
#endif

  print_asm_template2(out);
}
