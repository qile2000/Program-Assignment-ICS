#include <am.h>
#include <amdev.h>
#include <nemu.h>
#include <klib.h>

static uint32_t* const fb __attribute__((used)) = (uint32_t *)0x40000;

size_t __am_video_read(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_INFO: {
      _DEV_VIDEO_INFO_t *info = (_DEV_VIDEO_INFO_t *)buf;
      
      info->width = inl(0x100) >> 16;
      info->height = inl(0x100)<<16>>16;
      return sizeof(_DEV_VIDEO_INFO_t);
    }
  }
  return 0;
}

size_t __am_video_write(uintptr_t reg, void *buf, size_t size) {
  switch (reg) {
    case _DEVREG_VIDEO_FBCTL: {
      _DEV_VIDEO_FBCTL_t *ctl = (_DEV_VIDEO_FBCTL_t *)buf;
      
      int i;
      int size = screen_width() * screen_height();
      for (i = 0; i < size; i ++) fb[i] = i;
      
     /*
      int i;
			for(i = 0;i < ctl->h;i ++){
			  memcpy(fb+(ctl->y+i)*screen_width()+ctl->x,ctl->pixels+i*ctl->w,ctl->w*4);
			}
      */
      if (ctl->sync) {
        outl(SYNC_ADDR, 0);
      }
      return size;
      
    }
  }
  return 0;
}

void __am_vga_init() {
  int i;
  int size = screen_width() * screen_height();
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < size; i ++) fb[i] = i;
  draw_sync();
}
/*
      int k = 0, w = screen_width();
      for (int j = ctl->y, ej = j + ctl->h; j != ej; ++j) {
        for (int i = ctl->x, ei = i + ctl->w; i != ei; ++i) {
          fb[j * w + i] = ctl->pixels[k++];
        }
      }
      */