INCLUDES=-Iarm_ucos/s3c2410a/inc -Isource -Iarm_ucos/common/inc -Iheader

CFLAGS=-O2 -g+ -apcs /interwork/noswst -cpu ARM920T -DADS -D__APCS_INTERWORK -DF_REXY_ENV_PHONE $(INCLUDES)

DEP_CFLAGS=-DADS -DF_REXY_ENV_PHONE $(INCLUDES)

all: ../../lib/ucos.a

../../lib/ucos.a: ucos_ii.o
	armar -r ..\..\lib\ucos.a ucos_ii.o

ucos_ii.o: source\ucos_ii.c
	armcc -c $(CFLAGS) -o ucos_ii.o source\ucos_ii.c

dep:
	makedepend $(DEP_CFLAGS) source/ucos_ii.c

# DO NOT DELETE

source/ucos_ii.o: arm_ucos/s3c2410a/inc/includes.h source/ucos_ii.h
source/ucos_ii.o: arm_ucos/s3c2410a/inc/S3C2410A.h
source/ucos_ii.o: source/./os_core.c
source/ucos_ii.o: source/./os_flag.c source/./os_mbox.c source/./os_mem.c
source/ucos_ii.o: source/./os_mutex.c source/./os_q.c source/./os_sem.c
source/ucos_ii.o: source/./os_task.c source/./os_time.c
