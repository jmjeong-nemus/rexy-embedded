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

C_INC_PATH= -I$(DRIVER_BASE)\inc -I$(UCOS_BASE)\source -I$(UCOS_ARM_BASE)\common\inc -I$(UCOS_BASE)\header -I$(FRMWRK_BASE)\inc -I$(CORE_BASE)\header -I$(LOGIC_BASE)\header -I$(HAL_BASE) -I.. -I..\..\..\application -I..\..\..\resource -I..\..\..\lib\mgl\header -I..\..\..\application\control_demo\header -I..\..\..\application\biorhythm\header -I. -I..\..\..\control\header -I..\..\..\application\window_demo\header -I..\..\..\addin\rexyDB
AFLAGS=-via conf-asm.txt -I$(FRMWRK_BASE)\inc
CFLAGS=-via conf-c.txt $(C_INC_PATH)
DFLAGS=$(C_INC_PATH)
LFLAGS=-via conf-link.txt


SRCS= \
	$(APPLICATION_BASE)\mgl\app_menu1.c \
	$(APPLICATION_BASE)\mgl\app_menu2.c \
	$(APPLICATION_BASE)\mgl\app_menu3.c \
	$(APPLICATION_BASE)\mgl\app_menu4.c \
	$(APPLICATION_BASE)\mgl\app_menu5.c \
	$(APPLICATION_BASE)\mgl\app_mgl_idle.c \
	$(APPLICATION_BASE)\mgl\layer\mgl_il_rexy.c \
	$(APPLICATION_BASE)\control_demo\source\sample_anibmp.c \
	$(APPLICATION_BASE)\control_demo\source\sample_control.c \
	$(APPLICATION_BASE)\control_demo\source\sample_edit.c \
	$(APPLICATION_BASE)\control_demo\source\sample_list.c \
	$(APPLICATION_BASE)\control_demo\source\sample_popup.c \
	$(APPLICATION_BASE)\control_demo\source\sample_slidetext.c \
	$(APPLICATION_BASE)\tetris\source\tetris.c \
	$(APPLICATION_BASE)\title\idle.c \
	$(APPLICATION_BASE)\biorhythm\source\biorhythm.c \
	$(APPLICATION_BASE)\window_demo\source\win_demo.c \
	$(APPLICATION_BASE)\mainmenu\mainmenu.c \
	$(APPLICATION_BASE)\mainmenu\mainmenu.c_anibmp.c \

INCS= \
	$(APPLICATION_BASE)\control_demo\header\sample_anibmp.h \
	$(APPLICATION_BASE)\control_demo\header\sample_control.h \
	$(APPLICATION_BASE)\control_demo\header\sample_edit.h \
	$(APPLICATION_BASE)\control_demo\header\sample_list.h \
	$(APPLICATION_BASE)\control_demo\header\sample_popup.h \
	$(APPLICATION_BASE)\control_demo\header\sample_slidetext.h \
	$(APPLICATION_BASE)\tetris\header\tetris.h \
	$(APPLICATION_BASE)\biorhythm\header\biorhythm.h \
	$(APPLICATION_BASE)\window_demo\header\win_demo.h \
	$(APPLICATION_BASE)\mainmenu\mainmenu.h \
	$(APPLICATION_BASE)\mainmenu\mainmenu_anibmp.h \


OBJS= \
	obj\sample\app_menu1.obj \
	obj\sample\app_menu2.obj \
	obj\sample\app_menu3.obj \
	obj\sample\app_menu4.obj \
	obj\sample\app_menu5.obj \
	obj\sample\app_mgl_idle.obj \
	obj\sample\mgl_il_rexy.obj \
	obj\sample\sample_anibmp.obj \
	obj\sample\sample_control.obj \
	obj\sample\sample_edit.obj \
	obj\sample\sample_list.obj \
	obj\sample\sample_popup.obj \
	obj\sample\sample_slidetext.obj \
	obj\sample\tetris.obj \
	obj\sample\idle.obj \
	obj\sample\biorhythm.obj \
	obj\sample\win_demo.obj \
	obj\sample\mainmenu.obj \
	obj\sample\mainmenu_anibmp.obj \

TARGET=obj\sample.lib

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

{$(APPLICATION_BASE)\mgl}.c{obj\sample\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

{$(APPLICATION_BASE)\mgl\layer}.c{obj\sample\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

{$(APPLICATION_BASE)\control_demo\source}.c{obj\sample\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

{$(APPLICATION_BASE)\tetris\source}.c{obj\sample\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

{$(APPLICATION_BASE)\biorhythm\source}.c{obj\sample\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

{$(APPLICATION_BASE)\title}.c{obj\sample\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

{$(APPLICATION_BASE)\window_demo\source}.c{obj\sample\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

{$(APPLICATION_BASE)\mainmenu}.c{obj\sample\}.obj:
	armcc -c $(CFLAGS) -o $@ $<

clean:
	del /s/q obj\sample\*.obj
	del /s/q obj\sample.lib

dep:
	mdep $(DFLAGS) -osample.mak $(SRCS)
	mdep $(DFLAGS) -osample.mak $(INCS)

# dependencies
..\..\..\application\mgl\app_menu1.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\lib\mgl\header\mgl.h \
	..\..\..\lib\mgl\header\mgl_ip.h \
	..\..\..\lib\mgl\header\mgl_animator.h \
	..\..\..\lib\mgl\header\mgl_imageselector.h \
	..\..\..\application\application.h \

..\..\..\application\mgl\app_menu2.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\lib\mgl\header\mgl.h \
	..\..\..\lib\mgl\header\mgl_ip.h \
	..\..\..\lib\mgl\header\mgl_animator.h \
	..\..\..\lib\mgl\header\mgl_itemmanager.h \
	..\..\..\lib\mgl\header\mgl_keyframe.h \
	..\..\..\lib\mgl\header\mgl_imageselector.h \
	..\..\..\lib\mgl\header\mgl_particlepile.h \
	..\..\..\application\application.h \

..\..\..\application\mgl\app_menu3.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\lib\mgl\header\mgl.h \
	..\..\..\lib\mgl\header\mgl_ip.h \
	..\..\..\lib\mgl\header\mgl_animator.h \
	..\..\..\lib\mgl\header\mgl_imageselector.h \
	..\..\..\application\application.h \

..\..\..\application\mgl\app_menu4.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\lib\mgl\header\mgl.h \
	..\..\..\lib\mgl\header\mgl_ip.h \
	..\..\..\lib\mgl\header\mgl_animator.h \
	..\..\..\lib\mgl\header\mgl_imageselector.h \
	..\..\..\application\application.h \

..\..\..\application\mgl\app_menu5.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\lib\mgl\header\mgl.h \
	..\..\..\lib\mgl\header\mgl_ip.h \
	..\..\..\lib\mgl\header\mgl_animator.h \
	..\..\..\lib\mgl\header\mgl_itemmanager.h \
	..\..\..\lib\mgl\header\mgl_keyframe.h \
	..\..\..\lib\mgl\header\mgl_imageselector.h \
	..\..\..\application\application.h \

..\..\..\application\mgl\app_mgl_idle.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\application\application.h \
	..\..\..\lib\mgl\header\mgl.h \

..\..\..\application\mgl\layer\mgl_il_rexy.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\lib\mgl\header\mgl.h \
	..\..\..\lib\mgl\header\mgl_il.h \

..\..\..\application\control_demo\source\sample_anibmp.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\application\control_demo\header\../header/sample_anibmp.h \
	..\..\..\control\header\rexy_ctl_anibmp.h \
	..\..\..\application\application.h \

..\..\..\application\control_demo\source\sample_control.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\application\control_demo\header\../header/sample_control.h \
	..\..\..\application\application.h \

..\..\..\application\control_demo\source\sample_edit.c: \
	..\..\..\core\header\rexy.h \
	..\..\..\control\header\rexy_ctl_core.h \
	..\..\..\control\header\rexy_ctl_edit.h \
	..\..\..\application\application.h \
	..\..\..\application\control_demo\header\../header/sample_edit.h \

..\..\..\application\control_demo\source\sample_list.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\application\control_demo\header\../header/sample_list.h \
	..\..\..\control\header\rexy_ctl_core.h \
	..\..\..\control\header\rexy_ctl_list.h \
	..\..\..\application\application.h \

..\..\..\application\control_demo\source\sample_popup.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\application\control_demo\header\../header/sample_popup.h \
	..\..\..\control\header\rexy_ctl_popup.h \
	..\..\..\application\application.h \

..\..\..\application\control_demo\source\sample_slidetext.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\application\control_demo\header\../header/sample_list.h \
	..\..\..\control\header\rexy_ctl_core.h \
	..\..\..\control\header\rexy_ctl_list.h \
	..\..\..\control\header\rexy_ctl_slidetext.h \
	..\..\..\application\application.h \

..\..\..\application\tetris\source\tetris.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\control\header\rexy_ctl_popup.h \
	..\..\..\lib\mgl\header\../../../application/tetris/header/tetris.h \
	..\..\..\application\application.h \

..\..\..\application\title\idle.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\control\header\rexy_ctl_core.h \
	..\..\..\control\header\rexy_ctl_list.h \
	..\..\..\application\application.h \

..\..\..\application\biorhythm\source\biorhythm.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\application\biorhythm\header\../header/biorhythm.h \
	..\..\..\control\header\rexy_ctl_popup.h \
	..\..\..\application\application.h \

..\..\..\application\window_demo\source\win_demo.c: \
	..\..\..\core\header\rexy.h \
	..\rexy_haldevice.h \
	..\..\..\application\window_demo\header\../header/win_demo.h \
	..\..\..\control\header\rexy_ctl_core.h \
	..\..\..\control\header\rexy_ctl_list.h \
	..\..\..\control\header\rexy_ctl_popup.h \
	..\..\..\application\application.h \

..\..\..\application\control_demo\header\sample_anibmp.h: \
	..\..\..\core\header\rexy.h \

..\..\..\application\control_demo\header\sample_control.h: \
	..\..\..\core\header\rexy.h \

..\..\..\application\control_demo\header\sample_edit.h: \
	..\..\..\core\header\rexy.h \

..\..\..\application\control_demo\header\sample_list.h: \
	..\..\..\core\header\rexy.h \

..\..\..\application\control_demo\header\sample_popup.h: \
	..\..\..\core\header\rexy.h \

..\..\..\application\control_demo\header\sample_slidetext.h: \
	..\..\..\core\header\rexy.h \

..\..\..\application\tetris\header\tetris.h: \
	..\..\..\core\header\rexy.h \

..\..\..\application\biorhythm\header\biorhythm.h: \
	..\..\..\core\header\rexy.h \

..\..\..\application\window_demo\header\win_demo.h: \
	..\..\..\core\header\rexy.h \

