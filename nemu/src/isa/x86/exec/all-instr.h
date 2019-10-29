#include "cpu/exec.h"

make_EHelper(mov);
make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

make_EHelper(call);			//control.c
make_EHelper(push); 		//data-mov.c
make_EHelper(sub); 			//arith.c
make_EHelper(xor); 			//logic.c
make_EHelper(ret);  		//control.c
make_EHelper(lea);          //data-mov.c
make_EHelper(and);          //logic.c
make_EHelper(add);           //arith.c
make_EHelper(cmp);          //arith.c
make_EHelper(setcc);        //logic.c
make_EHelper(movzx);         //data-mov.c
make_EHelper(test);          //logic.c
make_EHelper(jcc);           //control.c
make_EHelper(leave);         //data-mov.c
make_EHelper(inc);           //arith.c
make_EHelper(nop);             //special.c
make_EHelper(pop);           //data-mov.c
make_EHelper(adc);           //arith.c
make_EHelper(or);             //logic.c
make_EHelper(sar);            //logic.c
make_EHelper(shl);             //logic.c
make_EHelper(dec);            //arith.c
make_EHelper(jmp);              //control.c
make_DHelper(imul);         //arith.c
make_DHelper(imul1);         //arith.c
make_EHelper(imul2);             //arith.c
make_EHelper(movsx);            //data-mov.c
make_EHelper(neg);              //arith.c
make_EHelper(test);           //logic.c
make_EHelper(cltd);           //data-mov.c
make_EHelper(idiv);         //arith.c
make_EHelper(not);          //logic.c
make_EHelper(and);           //logic.c
make_EHelper(sbb);           //arith.c