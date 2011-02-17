# Microsoft Developer Studio Project File - Name="application" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=application - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "application.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "application.mak" CFG="application - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "application - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "application - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "application - Win32 Release"

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

!ELSEIF  "$(CFG)" == "application - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "application_debug"
# PROP Intermediate_Dir "application_debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../control/header" /I "../../../core/header" /I "../../../addin/rexyDB" /I "../../../logic/header" /I "../../../resource" /I "../../../hal/windows" /I "../../../Lib/MGL/Header" /I "../../../application" /D "_LIB" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "__SOFT_DEBUG__" /YX /FD /GZ /c
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

# Name "application - Win32 Release"
# Name "application - Win32 Debug"
# Begin Group "Control Demo"

# PROP Default_Filter ""
# Begin Group "header"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\control_demo\header\sample_anibmp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\control_demo\header\sample_control.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\control_demo\header\sample_edit.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\control_demo\header\sample_list.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\control_demo\header\sample_popup.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\control_demo\header\sample_slidetext.h
# End Source File
# End Group
# Begin Group "source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\control_demo\source\sample_anibmp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\control_demo\source\sample_control.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\control_demo\source\sample_edit.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\control_demo\source\sample_list.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\control_demo\source\sample_popup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\control_demo\source\sample_slidetext.c
# End Source File
# End Group
# End Group
# Begin Group "MGL Demo"

# PROP Default_Filter ""
# Begin Group "Layer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\mgl\layer\mgl_il_rexy.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\application\mgl\app_menu1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\mgl\app_menu2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\mgl\app_menu3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\mgl\app_menu4.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\mgl\app_menu5.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\mgl\app_mgl_idle.c
# End Source File
# End Group
# Begin Group "Title"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\title\idle.c
# End Source File
# End Group
# Begin Group "Biorhythm"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\biorhythm\source\biorhythm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\biorhythm\header\biorhythm.h
# End Source File
# End Group
# Begin Group "Browser"

# PROP Default_Filter ""
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\browser\source\base64.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\build.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\control.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\file.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\functions.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\gif.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\html.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\http.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\image.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\image_generic.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\image_helpers.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\jpeg.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\layout.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\magick.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\main.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\parser_helpers.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\pngs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\protocol_generic.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\retrieve.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\rexy_browser.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\settings.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\states.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\string.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\table.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\tags.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\text.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\threads.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_close.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_helpers.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_image.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_init.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_line.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_main.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_open.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_render.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_table.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_text.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\ui_title.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\source\util.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\browser\header\base64.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\file.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\functions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\gif.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\http.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\image.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\info.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\jpeg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\layout.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\magick.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\parse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\parser_helpers.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\paths.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\pngs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\protocol.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\retrieve.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\rexy_browser.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\settings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\states.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\streams.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\table.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\tags.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\threads.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\ui.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\version.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\browser\header\zen_ui.h
# End Source File
# End Group
# End Group
# Begin Group "Tetris"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\tetris\source\tetris.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\tetris\header\tetris.h
# End Source File
# End Group
# Begin Group "window_demo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\window_demo\source\win_demo.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\window_demo\header\win_demo.h
# End Source File
# End Group
# Begin Group "Image Viewer"

# PROP Default_Filter ""
# Begin Group "source No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\imgviewer\source\gifview.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\imgviewer\source\imgviewer.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\imgviewer\source\jpgview.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\imgviewer\source\pngview.c
# End Source File
# End Group
# Begin Group "header No. 1"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\imgviewer\header\gifview.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\imgviewer\header\imgdata.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\imgviewer\header\imgviewer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\imgviewer\header\jpgview.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\imgviewer\header\pngview.h
# End Source File
# End Group
# End Group
# Begin Group "DB Demo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\db_demo\source\example_db.c
# End Source File
# End Group
# Begin Group "MainMenu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\mainmenu\mainmenu.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\mainmenu\mainmenu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\application\mainmenu\mainmenu_anibmp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\mainmenu\mainmenu_anibmp.h
# End Source File
# End Group
# Begin Group "Address"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\application\Address\sample_address.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\Address\sample_address.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\application\application.c
# End Source File
# Begin Source File

SOURCE=..\..\..\application\application.h
# End Source File
# End Target
# End Project
