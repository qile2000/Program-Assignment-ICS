#include "memory/memory.h"
#include "device/map.h"
#include "nemu.h"

#define IO_SPACE_MAX (1024 * 1024)
//实现了映射的管理, 包括I/O空间的分配及其映射, 还有映射的访问接口
static uint8_t io_space[IO_SPACE_MAX] PG_ALIGN = {};
static uint8_t *p_space = io_space;

uint8_t* new_space(int size) {
  uint8_t *p = p_space;
  // page aligned;
  size = (size + (PAGE_SIZE - 1)) & ~PAGE_MASK;
  p_space += size;
  assert(p_space - io_space < IO_SPACE_MAX);
  return p;
}

static inline void check_bound(IOMap *map, paddr_t addr) {
  Assert(map != NULL && addr <= map->high && addr >= map->low,
      "address (0x%08x) is out of bound {%s} [0x%08x, 0x%08x] at pc = 0x%08x",
      addr, (map ? map->name : "???"), (map ? map->low : 0), (map ? map->high : 0), cpu.pc);
}

static inline void invoke_callback(io_callback_t c, uint32_t offset, int len, bool is_write) {
  if (c != NULL) { c(offset, len, is_write); }
}

//map_read()和map_write()用于将地址addr映射到map所指示的目标空间, 并进行访问. 
//访问时, 可能会触发相应的回调函数, 对设备和目标空间的状态进行更新
//由于NEMU是单线程程序, 因此只能串行模拟整个计算机系统的工作, 
//每次进行I/O读写的时候, 才会调用设备提供的回调函数(callback).
uint32_t map_read(paddr_t addr, int len, IOMap *map) {
  printf("map_read");
  assert(len >= 1 && len <= 4);
  check_bound(map, addr);
  uint32_t offset = addr - map->low;
  invoke_callback(map->callback, offset, len, false); // prepare data to read

  uint32_t data = *(uint32_t *)(map->space + offset) & (~0u >> ((4 - len) << 3));
  return data;
}

void map_write(paddr_t addr, uint32_t data, int len, IOMap *map) {
  assert(len >= 1 && len <= 4);
  check_bound(map, addr);
  uint32_t offset = addr - map->low;

  memcpy(map->space + offset, &data, len);

  invoke_callback(map->callback, offset, len, true);
}
