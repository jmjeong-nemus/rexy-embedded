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

SRCS= \
	$(LOGIC_BASE)\source\rexy_sysmsg.c \
	$(LOGIC_BASE)\source\rexy_main.c \
	$(LOGIC_BASE)\source\rexy_mainproc.c \
	$(LOGIC_BASE)\source\rexy_state.c

INCS= \
	$(LOGIC_BASE)\header\rexy_main.h \
	$(LOGIC_BASE)\header\rexy_mainproc.h \
	$(LOGIC_BASE)\header\rexy_state.h \
	$(LOGIC_BASE)\header\rexy_sysmsg.h

OBJS= \
	obj\logic\rexy_sysmsg.obj \
	obj\logic\rexy_main.obj \
	obj\logic\rexy_mainproc.obj \
	obj\logic\rexy_state.obj

TARGET=obj\logic.lib

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

{$(LOGIC_BASE)\source\}.c{obj\logic\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

clean:
	del /s/q obj\logic\*.obj
	del /s/q obj\logic.lib

dep:
	mdep $(DFLAGS) -ologic.mak $(SRCS)
	mdep $(DFLAGS) -ologic.mak $(INCS)

# dependencies
..\..\..\logic\source\rexy_sysmsg.c: \
	..\..\..\core\header\rexy.h

..\..\..\logic\source\rexy_main.c: \
	..\..\..\core\header\rexy.h

..\..\..\logic\source\rexy_mainproc.c: \
	..\..\..\core\header\rexy.h

..\..\..\logic\source\rexy_state.c: \
	..\..\..\core\header\rexy.h

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
