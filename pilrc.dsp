#
# @(#)pilrc.dsp
#
# Copyright 1997-1999, Wes Cherry   (mailto:wesc@technosis.com)
#                2000, Aaron Ardiri (mailto:aaron@ardiri.com)
# All rights reserved.
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation;  either version 2, or (at your option)
# any version.
#
# This program is distributed in the hope that it will be useful,  but
# WITHOUT  ANY  WARRANTY;   without  even   the  implied  warranty  of 
# MERCHANTABILITY  or FITNESS FOR A  PARTICULAR  PURPOSE.  See the GNU
# General Public License for more details.
#
# You  should have  received a  copy of the GNU General Public License
# along with this program;  if not,  please write to the Free Software 
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# Revisions:
# ==========
#
# pre 18-Jun-2000 <numerous developers>
#                 creation
#     18-Jun-2000 Aaron Ardiri
#                 GNU GPL documentation additions
#

# Microsoft Developer Studio Project File - Name="pilrc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=pilrc - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pilrc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pilrc.mak" CFG="pilrc - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pilrc - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "pilrc - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "pilrc - Win32 WinGUI" (based on "Win32 (x86) Console Application")
!MESSAGE "pilrc - Win32 RelUI" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pilrc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE F90 /I "Release/"
# ADD F90 /I "Release/"
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /Zp2 /W3 /GX /O2 /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "pilrc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE F90 /I "Debug/"
# ADD F90 /I "Debug/"
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /Zp2 /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:I386

!ELSEIF  "$(CFG)" == "pilrc - Win32 WinGUI"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\debugui"
# PROP BASE Intermediate_Dir ".\debugui"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\debugui"
# PROP Intermediate_Dir ".\debugui"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE F90 /I "debugui/"
# ADD F90 /I "debugui/"
# ADD BASE CPP /nologo /Zp2 /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /YX /c
# ADD CPP /nologo /Zp2 /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WINGUI" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:console /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /debug /machine:I386 /out:".\debugui\Pilrcui.exe"
# SUBTRACT LINK32 /pdb:none /map

!ELSEIF  "$(CFG)" == "pilrc - Win32 RelUI"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\pilrc___"
# PROP BASE Intermediate_Dir ".\pilrc___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\relui"
# PROP Intermediate_Dir ".\relui"
# PROP Target_Dir ""
F90=fl32.exe
# ADD BASE F90 /I "pilrc___/"
# ADD F90 /I "relui/"
# ADD BASE CPP /nologo /Zp2 /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "WINGUI" /YX /c
# ADD CPP /nologo /Zp2 /W3 /GX /O2 /D "NDEBUG" /D "WINGUI" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:"debugui/Pilrcui.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /map /debug /machine:I386 /out:".\relui\Pilrcui.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "pilrc - Win32 Release"
# Name "pilrc - Win32 Debug"
# Name "pilrc - Win32 WinGUI"
# Name "pilrc - Win32 RelUI"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\BITMAP.C
# End Source File
# Begin Source File

SOURCE=.\font.c
# End Source File
# Begin Source File

SOURCE=.\lex.c
# End Source File
# Begin Source File

SOURCE=.\main.c

!IF  "$(CFG)" == "pilrc - Win32 Release"

!ELSEIF  "$(CFG)" == "pilrc - Win32 Debug"

!ELSEIF  "$(CFG)" == "pilrc - Win32 WinGUI"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pilrc - Win32 RelUI"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pilrc.c
# End Source File
# Begin Source File

SOURCE=.\plex.c
# End Source File
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\util.c
# End Source File
# Begin Source File

SOURCE=.\win.c

!IF  "$(CFG)" == "pilrc - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pilrc - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "pilrc - Win32 WinGUI"

!ELSEIF  "$(CFG)" == "pilrc - Win32 RelUI"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\bitmap.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\lex.h
# End Source File
# Begin Source File

SOURCE=.\pilrc.h
# End Source File
# Begin Source File

SOURCE=.\std.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
