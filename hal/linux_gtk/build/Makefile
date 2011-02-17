OBJS=  	obj/startup.o \
	obj/rexy_clib.o \
	obj/rexy_halinterface.o \
	obj/app_bmpdata.o \
	obj/app_bmpimage.o \
	obj/app_fontdata.o

SRCS= 	../device/startup.c \
	../device/rexy_clib.c \
	../device/rexy_halinterface.c \
	../../../resource/app_bmpdata.c \
	../../../resource/app_bmpimage.c \
	../../../resource/app_fontdata.c

LIBS=	../lib/libapplication.a \
	../lib/libcontrol.a	\
	../lib/liblogic.a	\
	../lib/libcore.a

# if pkg-config could not find gtk+-2.0.pc, make sure libgtk+-2.0-devel package.1
CFLAGS= `pkg-config gtk+-2.0 --cflags` -g -I../../../core/header -I../../../logic/header -I.. -I../../../resource -I../../../project/header -I../../../application -I../../../resource
LFLAGS= `pkg-config gtk+-2.0 gthread-2.0 --libs` -g -L../lib -L../../../lib/mgl/linux

all: init rexye

init:
	mkdir -p ../lib
	mkdir -p obj
	make -f Makefile.core
	make -f Makefile.control
	make -f Makefile.logic
	make -f Makefile.application

rexye: $(LIBS) $(OBJS)
	gcc -o rexye $(LFLAGS)  $(OBJS) -lcore -llogic -lcontrol -lapplication -llogic -lcontrol -lcore \
	-lmgl_core -lmgl_ani -lmgl_ip -lmgl_core -lmgl_imageselector -lmgl_itemmanager -lmgl_keyframe -lmgl_particle -lmgl_particlepile -lmgl_core -lapplication

obj/%.o: ../device/%.c
	gcc -c $(CFLAGS) -o $@ $<	

obj/%.o: ../../../core/source/%.c
	gcc -c $(CFLAGS) -o $@ $<	

obj/%.o: ../../../resource/%.c
	gcc -c $(CFLAGS) -o $@ $<	

clean:
	rm -rf obj
	rm -rf ../lib

cscope:
	@cd ../../..;cscope-indexer -r;etags -L cscope.files

update:
	@cd ../../..;svn update

status:
	@cd ../../..;svn status

dep:
	makedepend $(CFLAGS) $(SRCS)
# DO NOT DELETE

../device/startup.o: ../../../core/header/rexy_error.h
../device/startup.o: ../../../core/header/rexy_mem.h
../device/startup.o: ../../../logic/header/rexy_feature.h ../rexy_haltype.h
../device/startup.o: ../../../core/header/rexy_type.h ../rexy_halinterface.h
../device/startup.o: ../../../core/header/rexy_msg.h
../device/startup.o: ../../../core/header/rexy_lock.h
../device/startup.o: ../../../core/header/rexy_aniobj.h
../device/startup.o: ../../../core/header/rexy_win.h
../device/startup.o: ../../../core/header/rexy_user.h
../device/startup.o: ../../../logic/header/rexy_pict.h
../device/startup.o: ../../../logic/header/rexy_mainproc.h
../device/startup.o: ../../../logic/header/rexy_main.h
../device/startup.o: ../../../core/header/rexy_common.h
../device/startup.o: ../../../core/header/rexy_gdi.h
../device/startup.o: ../../../core/header/rexy_trace.h
../device/startup.o: ../../../core/header/rexy_resource.h
../device/startup.o: ../../../resource/rexy_fontdata.h
../device/startup.o: ../../../core/header/rexy_device.h
../device/startup.o: ../../../core/header/rexy_const.h ../rexy_clib.h
../device/startup.o: ../../../core/header/rexy_winalloc.h
../device/startup.o: ../../../core/header/rexy_msgq.h
../device/startup.o: ../../../logic/header/rexy_mbcs.h
../device/startup.o: ../../../logic/header/application.h
../device/startup.o: ../../../logic/header/phone_sys_app.h
../device/startup.o: ../../../core/header/rexy_font.h
../device/startup.o: ../../../core/header/rexy_util.h
../device/startup.o: ../../../core/header/rexy_string.h
../device/startup.o: ../../../logic/header/rexy_idletimer.h
../device/startup.o: ../../../logic/header/rexy_sysmsg.h
../device/startup.o: ../../../logic/header/rexy_state.h
../device/startup.o: ../../../logic/header/rexy_ctl_core.h
../device/rexy_clib.o: ../../../core/header/rexy_error.h
../device/rexy_clib.o: ../../../core/header/rexy_mem.h
../device/rexy_clib.o: ../../../logic/header/rexy_feature.h ../rexy_haltype.h
../device/rexy_clib.o: ../../../core/header/rexy_type.h
../device/rexy_clib.o: ../rexy_halinterface.h ../../../core/header/rexy_msg.h
../device/rexy_clib.o: ../../../core/header/rexy_lock.h
../device/rexy_clib.o: ../../../core/header/rexy_aniobj.h
../device/rexy_clib.o: ../../../core/header/rexy_win.h
../device/rexy_clib.o: ../../../core/header/rexy_user.h
../device/rexy_clib.o: ../../../logic/header/rexy_pict.h
../device/rexy_clib.o: ../../../logic/header/rexy_mainproc.h
../device/rexy_clib.o: ../../../logic/header/rexy_main.h
../device/rexy_clib.o: ../../../core/header/rexy_common.h
../device/rexy_clib.o: ../../../core/header/rexy_gdi.h
../device/rexy_clib.o: ../../../core/header/rexy_trace.h
../device/rexy_clib.o: ../../../core/header/rexy_resource.h
../device/rexy_clib.o: ../../../resource/rexy_fontdata.h
../device/rexy_clib.o: ../../../core/header/rexy_device.h
../device/rexy_clib.o: ../../../core/header/rexy_const.h ../rexy_clib.h
../device/rexy_clib.o: ../../../core/header/rexy_winalloc.h
../device/rexy_clib.o: ../../../core/header/rexy_msgq.h
../device/rexy_clib.o: ../../../logic/header/rexy_mbcs.h
../device/rexy_clib.o: ../../../logic/header/application.h
../device/rexy_clib.o: ../../../logic/header/phone_sys_app.h
../device/rexy_clib.o: ../../../core/header/rexy_font.h
../device/rexy_clib.o: ../../../core/header/rexy_util.h
../device/rexy_clib.o: ../../../core/header/rexy_string.h
../device/rexy_clib.o: ../../../logic/header/rexy_idletimer.h
../device/rexy_clib.o: ../../../logic/header/rexy_sysmsg.h
../device/rexy_clib.o: ../../../logic/header/rexy_state.h
../device/rexy_clib.o: ../../../logic/header/rexy_ctl_core.h
../device/rexy_halinterface.o: ../../../core/header/rexy.h
../device/rexy_halinterface.o: ../../../core/header/rexy_error.h
../device/rexy_halinterface.o: ../../../core/header/rexy_mem.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_feature.h
../device/rexy_halinterface.o: ../rexy_haltype.h
../device/rexy_halinterface.o: ../../../core/header/rexy_type.h
../device/rexy_halinterface.o: ../rexy_halinterface.h
../device/rexy_halinterface.o: ../../../core/header/rexy_msg.h
../device/rexy_halinterface.o: ../../../core/header/rexy_lock.h
../device/rexy_halinterface.o: ../../../core/header/rexy_aniobj.h
../device/rexy_halinterface.o: ../../../core/header/rexy_win.h
../device/rexy_halinterface.o: ../../../core/header/rexy_user.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_pict.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_mainproc.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_main.h
../device/rexy_halinterface.o: ../../../core/header/rexy_common.h
../device/rexy_halinterface.o: ../../../core/header/rexy_gdi.h
../device/rexy_halinterface.o: ../../../core/header/rexy_trace.h
../device/rexy_halinterface.o: ../../../core/header/rexy_resource.h
../device/rexy_halinterface.o: ../../../resource/rexy_fontdata.h
../device/rexy_halinterface.o: ../../../core/header/rexy_device.h
../device/rexy_halinterface.o: ../../../core/header/rexy_const.h
../device/rexy_halinterface.o: ../rexy_clib.h
../device/rexy_halinterface.o: ../../../core/header/rexy_winalloc.h
../device/rexy_halinterface.o: ../../../core/header/rexy_msgq.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_mbcs.h
../device/rexy_halinterface.o: ../../../logic/header/application.h
../device/rexy_halinterface.o: ../../../logic/header/phone_sys_app.h
../device/rexy_halinterface.o: ../../../core/header/rexy_font.h
../device/rexy_halinterface.o: ../../../core/header/rexy_util.h
../device/rexy_halinterface.o: ../../../core/header/rexy_string.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_idletimer.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_sysmsg.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_state.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_ctl_core.h
