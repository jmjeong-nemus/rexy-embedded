.SILENT:

UCOS_BASE=..\device\uCOS
UCOS_ARM_BASE=$(UCOS_BASE)\arm_ucos
FRMWRK_BASE=$(UCOS_ARM_BASE)\s3c2410a

CORE_BASE=..\..\..\core
LOGIC_BASE=..\..\..\logic
HAL_BASE=..

AFLAGS=-via conf-asm.txt -I$(FRMWRK_BASE)\inc
CFLAGS=-via conf-c.txt -I$(DRIVER_BASE)\inc -I$(UCOS_BASE)\source -I$(UCOS_ARM_BASE)\common\inc -I$(UCOS_BASE)\header -I$(FRMWRK_BASE)\inc -I$(CORE_BASE)\header -I$(LOGIC_BASE)\header -I$(HAL_BASE)
DFLAGS=-I$(DRIVER_BASE)\inc -I$(UCOS_BASE)\source -I$(UCOS_ARM_BASE)\common\inc -I$(UCOS_BASE)\header -I$(FRMWRK_BASE)\inc -I$(CORE_BASE)\header -I$(LOGIC_BASE)\header -I$(HAL_BASE)
LFLAGS=-via conf-link.txt


SRCS=\
	$(CORE_BASE)\source\rexy_winalloc.c\
	$(CORE_BASE)\source\rexy_font.c\
	$(CORE_BASE)\source\rexy_gdi.c\
	$(CORE_BASE)\source\rexy_mem.c\
	$(CORE_BASE)\source\rexy_msg.c\
	$(CORE_BASE)\source\rexy_msgq.c\
	$(CORE_BASE)\source\rexy_resource.c\
	$(CORE_BASE)\source\rexy_string.c\
	$(CORE_BASE)\source\rexy_trace.c\
	$(CORE_BASE)\source\rexy_user.c\
	$(CORE_BASE)\source\rexy_util.c\
	$(CORE_BASE)\source\rexy_win.c\
	$(CORE_BASE)\source\rexy_diag.c\
	$(CORE_BASE)\source\rexy_lock.c\
	$(CORE_BASE)\source\rexy_feature.c

INCS= \
	$(CORE_BASE)\header\rexy.h \
	$(CORE_BASE)\header\rexy_common.h \
	$(CORE_BASE)\header\rexy_const.h \
	$(CORE_BASE)\header\rexy_device.h \
	$(CORE_BASE)\header\rexy_diag.h \
	$(CORE_BASE)\header\rexy_error.h \
	$(CORE_BASE)\header\rexy_feature.h \
	$(CORE_BASE)\header\rexy_font.h \
	$(CORE_BASE)\header\rexy_gdi.h \
	$(CORE_BASE)\header\rexy_halinterface.h \
	$(CORE_BASE)\header\rexy_io.h \
	$(CORE_BASE)\header\rexy_lock.h \
	$(CORE_BASE)\header\rexy_mem.h \
	$(CORE_BASE)\header\rexy_msg.h \
	$(CORE_BASE)\header\rexy_msgq.h \
	$(CORE_BASE)\header\rexy_resource.h \
	$(CORE_BASE)\header\rexy_string.h \
	$(CORE_BASE)\header\rexy_trace.h \
	$(CORE_BASE)\header\rexy_type.h \
	$(CORE_BASE)\header\rexy_user.h \
	$(CORE_BASE)\header\rexy_util.h \
	$(CORE_BASE)\header\rexy_win.h \
	$(CORE_BASE)\header\rexy_winalloc.h

OBJS= \
	obj\core\rexy_winalloc.obj \
	obj\core\rexy_font.obj \
	obj\core\rexy_gdi.obj \
	obj\core\rexy_mem.obj \
	obj\core\rexy_msg.obj \
	obj\core\rexy_msgq.obj \
	obj\core\rexy_resource.obj \
	obj\core\rexy_string.obj \
	obj\core\rexy_trace.obj \
	obj\core\rexy_user.obj \
	obj\core\rexy_util.obj \
	obj\core\rexy_win.obj \
	obj\core\rexy_diag.obj \
	obj\core\rexy_lock.obj \
	obj\core\rexy_feature.obj

TARGET=obj\core.lib

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

{$(CORE_BASE)\source\}.c{obj\core\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

clean:
	del /s/q obj\core\*.obj
	del /s/q obj\core.lib

dep:
	mdep $(DFLAGS) -ocore.mak $(SRCS)
	mdep $(DFLAGS) -ocore.mak $(INCS)

# dependencies
..\..\..\core\source\rexy_winalloc.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_font.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_gdi.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_mem.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_msg.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_msgq.c: \
	..\..\..\core\header\rexy.h \
	..\..\..\core\header\rexy_msgq.h \

..\..\..\core\source\rexy_resource.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_string.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_trace.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_user.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_util.c: \
	..\..\..\core\header\rexy.h \
	..\..\..\core\header\rexy_util.h \

..\..\..\core\source\rexy_win.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_diag.c: \
	..\..\..\core\header\rexy.h \
	..\..\..\core\header\rexy_halinterface.h \

..\..\..\core\source\rexy_lock.c: \
	..\..\..\core\header\rexy.h \

..\..\..\core\source\rexy_feature.c: \
	..\..\..\core\header\rexy.h \
	..\..\..\core\header\rexy_feature.h \

..\..\..\core\header\rexy.h: \
	..\..\..\core\header\rexy_error.h \
	..\..\..\core\header\rexy_mem.h \
	..\..\..\core\header\rexy_feature.h \
	..\rexy_haltype.h \
	..\..\..\core\header\rexy_type.h \
	..\..\..\core\header\rexy_msg.h \
	..\..\..\core\header\rexy_lock.h \
	..\..\..\core\header\rexy_resource.h \
	..\..\..\core\header\rexy_win.h \
	..\..\..\core\header\rexy_user.h \
	..\..\..\logic\header\rexy_mainproc.h \
	..\..\..\logic\header\rexy_main.h \
	..\..\..\core\header\rexy_common.h \
	..\..\..\core\header\rexy_trace.h \
	..\..\..\core\header\rexy_device.h \
	..\..\..\core\header\rexy_const.h \
	..\..\..\core\header\rexy_winalloc.h \
	..\..\..\core\header\rexy_msg.h \
	..\..\..\core\header\rexy_msgq.h \
	..\..\..\core\header\rexy_user.h \
	..\..\..\core\header\rexy_font.h \
	..\..\..\core\header\rexy_util.h \
	..\..\..\core\header\rexy_string.h \
	..\..\..\logic\header\rexy_sysmsg.h \
	..\..\..\logic\header\rexy_state.h \
	..\..\..\core\header\rexy_feature.h \
	..\..\..\core\header\rexy_diag.h \
	..\rexy_haldevice.h \
	..\..\..\core\header\rexy_halinterface.h \

..\..\..\core\header\rexy_common.h: \
	..\..\..\core\header\rexy_gdi.h \

..\..\..\core\header\rexy_device.h: \
	..\..\..\core\header\rexy_type.h \

..\..\..\core\header\rexy_io.h: \
	..\..\..\core\header\rexy.h \

..\..\..\core\header\rexy_resource.h: \
	..\..\..\core\header\rexy_font.h \

..\..\..\core\header\rexy_winalloc.h: \
	..\..\..\core\header\rexy_type.h \
