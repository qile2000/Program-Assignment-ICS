#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  /*
  s0 = id_dest->val;
  rtl_push(&s0);
  */
  //rtl_push(&s0);
  //if (id_dest->type == OP_TYPE_REG) { rtl_sr(id_dest->reg, &s0, id_dest->width); }//如果目的操作数是寄存器操作数,写入寄存器
  //else if (id_dest->type == OP_TYPE_MEM) { rtl_sm(&id_dest->addr, &s0, id_dest->width); }//如果是在内存里面，写入内存
  if(id_dest->width == 1){
	  uint8_t utmp = id_dest->val;
		int8_t temp = utmp;
		id_dest->val = temp;
	}
   	rtl_push(&id_dest->val);  
  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&s0);
	
	
	if(id_dest->width == 1){
		uint8_t utemp = s0;
		int8_t temp = utemp; 
		id_dest->val = temp;
	}
	else 
	  	id_dest->val = s0;
	operand_write(id_dest, &id_dest->val);

  print_asm_template1(pop);
}

make_EHelper(pusha) {
  TODO();

  print_asm("pusha");
}

make_EHelper(popa) {
  TODO();

  print_asm("popa");
}

make_EHelper(leave) {
  cpu.esp = cpu.ebp;
  rtl_pop(&cpu.ebp);
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  operand_write(id_dest, &s0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
