# Microsoft Developer Studio Project File - Name="emulator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=emulator - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "emulator.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "emulator.mak" CFG="emulator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "emulator - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "emulator - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "emulator - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "emulator_release"
# PROP Intermediate_Dir "emulator_release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "emulator - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "emulator___Win32_Debug"
# PROP BASE Intermediate_Dir "emulator___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "emulator_debug"
# PROP Intermediate_Dir "emulator_debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../core/header" /I "../../../logic/header" /I ".." /I "../../../resource" /I "../../../test/header" /D "_WINDOWS" /D "__SOFT_DEBUG__" /D "WIN32" /D "_DEBUG" /D "_MBCS" /Fr /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mgl_ani.lib mgl_core.lib mgl_imageselector.lib mgl_ip.lib mgl_itemmanager.lib mgl_keyframe.lib mgl_particle.lib mgl_particlepile.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrtd.lib" /pdbtype:sept /libpath:"../../../Lib/MGL/windows"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "emulator - Win32 Release"
# Name "emulator - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\device\rexy_debug_win.c
# End Source File
# Begin Source File

SOURCE=..\device\rexy_halinterface.c
# End Source File
# Begin Source File

SOURCE=..\device\rexy_integrated.c
# End Source File
# Begin Source File

SOURCE=..\device\rexy_test_win.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\device\resource.h
# End Source File
# Begin Source File

SOURCE=..\rexy_clib.h
# End Source File
# Begin Source File

SOURCE=..\device\rexy_debug_win.h
# End Source File
# Begin Source File

SOURCE=..\rexy_haldevice.h
# End Source File
# Begin Source File

SOURCE=..\rexy_haltype.h
# End Source File
# Begin Source File

SOURCE=..\device\rexy_integrated.h
# End Source File
# Begin Source File

SOURCE=..\device\rexy_test_win.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\device\rexy_integrated.ico
# End Source File
# Begin Source File

SOURCE=..\device\rexy_integrated.rc
# End Source File
# Begin Source File

SOURCE=..\device\small.ico
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\lib\zlib\projects\visualc6\Debug\zlib1.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\lpng\projects\visualc6\Win32_LIB_ASM_Debug\libpngd.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libjpeg\project\vc60\Debug\libjpeg.lib
# End Source File
# Begin Source File

SOURCE=..\..\..\lib\libungif\project\vc60\Release\libungif.lib
# End Source File
# End Target
# End Project
