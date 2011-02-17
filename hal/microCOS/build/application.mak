.SILENT:

UCOS_BASE=..\device\uCOS
UCOS_ARM_BASE=$(UCOS_BASE)\arm_ucos
FRMWRK_BASE=$(UCOS_ARM_BASE)\s3c2410a

CORE_BASE=..\..\..\core
LOGIC_BASE=..\..\..\logic
HAL_BASE=..
DEVICE_BASE=..\device
APPLICATION_BASE=..\..\..\application

C_INC_PATH= -I$(DRIVER_BASE)\inc -I$(UCOS_BASE)\source -I$(UCOS_ARM_BASE)\common\inc -I$(UCOS_BASE)\header -I$(FRMWRK_BASE)\inc -I$(CORE_BASE)\header -I$(LOGIC_BASE)\header -I$(HAL_BASE) -I.. -I..\..\..\application -I..\..\..\resource
AFLAGS=-via conf-asm.txt -I$(FRMWRK_BASE)\inc
CFLAGS=-via conf-c.txt $(C_INC_PATH)
DFLAGS=$(C_INC_PATH)
LFLAGS=-via conf-link.txt


SRCS= \
	$(APPLICATION_BASE)\application.c

INCS= \
	$(APPLICATION_BASE)\application.h \
	..\..\..\resource\app_bmpdata.h \
	..\..\..\core\header\rexy.h

OBJS= \
	obj\application\application.obj

TARGET=obj\application.lib

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

{$(APPLICATION_BASE)}.c{obj\application\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

clean:
	del /s/q obj\application\*.obj
	del /s/q obj\application.lib

dep:
	mdep $(DFLAGS) -oapplication.mak $(SRCS)
	mdep $(DFLAGS) -oapplication.mak $(INCS)

# dependencies
..\..\..\application\application.c: \
	..\..\..\application\application.h \

..\..\..\application\application.h: \
	..\..\..\core\header\rexy.h \
	..\..\..\resource\app_bmpdata.h \

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
