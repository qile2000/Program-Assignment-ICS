#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

extern size_t ramdisk_read(void* buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write,0},
  {"stdout", 0, 0, invalid_read, invalid_write,0},
  {"stderr", 0, 0, invalid_read, invalid_write,0},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

size_t get_file_size(int fd){
	return file_table[fd].size;
}

int fs_open(const char *pathname, int flags, int mode) {
	int i;
	for (i = 0; i < NR_FILES; i++) {
		if (strcmp(file_table[i].name, pathname) == 0) {
      file_table[i].open_offset = 0;
			return i;
		}
	}
	assert(0);
	return -1;
}

size_t fs_read(int fd, void *buf, size_t len){
  size_t flsz = get_file_size(fd);
  if(flsz-file_table[fd].open_offset<len){
    len = flsz - file_table[fd].open_offset;
  }
  ramdisk_read(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
  file_table[fd].open_offset=file_table[fd].open_offset+len;
  printf("%s\n",len);
  return len;
}

int fs_close(int fd){
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}


