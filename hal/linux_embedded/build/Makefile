OBJS=  	obj/startup.o \
	obj/rexy_clib.o \
	obj/rexy_halinterface.o \
	obj/app_bmpdata.o \
	obj/app_bmpimage.o \
	obj/app_fontdata.o \
	obj/fbScreen.o

SRCS= 	../device/startup.c \
	../device/rexy_clib.c \
	../device/rexy_halinterface.c \
	../../../resource/app_bmpdata.c \
	../../../resource/app_bmpimage.c \
	../../../resource/app_fontdata.c \
	../device/fbScreen.o

LIBS=	../lib/libapplication.a \
	../lib/libcontrol.a	\
	../lib/liblogic.a	\
	../lib/libcore.a

CFLAGS= $(DEBUG_OPTION) -I../../../core/header -I../../../logic/header -I.. -I../../../resource -I../../../project/header -I../../../application -I../../../resource
LDFLAGS= $(LDFLAGS) -lpthread

all: init rexye

init:
	mkdir -p ../lib
	mkdir -p obj
	make -f Makefile.core
	make -f Makefile.control
	make -f Makefile.logic
	make -f Makefile.application

rexye: $(LIBS) $(OBJS)
	$(CC) -o rexye $(LFLAGS)  $(OBJS) -lcore -llogic -lcontrol -lapplication -llogic -lcontrol -lcore \
	-lmgl_core -lmgl_ani -lmgl_ip -lmgl_core -lmgl_imageselector -lmgl_itemmanager -lmgl_keyframe -lmgl_particle -lmgl_particlepile -lmgl_core -lapplication -llogic -lm -lpthread

obj/%.o: ../device/%.c
	$(CC) -c $(CFLAGS) -o $@ $<	

obj/%.o: ../../../core/source/%.c
	$(CC) -c $(CFLAGS) -o $@ $<	

obj/%.o: ../../../resource/%.c
	$(CC) -c $(CFLAGS) -o $@ $<	

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
	make -f Makefile.core dep
	make -f Makefile.control dep 
	make -f Makefile.logic dep
	make -f Makefile.application dep

# DO NOT DELETE

../device/startup.o: /usr/include/linux/input.h /usr/include/sys/time.h
../device/startup.o: /usr/include/features.h /usr/include/sys/cdefs.h
../device/startup.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../device/startup.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
../device/startup.o: /usr/include/bits/typesizes.h /usr/include/time.h
../device/startup.o: /usr/include/bits/time.h /usr/include/sys/select.h
../device/startup.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
../device/startup.o: /usr/include/sys/ioctl.h /usr/include/bits/ioctls.h
../device/startup.o: /usr/include/asm/ioctls.h /usr/include/asm/ioctl.h
../device/startup.o: /usr/include/asm-generic/ioctl.h
../device/startup.o: /usr/include/bits/ioctl-types.h
../device/startup.o: /usr/include/sys/ttydefaults.h /usr/include/sys/types.h
../device/startup.o: /usr/include/endian.h /usr/include/bits/endian.h
../device/startup.o: /usr/include/sys/sysmacros.h
../device/startup.o: /usr/include/bits/pthreadtypes.h
../device/startup.o: /usr/include/asm/types.h /usr/include/fcntl.h
../device/startup.o: /usr/include/bits/fcntl.h /usr/include/pthread.h
../device/startup.o: /usr/include/sched.h /usr/include/bits/sched.h
../device/startup.o: /usr/include/signal.h /usr/include/bits/setjmp.h
../device/startup.o: /usr/include/sys/ipc.h /usr/include/bits/ipctypes.h
../device/startup.o: /usr/include/bits/ipc.h /usr/include/sys/msg.h
../device/startup.o: /usr/include/bits/msq.h ../../../core/header/rexy.h
../device/startup.o: /usr/include/stdlib.h /usr/include/alloca.h
../device/startup.o: ../../../core/header/rexy_error.h
../device/startup.o: ../../../core/header/rexy_mem.h
../device/startup.o: ../../../core/header/rexy_feature.h ../rexy_haltype.h
../device/startup.o: ../../../core/header/rexy_type.h
../device/startup.o: ../../../core/header/rexy_msg.h
../device/startup.o: ../../../core/header/rexy_lock.h
../device/startup.o: ../../../core/header/rexy_resource.h
../device/startup.o: ../../../core/header/rexy_font.h
../device/startup.o: ../../../core/header/rexy_win.h
../device/startup.o: ../../../core/header/rexy_user.h
../device/startup.o: ../../../logic/header/rexy_mainproc.h
../device/startup.o: ../../../logic/header/rexy_main.h
../device/startup.o: ../../../core/header/rexy_common.h
../device/startup.o: ../../../core/header/rexy_gdi.h
../device/startup.o: ../../../core/header/rexy_trace.h
../device/startup.o: ../../../core/header/rexy_device.h
../device/startup.o: ../../../core/header/rexy_const.h
../device/startup.o: ../../../core/header/rexy_winalloc.h
../device/startup.o: ../../../core/header/rexy_msgq.h
../device/startup.o: ../../../core/header/rexy_util.h
../device/startup.o: ../../../core/header/rexy_string.h
../device/startup.o: ../../../logic/header/rexy_sysmsg.h
../device/startup.o: ../../../logic/header/rexy_state.h
../device/startup.o: ../../../core/header/rexy_diag.h ../rexy_haldevice.h
../device/startup.o: ../../../core/header/rexy_halinterface.h
../device/startup.o: ../../../core/header/rexy_db.h
../device/rexy_clib.o: ../../../core/header/rexy.h /usr/include/stdlib.h
../device/rexy_clib.o: /usr/include/features.h /usr/include/sys/cdefs.h
../device/rexy_clib.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
../device/rexy_clib.o: /usr/include/gnu/stubs-32.h /usr/include/sys/types.h
../device/rexy_clib.o: /usr/include/bits/types.h
../device/rexy_clib.o: /usr/include/bits/typesizes.h /usr/include/time.h
../device/rexy_clib.o: /usr/include/endian.h /usr/include/bits/endian.h
../device/rexy_clib.o: /usr/include/sys/select.h /usr/include/bits/select.h
../device/rexy_clib.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
../device/rexy_clib.o: /usr/include/sys/sysmacros.h
../device/rexy_clib.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
../device/rexy_clib.o: ../../../core/header/rexy_error.h
../device/rexy_clib.o: ../../../core/header/rexy_mem.h
../device/rexy_clib.o: ../../../core/header/rexy_feature.h ../rexy_haltype.h
../device/rexy_clib.o: ../../../core/header/rexy_type.h
../device/rexy_clib.o: ../../../core/header/rexy_msg.h
../device/rexy_clib.o: ../../../core/header/rexy_lock.h
../device/rexy_clib.o: ../../../core/header/rexy_resource.h
../device/rexy_clib.o: ../../../core/header/rexy_font.h
../device/rexy_clib.o: ../../../core/header/rexy_win.h
../device/rexy_clib.o: ../../../core/header/rexy_user.h
../device/rexy_clib.o: ../../../logic/header/rexy_mainproc.h
../device/rexy_clib.o: ../../../logic/header/rexy_main.h
../device/rexy_clib.o: ../../../core/header/rexy_common.h
../device/rexy_clib.o: ../../../core/header/rexy_gdi.h
../device/rexy_clib.o: ../../../core/header/rexy_trace.h
../device/rexy_clib.o: ../../../core/header/rexy_device.h
../device/rexy_clib.o: ../../../core/header/rexy_const.h
../device/rexy_clib.o: ../../../core/header/rexy_winalloc.h
../device/rexy_clib.o: ../../../core/header/rexy_msgq.h
../device/rexy_clib.o: ../../../core/header/rexy_util.h
../device/rexy_clib.o: ../../../core/header/rexy_string.h
../device/rexy_clib.o: ../../../logic/header/rexy_sysmsg.h
../device/rexy_clib.o: ../../../logic/header/rexy_state.h
../device/rexy_clib.o: ../../../core/header/rexy_diag.h ../rexy_haldevice.h
../device/rexy_clib.o: ../../../core/header/rexy_halinterface.h
../device/rexy_clib.o: ../../../core/header/rexy_db.h /usr/include/stdio.h
../device/rexy_clib.o: /usr/include/libio.h /usr/include/_G_config.h
../device/rexy_clib.o: /usr/include/wchar.h /usr/include/bits/wchar.h
../device/rexy_clib.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
../device/rexy_clib.o: /usr/include/bits/sys_errlist.h /usr/include/string.h
../device/rexy_clib.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
../device/rexy_clib.o: /usr/include/bits/confname.h /usr/include/getopt.h
../device/rexy_clib.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h
../device/rexy_halinterface.o: ../../../core/header/rexy.h
../device/rexy_halinterface.o: /usr/include/stdlib.h /usr/include/features.h
../device/rexy_halinterface.o: /usr/include/sys/cdefs.h
../device/rexy_halinterface.o: /usr/include/bits/wordsize.h
../device/rexy_halinterface.o: /usr/include/gnu/stubs.h
../device/rexy_halinterface.o: /usr/include/gnu/stubs-32.h
../device/rexy_halinterface.o: /usr/include/sys/types.h
../device/rexy_halinterface.o: /usr/include/bits/types.h
../device/rexy_halinterface.o: /usr/include/bits/typesizes.h
../device/rexy_halinterface.o: /usr/include/time.h /usr/include/endian.h
../device/rexy_halinterface.o: /usr/include/bits/endian.h
../device/rexy_halinterface.o: /usr/include/sys/select.h
../device/rexy_halinterface.o: /usr/include/bits/select.h
../device/rexy_halinterface.o: /usr/include/bits/sigset.h
../device/rexy_halinterface.o: /usr/include/bits/time.h
../device/rexy_halinterface.o: /usr/include/sys/sysmacros.h
../device/rexy_halinterface.o: /usr/include/bits/pthreadtypes.h
../device/rexy_halinterface.o: /usr/include/alloca.h
../device/rexy_halinterface.o: ../../../core/header/rexy_error.h
../device/rexy_halinterface.o: ../../../core/header/rexy_mem.h
../device/rexy_halinterface.o: ../../../core/header/rexy_feature.h
../device/rexy_halinterface.o: ../rexy_haltype.h
../device/rexy_halinterface.o: ../../../core/header/rexy_type.h
../device/rexy_halinterface.o: ../../../core/header/rexy_msg.h
../device/rexy_halinterface.o: ../../../core/header/rexy_lock.h
../device/rexy_halinterface.o: ../../../core/header/rexy_resource.h
../device/rexy_halinterface.o: ../../../core/header/rexy_font.h
../device/rexy_halinterface.o: ../../../core/header/rexy_win.h
../device/rexy_halinterface.o: ../../../core/header/rexy_user.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_mainproc.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_main.h
../device/rexy_halinterface.o: ../../../core/header/rexy_common.h
../device/rexy_halinterface.o: ../../../core/header/rexy_gdi.h
../device/rexy_halinterface.o: ../../../core/header/rexy_trace.h
../device/rexy_halinterface.o: ../../../core/header/rexy_device.h
../device/rexy_halinterface.o: ../../../core/header/rexy_const.h
../device/rexy_halinterface.o: ../../../core/header/rexy_winalloc.h
../device/rexy_halinterface.o: ../../../core/header/rexy_msgq.h
../device/rexy_halinterface.o: ../../../core/header/rexy_util.h
../device/rexy_halinterface.o: ../../../core/header/rexy_string.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_sysmsg.h
../device/rexy_halinterface.o: ../../../logic/header/rexy_state.h
../device/rexy_halinterface.o: ../../../core/header/rexy_diag.h
../device/rexy_halinterface.o: ../rexy_haldevice.h
../device/rexy_halinterface.o: ../../../core/header/rexy_halinterface.h
../device/rexy_halinterface.o: ../../../core/header/rexy_db.h
../device/rexy_halinterface.o: /usr/include/stdio.h /usr/include/libio.h
../device/rexy_halinterface.o: /usr/include/_G_config.h /usr/include/wchar.h
../device/rexy_halinterface.o: /usr/include/bits/wchar.h /usr/include/gconv.h
../device/rexy_halinterface.o: /usr/include/bits/stdio_lim.h
../device/rexy_halinterface.o: /usr/include/bits/sys_errlist.h ../fbScreen.h
../device/rexy_halinterface.o: /usr/include/sys/time.h /usr/include/sys/ipc.h
../device/rexy_halinterface.o: /usr/include/bits/ipctypes.h
../device/rexy_halinterface.o: /usr/include/bits/ipc.h /usr/include/signal.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy.h
../../../resource/app_bmpdata.o: /usr/include/stdlib.h
../../../resource/app_bmpdata.o: /usr/include/features.h
../../../resource/app_bmpdata.o: /usr/include/sys/cdefs.h
../../../resource/app_bmpdata.o: /usr/include/bits/wordsize.h
../../../resource/app_bmpdata.o: /usr/include/gnu/stubs.h
../../../resource/app_bmpdata.o: /usr/include/gnu/stubs-32.h
../../../resource/app_bmpdata.o: /usr/include/sys/types.h
../../../resource/app_bmpdata.o: /usr/include/bits/types.h
../../../resource/app_bmpdata.o: /usr/include/bits/typesizes.h
../../../resource/app_bmpdata.o: /usr/include/time.h /usr/include/endian.h
../../../resource/app_bmpdata.o: /usr/include/bits/endian.h
../../../resource/app_bmpdata.o: /usr/include/sys/select.h
../../../resource/app_bmpdata.o: /usr/include/bits/select.h
../../../resource/app_bmpdata.o: /usr/include/bits/sigset.h
../../../resource/app_bmpdata.o: /usr/include/bits/time.h
../../../resource/app_bmpdata.o: /usr/include/sys/sysmacros.h
../../../resource/app_bmpdata.o: /usr/include/bits/pthreadtypes.h
../../../resource/app_bmpdata.o: /usr/include/alloca.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_error.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_mem.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_feature.h
../../../resource/app_bmpdata.o: ../rexy_haltype.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_type.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_msg.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_lock.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_resource.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_font.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_win.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_user.h
../../../resource/app_bmpdata.o: ../../../logic/header/rexy_mainproc.h
../../../resource/app_bmpdata.o: ../../../logic/header/rexy_main.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_common.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_gdi.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_trace.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_device.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_const.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_winalloc.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_msgq.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_util.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_string.h
../../../resource/app_bmpdata.o: ../../../logic/header/rexy_sysmsg.h
../../../resource/app_bmpdata.o: ../../../logic/header/rexy_state.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_diag.h
../../../resource/app_bmpdata.o: ../rexy_haldevice.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_halinterface.h
../../../resource/app_bmpdata.o: ../../../core/header/rexy_db.h
../../../resource/app_bmpdata.o: ../../../resource/app_bmpdata.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy.h
../../../resource/app_bmpimage.o: /usr/include/stdlib.h
../../../resource/app_bmpimage.o: /usr/include/features.h
../../../resource/app_bmpimage.o: /usr/include/sys/cdefs.h
../../../resource/app_bmpimage.o: /usr/include/bits/wordsize.h
../../../resource/app_bmpimage.o: /usr/include/gnu/stubs.h
../../../resource/app_bmpimage.o: /usr/include/gnu/stubs-32.h
../../../resource/app_bmpimage.o: /usr/include/sys/types.h
../../../resource/app_bmpimage.o: /usr/include/bits/types.h
../../../resource/app_bmpimage.o: /usr/include/bits/typesizes.h
../../../resource/app_bmpimage.o: /usr/include/time.h /usr/include/endian.h
../../../resource/app_bmpimage.o: /usr/include/bits/endian.h
../../../resource/app_bmpimage.o: /usr/include/sys/select.h
../../../resource/app_bmpimage.o: /usr/include/bits/select.h
../../../resource/app_bmpimage.o: /usr/include/bits/sigset.h
../../../resource/app_bmpimage.o: /usr/include/bits/time.h
../../../resource/app_bmpimage.o: /usr/include/sys/sysmacros.h
../../../resource/app_bmpimage.o: /usr/include/bits/pthreadtypes.h
../../../resource/app_bmpimage.o: /usr/include/alloca.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_error.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_mem.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_feature.h
../../../resource/app_bmpimage.o: ../rexy_haltype.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_type.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_msg.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_lock.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_resource.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_font.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_win.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_user.h
../../../resource/app_bmpimage.o: ../../../logic/header/rexy_mainproc.h
../../../resource/app_bmpimage.o: ../../../logic/header/rexy_main.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_common.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_gdi.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_trace.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_device.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_const.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_winalloc.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_msgq.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_util.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_string.h
../../../resource/app_bmpimage.o: ../../../logic/header/rexy_sysmsg.h
../../../resource/app_bmpimage.o: ../../../logic/header/rexy_state.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_diag.h
../../../resource/app_bmpimage.o: ../rexy_haldevice.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_halinterface.h
../../../resource/app_bmpimage.o: ../../../core/header/rexy_db.h
../../../resource/app_fontdata.o: ../../../core/header/rexy.h
../../../resource/app_fontdata.o: /usr/include/stdlib.h
../../../resource/app_fontdata.o: /usr/include/features.h
../../../resource/app_fontdata.o: /usr/include/sys/cdefs.h
../../../resource/app_fontdata.o: /usr/include/bits/wordsize.h
../../../resource/app_fontdata.o: /usr/include/gnu/stubs.h
../../../resource/app_fontdata.o: /usr/include/gnu/stubs-32.h
../../../resource/app_fontdata.o: /usr/include/sys/types.h
../../../resource/app_fontdata.o: /usr/include/bits/types.h
../../../resource/app_fontdata.o: /usr/include/bits/typesizes.h
../../../resource/app_fontdata.o: /usr/include/time.h /usr/include/endian.h
../../../resource/app_fontdata.o: /usr/include/bits/endian.h
../../../resource/app_fontdata.o: /usr/include/sys/select.h
../../../resource/app_fontdata.o: /usr/include/bits/select.h
../../../resource/app_fontdata.o: /usr/include/bits/sigset.h
../../../resource/app_fontdata.o: /usr/include/bits/time.h
../../../resource/app_fontdata.o: /usr/include/sys/sysmacros.h
../../../resource/app_fontdata.o: /usr/include/bits/pthreadtypes.h
../../../resource/app_fontdata.o: /usr/include/alloca.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_error.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_mem.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_feature.h
../../../resource/app_fontdata.o: ../rexy_haltype.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_type.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_msg.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_lock.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_resource.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_font.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_win.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_user.h
../../../resource/app_fontdata.o: ../../../logic/header/rexy_mainproc.h
../../../resource/app_fontdata.o: ../../../logic/header/rexy_main.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_common.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_gdi.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_trace.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_device.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_const.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_winalloc.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_msgq.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_util.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_string.h
../../../resource/app_fontdata.o: ../../../logic/header/rexy_sysmsg.h
../../../resource/app_fontdata.o: ../../../logic/header/rexy_state.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_diag.h
../../../resource/app_fontdata.o: ../rexy_haldevice.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_halinterface.h
../../../resource/app_fontdata.o: ../../../core/header/rexy_db.h
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_10x10_Han_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_10x10_Bl_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_10x10_Sl_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_12x12_Han_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_12x12_Bl_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_12x12_Sl_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_14x14_Han_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_14x14_Bl_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_14x14_Sl_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_16x16_Han_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_16x16_BL_Img.txt
../../../resource/app_fontdata.o: ../../../resource/rexy_fontdata_Unicode_GulimChe_16x16_SL_Img.txt
