#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#define FBDEVFILE "/dev/fb"

#define DEVICE_HEIGHT	320
#define DEVICE_WIDTH	240

static UNITCL frame_buffer[DEVICE_HEIGHT][DEVICE_WIDTH] = {0,};

int fbScrFlushAll(void* lcdBuffer)
{
	int fbfd;
	int ret;
	struct fb_var_screeninfo fvar;
	unsigned short pixel;
	int offset;
	unsigned short *pfbdata;
	int xpos, ypos;


	fbfd = open(FBDEVFILE, O_RDWR);
	if(fbfd < 0)
	{
		perror("fbdev open");
		exit(1);
	}

	ret = ioctl(fbfd, FBIOGET_VSCREENINFO, &fbvar);
	if(ret <0)
	{
		perror("fbdev ioctl");
		exit(1);
	}

	pfbdata = (unsigned short *)mmap(0, fbvar.xres*fbvar.yres*16/8,PROT_READ|PROT_WRITE, MAP_SHARED, fbfd, 0);

	if((unsigned)pfbdata == (unsigned)-1)
	{
		perror("fbdev mmap");
		exit(1);
	}

	frame_buffer = lcdBuffer;

	for(ypos = 0; ypos < DEVICE_HEIGHT; ypos++)
	{
		for(xpos = 0; xpos < DEVICE_WIDTH, xpos++)
		{
			pfbdata[DEVICE_WIDTH*ypos+xpos] = lcdBuffer[ypos][xpos];
		}
	}
	
	munmap(pfbdata, fbvar.xres*fbvar.yres*16/8);
	close(fbfd);
}
