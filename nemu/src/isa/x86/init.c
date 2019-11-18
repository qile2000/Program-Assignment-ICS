#include "nemu.h"
/*内置客户程序*/
/*isa_default_img,isa_default_img_size两个变量作为API来给monitor使用，通过这两个，monitor可将内置客户程序读入到IMAGE-START的内存位置*/
const uint8_t isa_default_img []  = {
  0xb8, 0x34, 0x12, 0x00, 0x00,        // 100000:  movl  $0x1234,%eax
  0xb9, 0x27, 0x00, 0x10, 0x00,        // 100005:  movl  $0x100027,%ecx
  0x89, 0x01,                          // 10000a:  movl  %eax,(%ecx)
  0x66, 0xc7, 0x41, 0x04, 0x01, 0x00,  // 10000c:  movw  $0x1,0x4(%ecx)
  0xbb, 0x02, 0x00, 0x00, 0x00,        // 100012:  movl  $0x2,%ebx
  0x66, 0xc7, 0x84, 0x99, 0x00, 0xe0,  // 100017:  movw  $0x1,-0x2000(%ecx,%ebx,4)
  0xff, 0xff, 0x01, 0x00,
  0xb8, 0x00, 0x00, 0x00, 0x00,        // 100021:  movl  $0x0,%eax
  0xd6,                                // 100026:  nemu_trap
};
const long isa_default_img_size = sizeof(isa_default_img);

static void restart() {
  /* Set the initial program counter. 可以让cpu从我们约定的内存位置开始执行客户程序*/
  cpu.pc = PC_START;
  cpu.cs = 0x8;
  cpu.eflags.efl_val = 0x2;
}

void init_isa(void) {
  /* Test the implementation of the `CPU_state' structure. */
  void reg_test(void);
  reg_test();

  /* Setup physical memory address space. 记录物理内存的起始地址 */
  register_pmem(0);

  /* Initialize this virtual computer system. 目前主要功能：初始化寄存器*/
  restart();
}
