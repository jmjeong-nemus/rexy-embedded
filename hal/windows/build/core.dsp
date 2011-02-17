# Microsoft Developer Studio Project File - Name="core" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=core - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "core.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "core.mak" CFG="core - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "core - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "core - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "core - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "core_release"
# PROP Intermediate_Dir "core_release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\core.lib"

!ELSEIF  "$(CFG)" == "core - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "core___Win32_Debug"
# PROP BASE Intermediate_Dir "core___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "core_debug"
# PROP Intermediate_Dir "core_debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../core/header" /I "../../../logic/header" /I ".." /I "../../../resource" /I "../../../hal/windows" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "__SOFT_DEBUG__" /YX /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\lib\core.lib"

!ENDIF 

# Begin Target

# Name "core - Win32 Release"
# Name "core - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\core\source\rexy_db.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_diag.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_feature.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_font.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_gdi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_io.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_lock.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_msg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_msgq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_resource.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_string.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_trace.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_user.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_util.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_win.c
# End Source File
# Begin Source File

SOURCE=..\..\..\core\source\rexy_winalloc.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\core\header\rexy.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_common.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_const.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_db.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_device.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_diag.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_error.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_feature.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_font.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_gdi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_halinterface.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_io.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_lock.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_mem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_msg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_msgq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_popup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_string.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_trace.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_type.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_user.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_win.h
# End Source File
# Begin Source File

SOURCE=..\..\..\core\header\rexy_winalloc.h
# End Source File
# End Group
# End Target
# End Project
