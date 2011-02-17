.SILENT:

UCOS_BASE=..\device\uCOS
UCOS_ARM_BASE=$(UCOS_BASE)\arm_ucos
FRMWRK_BASE=$(UCOS_ARM_BASE)\s3c2410a

TARGET=rexye.bin
AFLAGS=-via conf-asm.txt -I$(FRMWRK_BASE)\inc
LFLAGS=-via conf-linker.txt

LIBS= \
	obj\core.lib \
	obj\device.lib \
	obj\driver.lib \
	obj\frmwrk.lib \
	obj\logic.lib \
	obj\ucos.lib \
	obj\application.lib \
	obj\resource.lib \
	obj\sample.lib \
	obj\control.lib \
	..\..\..\lib\mgl\s3c2410\mgl_core.a \
	..\..\..\lib\mgl\s3c2410\mgl_imageselector.a \
	..\..\..\lib\mgl\s3c2410\mgl_ip.a \
	..\..\..\lib\mgl\s3c2410\mgl_itemmanager.a \
	..\..\..\lib\mgl\s3c2410\mgl_keyframe.a \
	..\..\..\lib\mgl\s3c2410\mgl_particle.a \
	..\..\..\lib\mgl\s3c2410\mgl_particlepile.a \
	..\..\..\lib\mgl\s3c2410\mgl_ani.a

all: init $(TARGET)

init:
	echo setting environment variables.
	set ARMHOME=C:\Program Files\ARM\ADSv1_2
	set ARMCONF=%ARMHOME%\BIN
	set ARMDLL=%ARMHOME%\BIN
	set ARMINC=%ARMHOME%\INCLUDE
	set ARMLIB=%ARMHOME%\LIB
	set PATH=%PATH%;%ARMDLL%;
	nmake /fcore.mak /nologo
	nmake /fdevice.mak /nologo
	nmake /fdriver.mak /nologo
	nmake /ffrmwrk.mak /nologo
	nmake /flogic.mak /nologo
	nmake /fucos.mak /nologo
	nmake /fapplication.mak /nologo
	nmake /fresource.mak /nologo
	nmake /fsample.mak /nologo
	nmake /fcontrol.mak /nologo

$(TARGET): s3c2410.axf
	echo fromelf $(TARGET)
	fromelf -bin -output $(TARGET) s3c2410.axf

s3c2410.axf: obj\init.obj $(LIBS)
	echo link s3c2410.axf
	armlink $(LFLAGS) -o s3c2410.axf $(LIBS) obj\init.obj

obj\init.obj:$(FRMWRK_BASE)\src\init.s
	echo assemble init.s
	armasm $(AFLAGS) -o obj\init.obj $(FRMWRK_BASE)\src\init.s

clean:
	nmake /fcore.mak /nologo clean
	nmake /fdevice.mak /nologo clean
	nmake /fdriver.mak /nologo clean
	nmake /ffrmwrk.mak /nologo clean
	nmake /flogic.mak /nologo clean
	nmake /fucos.mak /nologo clean
	nmake /fapplication.mak /nologo clean
	nmake /fresource.mak /nologo clean
	nmake /fsample.mak /nologo clean
	nmake /fcontrol.mak /nologo clean
	del obj\init.obj
	del s3c2410.axf
	del $(TARGET)


$(FRMWRK_BASE)\src\init.s:\
	$(FRMWRK_BASE)\inc\s3c2410x.a \
	$(FRMWRK_BASE)\inc\memcfg.a
