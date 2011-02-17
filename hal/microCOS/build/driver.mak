.SILENT:

UCOS_BASE=..\device\uCOS
UCOS_ARM_BASE=$(UCOS_BASE)\arm_ucos
DRIVER_BASE=$(UCOS_ARM_BASE)\s3c2410a

AFLAGS=-via conf-asm.txt
CFLAGS=-via conf-c.txt -I$(DRIVER_BASE)\inc -I$(UCOS_BASE)\source -I$(UCOS_ARM_BASE)\common\inc -I$(UCOS_BASE)\header
LFLAGS=-via conf-link.txt

DRIVER_OBJS= \
	obj\driver\HwrDisplay.obj \
	obj\driver\HwrInterrupts.obj \
	obj\driver\HwrKeyMgr.obj \
	obj\driver\HwrRTCTimer.obj \
	obj\driver\HwrTimer.obj \
	obj\driver\queue.obj

TARGET= obj\driver.lib


all: init $(TARGET)

init:
	echo setting environment variables.
	set ARMHOME=C:\Program Files\ARM\ADSv1_2
	set ARMCONF=%ARMHOME%\BIN
	set ARMDLL=%ARMHOME%\BIN
	set ARMINC=%ARMHOME%\INCLUDE
	set ARMLIB=%ARMHOME%\LIB
	set PATH=%PATH%;%ARMDLL%;

$(TARGET): $(DRIVER_OBJS) 
	armar rcs $(TARGET) $(DRIVER_OBJS)

clean:
	del /s/q obj\driver\*.obj
	del /s/q obj\driver.lib

{$(DRIVER_BASE)\src\}.c{obj\driver\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

obj\driver\HwrDisplay.obj: $(DRIVER_BASE)\src\HwrDisplay.c
obj\driver\HwrInterrupts.obj:$(DRIVER_BASE)\src\HwrInterrupts.c
obj\driver\HwrKeyMgr.obj:$(DRIVER_BASE)\src\HwrKeyMgr.c
obj\driver\HwrRTCTimer.obj:$(DRIVER_BASE)\src\HwrRTCTimer.c
obj\driver\HwrTimer.obj:$(DRIVER_BASE)\src\HwrTimer.c
obj\driver\queue.obj:$(DRIVER_BASE)\src\queue.c

# dependencies

$(DRIVER_BASE)\inc\includes.h: \
	$(UCOS_ARM_BASE)\common\inc\os_cpu.h \
	$(UCOS_BASE)\header\os_cfg.h \
	$(UCOS_BASE)\source\ucos_ii.h \
	$(UCOS_ARM_BASE)\s3c2410a\inc\S3C2410A.h \
	$(UCOS_ARM_BASE)\common\inc\def.h

$(DRIVER_BASE)\src\HwrDisplay.c: \
	$(DRIVER_BASE)\inc\includes.h \
	$(DRIVER_BASE)\inc\HwrDisplay.h

$(DRIVER_BASE)\inc\HwrTimer.h: \
	$(DRIVER_BASE)\inc\includes.h \

$(DRIVER_BASE)\inc\frmwrk.h: \
	$(UCOS_ARM_BASE)\common\inc\def.h

$(DRIVER_BASE)\src\HwrInterrupts.c: \
	$(DRIVER_BASE)\inc\includes.h \
	$(DRIVER_BASE)\inc\HwrTimer.h \
	$(DRIVER_BASE)\inc\frmwrk.h

$(DRIVER_BASE)\inc\HwrKeyMgr.h: \
	$(DRIVER_BASE)\inc\includes.h

$(DRIVER_BASE)\inc\HwrRTCTimer.h: \
	$(DRIVER_BASE)\inc\includes.h

$(DRIVER_BASE)\src\HwrKeyMgr.c: \
	$(DRIVER_BASE)\inc\includes.h \
	$(DRIVER_BASE)\inc\HwrDisplay.h \
	$(DRIVER_BASE)\inc\frmwrk.h \
	$(DRIVER_BASE)\inc\HwrKeyMgr.h \
	$(DRIVER_BASE)\inc\HwrTimer.h \
	$(DRIVER_BASE)\inc\queue.h \
	$(DRIVER_BASE)\inc\consol.h

$(DRIVER_BASE)\src\HwrRTCTimer.c: \
	$(DRIVER_BASE)\inc\includes.h \
	$(DRIVER_BASE)\inc\frmwrk.h \
	$(DRIVER_BASE)\inc\HwrRTCTimer.h

$(DRIVER_BASE)\src\HwrTimer.c: \
	$(DRIVER_BASE)\inc\includes.h \
	$(DRIVER_BASE)\inc\frmwrk.h \
	$(DRIVER_BASE)\inc\HwrKeyMgr.h \
	$(DRIVER_BASE)\inc\HwrTimer.h

$(DRIVER_BASE)\src\queue.c: \
	$(DRIVER_BASE)\inc\includes.h \
	$(DRIVER_BASE)\inc\frmwrk.h \
	$(DRIVER_BASE)\inc\queue.h



