# Microsoft Developer Studio Generated NMAKE File, Based on pilrc.dsp
!IF "$(CFG)" == ""
CFG=pilrc - Win32 Release
!MESSAGE No configuration specified. Defaulting to pilrc - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "pilrc - Win32 Release" && "$(CFG)" != "pilrc - Win32 Debug" && "$(CFG)" != "pilrc - Win32 WinGUI" && "$(CFG)" != "pilrc - Win32 RelUI"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pilrc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\pilrc.exe"


CLEAN :
	-@erase "$(INTDIR)\BITMAP.OBJ"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\lex.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\pilrc.obj"
	-@erase "$(INTDIR)\plex.obj"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\pilrc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
CPP_PROJ=/nologo /Zp2 /ML /W3 /GX /O2 /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /Fp"$(INTDIR)\pilrc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\resource.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pilrc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\pilrc.pdb" /machine:I386 /out:"$(OUTDIR)\pilrc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BITMAP.OBJ" \
	"$(INTDIR)\lex.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\pilrc.obj" \
	"$(INTDIR)\plex.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\resource.res" \
	"$(INTDIR)\font.obj"

"$(OUTDIR)\pilrc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pilrc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\pilrc.exe"


CLEAN :
	-@erase "$(INTDIR)\BITMAP.OBJ"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\lex.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\pilrc.obj"
	-@erase "$(INTDIR)\plex.obj"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\pilrc.exe"
	-@erase "$(OUTDIR)\pilrc.ilk"
	-@erase "$(OUTDIR)\pilrc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
CPP_PROJ=/nologo /Zp2 /MLd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /Fp"$(INTDIR)\pilrc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\resource.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pilrc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\pilrc.pdb" /debug /machine:I386 /out:"$(OUTDIR)\pilrc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BITMAP.OBJ" \
	"$(INTDIR)\lex.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\pilrc.obj" \
	"$(INTDIR)\plex.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\resource.res" \
	"$(INTDIR)\font.obj"

"$(OUTDIR)\pilrc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pilrc - Win32 WinGUI"

OUTDIR=.\debugui
INTDIR=.\debugui
# Begin Custom Macros
OutDir=.\debugui
# End Custom Macros

ALL : "$(OUTDIR)\Pilrcui.exe"


CLEAN :
	-@erase "$(INTDIR)\BITMAP.OBJ"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\lex.obj"
	-@erase "$(INTDIR)\pilrc.obj"
	-@erase "$(INTDIR)\plex.obj"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\win.obj"
	-@erase "$(OUTDIR)\Pilrcui.exe"
	-@erase "$(OUTDIR)\Pilrcui.ilk"
	-@erase "$(OUTDIR)\Pilrcui.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
CPP_PROJ=/nologo /Zp2 /MLd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WINGUI" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /Fp"$(INTDIR)\pilrc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\resource.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pilrc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Pilrcui.pdb" /debug /machine:I386 /out:"$(OUTDIR)\Pilrcui.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BITMAP.OBJ" \
	"$(INTDIR)\lex.obj" \
	"$(INTDIR)\pilrc.obj" \
	"$(INTDIR)\plex.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\win.obj" \
	"$(INTDIR)\resource.res" \
	"$(INTDIR)\font.obj"

"$(OUTDIR)\Pilrcui.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pilrc - Win32 RelUI"

OUTDIR=.\relui
INTDIR=.\relui
# Begin Custom Macros
OutDir=.\relui
# End Custom Macros

ALL : "$(OUTDIR)\Pilrcui.exe"


CLEAN :
	-@erase "$(INTDIR)\BITMAP.OBJ"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\lex.obj"
	-@erase "$(INTDIR)\pilrc.obj"
	-@erase "$(INTDIR)\plex.obj"
	-@erase "$(INTDIR)\resource.res"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\win.obj"
	-@erase "$(OUTDIR)\Pilrcui.exe"
	-@erase "$(OUTDIR)\Pilrcui.ilk"
	-@erase "$(OUTDIR)\Pilrcui.map"
	-@erase "$(OUTDIR)\Pilrcui.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
CPP_PROJ=/nologo /Zp2 /MLd /W3 /GX /O2 /D "NDEBUG" /D "WINGUI" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /Fp"$(INTDIR)\pilrc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\resource.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pilrc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\Pilrcui.pdb" /map:"$(INTDIR)\Pilrcui.map" /debug /machine:I386 /out:"$(OUTDIR)\Pilrcui.exe" 
LINK32_OBJS= \
	"$(INTDIR)\BITMAP.OBJ" \
	"$(INTDIR)\lex.obj" \
	"$(INTDIR)\pilrc.obj" \
	"$(INTDIR)\plex.obj" \
	"$(INTDIR)\util.obj" \
	"$(INTDIR)\win.obj" \
	"$(INTDIR)\resource.res" \
	"$(INTDIR)\font.obj"

"$(OUTDIR)\Pilrcui.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("pilrc.dep")
!INCLUDE "pilrc.dep"
!ELSE 
!MESSAGE Warning: cannot find "pilrc.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "pilrc - Win32 Release" || "$(CFG)" == "pilrc - Win32 Debug" || "$(CFG)" == "pilrc - Win32 WinGUI" || "$(CFG)" == "pilrc - Win32 RelUI"
SOURCE=.\BITMAP.C

"$(INTDIR)\BITMAP.OBJ" : $(SOURCE) "$(INTDIR)"


SOURCE=.\font.c

"$(INTDIR)\font.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\lex.c

"$(INTDIR)\lex.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\main.c

!IF  "$(CFG)" == "pilrc - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "pilrc - Win32 Debug"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "pilrc - Win32 WinGUI"

!ELSEIF  "$(CFG)" == "pilrc - Win32 RelUI"

!ENDIF 

SOURCE=.\pilrc.c

"$(INTDIR)\pilrc.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\plex.c

"$(INTDIR)\plex.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\resource.rc

"$(INTDIR)\resource.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\util.c

"$(INTDIR)\util.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\win.c

!IF  "$(CFG)" == "pilrc - Win32 Release"

!ELSEIF  "$(CFG)" == "pilrc - Win32 Debug"

!ELSEIF  "$(CFG)" == "pilrc - Win32 WinGUI"


"$(INTDIR)\win.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "pilrc - Win32 RelUI"


"$(INTDIR)\win.obj" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

