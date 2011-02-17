.SILENT:

UCOS_BASE=..\device\uCOS
UCOS_ARM_BASE=$(UCOS_BASE)\arm_ucos
FRMWRK_BASE=$(UCOS_ARM_BASE)\s3c2410a

CORE_BASE=..\..\..\core
LOGIC_BASE=..\..\..\logic
HAL_BASE=..
DEVICE_BASE=..\device

AFLAGS=-via conf-asm.txt -I$(FRMWRK_BASE)\inc
CFLAGS=-via conf-c.txt -I$(DRIVER_BASE)\inc -I$(UCOS_BASE)\source -I$(UCOS_ARM_BASE)\common\inc -I$(UCOS_BASE)\header -I$(FRMWRK_BASE)\inc -I$(CORE_BASE)\header -I$(LOGIC_BASE)\header -I$(HAL_BASE) -I..
DFLAGS=-I$(DRIVER_BASE)\inc -I$(UCOS_BASE)\source -I$(UCOS_ARM_BASE)\common\inc -I$(UCOS_BASE)\header -I$(FRMWRK_BASE)\inc -I$(CORE_BASE)\header -I$(LOGIC_BASE)\header -I$(HAL_BASE) -I..
LFLAGS=-via conf-link.txt


SRCS= \
	$(DEVICE_BASE)\startup.c \
	$(DEVICE_BASE)\rexy_clib.c \
	$(DEVICE_BASE)\rexy_halinterface.c	

INCS= \
	$(DEVICE_BASE)\..\rexy_haldevice.h \
	$(DEVICE_BASE)\..\rexy_haltype.h \
	$(DEVICE_BASE)\..\rexy_clib.h \
	$(DEVICE_BASE)\..\rexy_halinterface.h \
	$(DEVICE_BASE)\..\rexy_resource.h

OBJS= \
	obj\device\startup.obj \
	obj\device\rexy_clib.obj \
	obj\device\rexy_halinterface.obj

TARGET=obj\device.lib

all: init $(TARGET)

init:
	echo setting environment variables.
	set ARMHOME=C:\Program Files\ARM\ADSv1_2
	set ARMCONF=%ARMHOME%\BIN
	set ARMDLL=%ARMHOME%\BIN
	set ARMINC=%ARMHOME%\INCLUDE
	set ARMLIB=%ARMHOME%\LIB
	set PATH=%PATH%;%ARMDLL%;

$(TARGET): $(OBJS)
	armar rcs $(TARGET) $(OBJS)

{$(DEVICE_BASE)}.c{obj\device\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

clean:
	del /s/q obj\device\*.obj
	del /s/q obj\device.lib

dep:
	mdep $(DFLAGS) -odevice.mak $(SRCS)
	mdep $(DFLAGS) -odevice.mak $(INCS)

# dependencies
..\device\startup.c: \
	..\..\..\core\header\rexy.h \
	..\device\uCOS\arm_ucos\s3c2410a\inc\includes.h \
	..\device\uCOS\arm_ucos\s3c2410a\inc\consol.h \
	..\device\uCOS\arm_ucos\s3c2410a\inc\HwrDisplay.h \
	..\device\uCOS\arm_ucos\s3c2410a\inc\HwrTimer.h \
	..\device\uCOS\arm_ucos\s3c2410a\inc\queue.h \
	..\device\uCOS\arm_ucos\s3c2410a\inc\HwrKeyMgr.h \
	..\device\uCOS\arm_ucos\s3c2410a\inc\HwrRTCTimer.h \

..\device\rexy_clib.c: \
	..\..\..\core\header\rexy.h \

..\device\rexy_halinterface.c: \
	..\..\..\core\header\rexy.h \
	..\device\uCOS\arm_ucos\s3c2410a\inc\HwrRTCTimer.h \
	..\device\uCOS\arm_ucos\s3c2410a\inc\HwrKeyMgr.h \
	..\device\uCOS\arm_ucos\s3c2410a\inc\queue.h \

$(DRIVER_BASE)\inc\includes.h: \
	$(UCOS_ARM_BASE)\common\inc\os_cpu.h \
	$(UCOS_BASE)\header\os_cfg.h \
	$(UCOS_BASE)\source\ucos_ii.h \
	$(UCOS_ARM_BASE)\s3c2410a\inc\S3C2410A.h \
	$(UCOS_ARM_BASE)\common\inc\def.h

$(DRIVER_BASE)\inc\HwrKeyMgr.h: \
	$(DRIVER_BASE)\inc\includes.h

$(DRIVER_BASE)\inc\HwrRTCTimer.h: \
	$(DRIVER_BASE)\inc\includes.h

$(DRIVER_BASE)\inc\HwrTimer.h: \
	$(DRIVER_BASE)\inc\includes.h \
