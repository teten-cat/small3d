// Reading and Writing to the Framebuffer
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>


int main(int argc, char *argv[]) {
  int fd;
  char *fbp;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;

  fd = open("/dev/fb0", O_RDWR);
  if(fd == -1) {
    perror("Failed to open frambuffer device");
    return 1;
  }


  // Get varaible screen infomation
  if(ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
    perror("Failed to get variable screen info");
    close(fd);
    return 1;
  }

  // Get fixed screen infomation
  if(ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1) {
    perror("Failed to get fixed screen info");
    close(fd);
    return 1;
  }

  int width = vinfo.xres;
  int height = vinfo.yres;
  int bpp = vinfo.bits_per_pixel;
  int line_length = finfo.line_length;
  int screensize = line_length * height;

  fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(fbp == MAP_FAILED) {
    perror("Failed to mmap frambuffer device");
    close(fd);
    return 1;
  }

  // Clear frame buffer
  memset(fbp, 0, sizeof(char)*screensize);

  munmap(fbp, screensize);
  close(fd);
  return 0;
}
