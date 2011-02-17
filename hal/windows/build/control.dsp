# Microsoft Developer Studio Project File - Name="control" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=control - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "control.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "control.mak" CFG="control - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "control - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "control - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "control - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
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
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "control - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "control_debug"
# PROP Intermediate_Dir "control_debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../core/header" /I "../../../logic/header" /I ".." /I "../../../resource" /I "../../../hal/windows" /I "../../../control/header" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "__SOFT_DEBUG__" /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "control - Win32 Release"
# Name "control - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\control\source\rexy_ctl_anibmp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\control\source\rexy_ctl_callback.c
# End Source File
# Begin Source File

SOURCE=..\..\..\control\source\rexy_ctl_core.c
# End Source File
# Begin Source File

SOURCE=..\..\..\control\source\rexy_ctl_edit.c
# End Source File
# Begin Source File

SOURCE=..\..\..\control\source\rexy_ctl_list.c
# End Source File
# Begin Source File

SOURCE=..\..\..\control\source\rexy_ctl_popup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\control\source\rexy_ctl_slidetext.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\control\header\rexy_ctl_anibmp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\control\header\rexy_ctl_core.h
# End Source File
# Begin Source File

SOURCE=..\..\..\control\header\rexy_ctl_edit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\control\header\rexy_ctl_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\control\header\rexy_ctl_popup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\control\header\rexy_ctl_slidetext.h
# End Source File
# End Group
# End Target
# End Project
