#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

extern size_t serial_write(const void *buf, size_t offset, size_t len);
extern size_t ramdisk_read(void* buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void* buf, size_t offset, size_t len);
extern size_t events_read(void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
  size_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENTS};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("invalid_read,should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("invalid_write,should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin", 0, 0, invalid_read, invalid_write, 0},
  {"stdout", 0, 0, invalid_read, serial_write, 0},
  {"stderr", 0, 0, invalid_read, serial_write, 0},
  {"/dev/fb", 0, 0, invalid_read, invalid_write, 0},
  {"/dev/events", 0, 0, events_read, invalid_write, 0},
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
      		//printf("file no.%d: %s\n",i,file_table[i].name);
      		//file_table[i].open_offset = 0;
			return i;
		}
	}
	assert(0);
	return -1;
}

size_t fs_read(int fd, void *buf, size_t len){
 /*
  size_t flsz = get_file_size(fd);
  if(flsz-file_table[fd].open_offset<len){
    len = flsz - file_table[fd].open_offset;
  }
  ramdisk_read(buf, \
  			   file_table[fd].disk_offset+file_table[fd].open_offset, \
			   len);
  file_table[fd].open_offset=file_table[fd].open_offset+len;
  //printf("len:%d\n",len);
  return len;

*/
/*
    if(file_table[fd].read==NULL){
		size_t fz=get_file_size(fd);
		if(fz-file_table[fd].open_offset<len){
			len = fz - file_table[fd].open_offset;
		}
		ramdisk_read(buf, file_table[fd].disk_offset+file_table[fd].open_offset, len);
		file_table[fd].open_offset += len;
		return len;
	}
	else{
		size_t l=file_table[fd].read(buf, file_table[fd].open_offset, len);
		file_table[fd].open_offset+=len;
		return l;
	}
	*/
	switch(fd){
		case FD_EVENTS:
			printf("EVENTS_READ\n");
			return events_read(buf,0,len);
		default:{
			size_t flsz = get_file_size(fd);
  			if(flsz-file_table[fd].open_offset<len){
    			len = flsz - file_table[fd].open_offset;
  			}
  			ramdisk_read(buf, \
  			   			file_table[fd].disk_offset+file_table[fd].open_offset, \
			   			len);
  			file_table[fd].open_offset=file_table[fd].open_offset+len;
  			//printf("len:%d\n",len);
  			return len;
		}
	}
}

size_t fs_write(int fd, const void *buf, size_t len){
  /*
  if(fd==FD_STDOUT || fd==FD_STDERR){
	char* _buf=(char*)buf;
	for(int i=0;i<len;i++){
		_putc(_buf[i]);
    }
	return len;
  }
  */
 
  if(fd==FD_STDOUT || fd==FD_STDERR ){//|| strcmp(file_table[fd].name,"/dev/events")){
	uint32_t filelen=file_table[fd].write(buf, 0, len);
	return filelen;
  }
  size_t flsz=get_file_size(fd);
  if(flsz-file_table[fd].open_offset<len){
	len = flsz - file_table[fd].open_offset;
  }
  ramdisk_write(buf, \
  				file_table[fd].disk_offset+file_table[fd].open_offset, \
				len);
  file_table[fd].open_offset=file_table[fd].open_offset+len;
  return len;
}
size_t fs_lseek(int fd,size_t offset,int whence){
	switch(whence){
		case SEEK_SET: 
			if(offset<0 || offset>file_table[fd].size)
				return -1;
			file_table[fd].open_offset = offset;
			return file_table[fd].open_offset;
		case SEEK_CUR:
			if(file_table[fd].open_offset+offset<0||file_table[fd].open_offset+offset>file_table[fd].size)
				return -1;
			file_table[fd].open_offset += offset;
			return file_table[fd].open_offset;
		case SEEK_END:
			if(file_table[fd].size+offset<0)
				return -1;
			file_table[fd].open_offset = file_table[fd].size+offset;
			return file_table[fd].open_offset;
		default:
			assert(0);
			return 0;
 	}
}

int fs_close(int fd){
  return 0;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}


