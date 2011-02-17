.SILENT:

UCOS_BASE=..\device\uCOS
UCOS_ARM_BASE=$(UCOS_BASE)\arm_ucos
FRMWRK_BASE=$(UCOS_ARM_BASE)\s3c2410a

CORE_BASE=..\..\..\core
LOGIC_BASE=..\..\..\logic
HAL_BASE=..
DEVICE_BASE=..\device
APPLICATION_BASE=..\..\..\application
RESOURCE_BASE=..\..\..\resource
CONTROL_BASE=..\..\..\control

C_INC_PATH= -I$(DRIVER_BASE)\inc -I$(UCOS_BASE)\source -I$(UCOS_ARM_BASE)\common\inc -I$(UCOS_BASE)\header -I$(FRMWRK_BASE)\inc -I$(CORE_BASE)\header -I$(LOGIC_BASE)\header -I$(HAL_BASE) -I.. -I..\..\..\application -I..\..\..\resource -I..\..\..\lib\mgl\header -I..\..\..\application\control_demo\header -I..\..\..\application\biorhythm\header -I. -I..\..\..\control\header -I..\..\..\application\window_demo\header
AFLAGS=-via conf-asm.txt -I$(FRMWRK_BASE)\inc
CFLAGS=-via conf-c.txt $(C_INC_PATH)
DFLAGS=$(C_INC_PATH)
LFLAGS=-via conf-link.txt



SRCS= \
	$(CONTROL_BASE)\source\rexy_ctl_anibmp.c \
	$(CONTROL_BASE)\source\rexy_ctl_callback.c \
	$(CONTROL_BASE)\source\rexy_ctl_core.c \
	$(CONTROL_BASE)\source\rexy_ctl_edit.c \
	$(CONTROL_BASE)\source\rexy_ctl_edit.c \
	$(CONTROL_BASE)\source\rexy_ctl_popup.c \
	$(CONTROL_BASE)\source\rexy_ctl_slidetext.c \

INCS= \
	$(CONTROL_BASE)\header\rexy_ctl_anibmp.h \
	$(CONTROL_BASE)\header\rexy_ctl_callback.h \
	$(CONTROL_BASE)\header\rexy_ctl_core.h \
	$(CONTROL_BASE)\header\rexy_ctl_edit.h \
	$(CONTROL_BASE)\header\rexy_ctl_list.h \
	$(CONTROL_BASE)\header\rexy_ctl_popup.h \
	$(CONTROL_BASE)\header\rexy_ctl_slidetext.h \

OBJS= \
	obj\control\rexy_ctl_anibmp.obj \
	obj\control\rexy_ctl_callback.obj \
	obj\control\rexy_ctl_core.obj \
	obj\control\rexy_ctl_edit.obj \
	obj\control\rexy_ctl_list.obj \
	obj\control\rexy_ctl_popup.obj \
	obj\control\rexy_ctl_slidetext.obj \

TARGET=obj\control.lib

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

{$(CONTROL_BASE)\source}.c{obj\control\}.obj:
	armcc -c $(CFLAGS) -o $@ $<


clean:
	del /s/q obj\control\*.obj
	del /s/q obj\control.lib

dep:
	mdep $(DFLAGS) -ocontrol.mak $(SRCS)
	mdep $(DFLAGS) -ocontrol.mak $(INCS)

# dependencies
..\..\..\control\source\rexy_ctl_anibmp.c: \
	..\rexy_haldevice.h \
	..\..\..\control\header\rexy_ctl_anibmp.h \

..\..\..\control\source\rexy_ctl_callback.c: \
	..\..\..\core\header\rexy.h \
	..\..\..\control\header\rexy_ctl_core.h \
	..\..\..\control\header\rexy_ctl_list.h \
	..\..\..\control\header\rexy_ctl_popup.h \
	..\..\..\control\header\rexy_ctl_slidetext.h \
	..\..\..\control\header\rexy_ctl_edit.h \
	..\..\..\control\header\rexy_ctl_anibmp.h \

..\..\..\control\source\rexy_ctl_core.c: \
	..\..\..\core\header\rexy.h \
	..\..\..\control\header\rexy_ctl_core.h \
	..\..\..\control\header\rexy_ctl_edit.h \

..\..\..\control\source\rexy_ctl_edit.c: \
	..\..\..\control\header\rexy_ctl_edit.h \

..\..\..\control\source\rexy_ctl_edit.c: \
	..\..\..\control\header\rexy_ctl_edit.h \

..\..\..\control\source\rexy_ctl_popup.c: \
	..\..\..\control\header\rexy_ctl_popup.h \

..\..\..\control\source\rexy_ctl_slidetext.c: \
	..\..\..\control\header\rexy_ctl_slidetext.h \

..\..\..\control\header\rexy_ctl_anibmp.h: \
	..\..\..\core\header\rexy.h \
	..\..\..\control\header\rexy_ctl_core.h \
