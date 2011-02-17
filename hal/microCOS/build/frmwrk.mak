.SILENT:

UCOS_BASE=..\device\uCOS
UCOS_ARM_BASE=$(UCOS_BASE)\arm_ucos
FRMWRK_BASE=$(UCOS_ARM_BASE)\s3c2410a

AFLAGS=-via conf-asm.txt -I$(FRMWRK_BASE)\inc
CFLAGS=-via conf-c.txt -I$(DRIVER_BASE)\inc -I$(UCOS_BASE)\source -I$(UCOS_ARM_BASE)\common\inc -I$(UCOS_BASE)\header -I$(FRMWRK_BASE)\inc
LFLAGS=-via conf-link.txt

FRMWRK_OBJS= \
	obj\frmwrk\frmwrk.obj \
	obj\frmwrk\mmu.obj \
	obj\frmwrk\mmulib.obj \
	obj\frmwrk\consol.obj

TARGET= obj\frmwrk.lib


all: init $(TARGET)

init:
	echo setting environment variables.
	set ARMHOME=C:\Program Files\ARM\ADSv1_2
	set ARMCONF=%ARMHOME%\BIN
	set ARMDLL=%ARMHOME%\BIN
	set ARMINC=%ARMHOME%\INCLUDE
	set ARMLIB=%ARMHOME%\LIB
	set PATH=%PATH%;%ARMDLL%;

$(TARGET): $(FRMWRK_OBJS) 
	armar rcs $(TARGET) $(FRMWRK_OBJS)

clean:
	del /s/q obj\frmwrk\*.obj
	del /s/q obj\frmwrk.lib

{$(FRMWRK_BASE)\src\}.c{obj\frmwrk\}.obj:
	armcc -c $(CFLAGS) -o $@ $<


obj\frmwrk\mmulib.obj:$(FRMWRK_BASE)\src\mmulib.s
	armasm $(AFLAGS) -o obj\frmwrk\mmulib.obj $(FRMWRK_BASE)\src\mmulib.s

obj\frmwrk\frmwrk.obj:$(FRMWRK_BASE)\src\frmwrk.c
obj\frmwrk\mmu.obj:$(FRMWRK_BASE)\src\mmu.c
obj\frmwrk\consol.obj:$(FRMWRK_BASE)\src\consol.c


# dependencies

$(DRIVER_BASE)\inc\includes.h: \
	$(UCOS_ARM_BASE)\common\inc\os_cpu.h \
	$(UCOS_BASE)\header\os_cfg.h \
	$(UCOS_BASE)\source\ucos_ii.h \
	$(UCOS_ARM_BASE)\s3c2410a\inc\S3C2410A.h \
	$(UCOS_ARM_BASE)\common\inc\def.h

$(FRMWRK_BASE)\src\frmwrk.c:\
	$(DRIVER_BASE)\inc\includes.h \
	$(FRMWRK_BASE)\inc\frmwrk.h \
	$(FRMWRK_BASE)\inc\consol.h

$(FRMWRK_BASE)\inc\mmu.h:\
	$(FRMWRK_BASE)\inc\mmulib.h

$(FRMWRK_BASE)\src\mmu.c:\
	$(UCOS_ARM_BASE)\common\inc\def.h \
	$(FRMWRK_BASE)\inc\frmwrk.h \
	$(FRMWRK_BASE)\inc\s3c2410x.h \
	$(FRMWRK_BASE)\inc\mmu.h

$(FRMWRK_BASE)\src\consol.c:\
	$(UCOS_ARM_BASE)\common\inc\def.h \
	$(FRMWRK_BASE)\inc\s3c2410x.h \
	$(FRMWRK_BASE)\inc\consol.h


