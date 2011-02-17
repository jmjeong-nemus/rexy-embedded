.SILENT:

UCOS_BASE=..\device\uCOS
UCOS_ARM_BASE=$(UCOS_BASE)\arm_ucos

AFLAGS=-via conf-asm.txt
CFLAGS=-via conf-c.txt -I$(UCOS_ARM_BASE)\s3c2410a\inc -I$(UCOS_BASE)\source -I$(UCOS_ARM_BASE)\common\inc -I$(UCOS_BASE)\header
LFLAGS=-via conf-link.txt

UCOS_OBJS= \
	obj\ucos\ucos_ii.obj \
	obj\ucos\os_cpu_a.obj \
	obj\ucos\os_cpu_c.obj

TARGET= obj\ucos.lib


all: init $(TARGET)

init:
	echo setting environment variables.
	set ARMHOME=C:\Program Files\ARM\ADSv1_2
	set ARMCONF=%ARMHOME%\BIN
	set ARMDLL=%ARMHOME%\BIN
	set ARMINC=%ARMHOME%\INCLUDE
	set ARMLIB=%ARMHOME%\LIB
	set PATH=%PATH%;%ARMDLL%;

$(TARGET): $(UCOS_OBJS) 
	armar rcs $(TARGET) $(UCOS_OBJS)

obj\ucos\ucos_ii.obj:$(UCOS_BASE)\source\ucos_ii.c
	armcc -c $(CFLAGS) -o obj\ucos\ucos_ii.obj $(UCOS_BASE)\source\ucos_ii.c

obj\ucos\os_cpu_c.obj:$(UCOS_ARM_BASE)\common\src\os_cpu_c.c
	armcc -c $(CFLAGS) -o obj\ucos\os_cpu_c.obj $(UCOS_ARM_BASE)\common\src\os_cpu_c.c

obj\ucos\os_cpu_a.obj:$(UCOS_ARM_BASE)\common\src\os_cpu_a.s
	armasm $(AFLAGS) -o obj\ucos\os_cpu_a.obj $(UCOS_ARM_BASE)\common\src\os_cpu_a.s

clean:
	del /s/q obj\ucos\*.obj
	del /s/q obj\ucos.lib

# dependencies

$(UCOS_BASE)\arm_ucos\s3c2410a\inc\includes.h: \
	$(UCOS_ARM_BASE)\common\inc\os_cpu.h \
	$(UCOS_BASE)\header\os_cfg.h \
	$(UCOS_BASE)\source\ucos_ii.h \
	$(UCOS_ARM_BASE)\s3c2410a\inc\S3C2410A.h \
	$(UCOS_ARM_BASE)\common\inc\def.h

$(UCOS_BASE)\source\os_core.c: \
	$(UCOS_ARM_BASE)\s3c2410a\inc\includes.h

$(UCOS_BASE)\source\os_flag.c: \
	$(UCOS_ARM_BASE)\s3c2410a\inc\includes.h

$(UCOS_BASE)\source\os_mbox.c: \
	$(UCOS_ARM_BASE)\s3c2410a\inc\includes.h

$(UCOS_BASE)\source\os_mem.c: \
	$(UCOS_ARM_BASE)\s3c2410a\inc\includes.h

$(UCOS_BASE)\source\os_mutex.c: \
	$(UCOS_ARM_BASE)\s3c2410a\inc\includes.h

$(UCOS_BASE)\source\os_q.c: \
	$(UCOS_ARM_BASE)\s3c2410a\inc\includes.h

$(UCOS_BASE)\source\os_sem.c: \
	$(UCOS_ARM_BASE)\s3c2410a\inc\includes.h

$(UCOS_BASE)\source\os_task.c: \
	$(UCOS_ARM_BASE)\s3c2410a\inc\includes.h

$(UCOS_BASE)\source\os_time.c:
	$(UCOS_ARM_BASE)\s3c2410a\inc\includes.h

$(UCOS_SOURCE_DIR)\ucos_ii.c:\
	$(UCOS_ARM_BASE)\s3c2410a\inc\includes.h \
	$(UCOS_BASE)\source\os_core.c \
	$(UCOS_BASE)\source\os_flag.c \
	$(UCOS_BASE)\source\os_mbox.c \
	$(UCOS_BASE)\source\os_mem.c \
	$(UCOS_BASE)\source\os_mutex.c \
	$(UCOS_BASE)\source\os_q.c \
	$(UCOS_BASE)\source\os_sem.c \
	$(UCOS_BASE)\source\os_task.c \
	$(UCOS_BASE)\source\os_time.c
