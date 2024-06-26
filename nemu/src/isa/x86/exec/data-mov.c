#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
	rtl_push(&(id_dest->val));
  print_asm_template1(push);
}

make_EHelper(pop) {
  rtl_pop(&s0);
	operand_write(id_dest, &s0);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  uint32_t temp = cpu.esp;
  rtl_push(&cpu.eax);
  rtl_push(&cpu.ecx);
  rtl_push(&cpu.edx);
  rtl_push(&cpu.ebx);
  rtl_push(&temp);
  rtl_push(&cpu.ebp);
  rtl_push(&cpu.esi);
  rtl_push(&cpu.edi);
  print_asm("pusha");
}

make_EHelper(popa) {
  rtl_pop(&cpu.edi);
  rtl_pop(&cpu.esi);
  rtl_pop(&cpu.ebp);
  uint32_t throwaway=0;
  rtl_pop(&throwaway);
  rtl_pop(&cpu.ebx);
  rtl_pop(&cpu.edx);
  rtl_pop(&cpu.ecx);
  rtl_pop(&cpu.eax);
  print_asm("popa");
}

make_EHelper(leave) {
  cpu.esp = cpu.ebp;
  rtl_pop(&cpu.ebp);
  
  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
		if((int16_t)(reg_w(R_AX))<0){
			reg_w(R_DX) = 0b1111111111111111;
		}
    else{
			reg_w(R_DX) = 0;
		}
  }
  else {
		if((int32_t)(reg_l(R_EAX))<0){
			reg_l(R_EDX) = 0b11111111111111111111111111111111;
		}
    else{
			reg_l(R_EDX) = 0;
		}
  }
  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    rtlreg_t temp1 = 24;
    rtlreg_t temp2 = 8;
    rtlreg_t temp3 = 16;
		rtl_shl(&reg_l(R_EAX), &reg_l(R_EAX), &temp1);
		rtl_sar(&reg_l(R_EAX), &reg_l(R_EAX), &temp2);
	  rtl_shr(&reg_l(R_EAX), &reg_l(R_EAX), &temp3);
	}
	else {
		rtl_sext(&reg_l(R_EAX), &reg_l(R_EAX), 2);
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
