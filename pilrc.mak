# Microsoft Developer Studio Generated NMAKE File, Based on pilrc.dsp
!IF "$(CFG)" == ""
CFG=pilrc - Win32 Debug
!MESSAGE No configuration specified. Defaulting to pilrc - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "pilrc - Win32 int Release" && "$(CFG)" != "pilrc - Win32 int Debug" && "$(CFG)" != "pilrc - Win32 Debug" && "$(CFG)" != "pilrc - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pilrc.mak" CFG="pilrc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pilrc - Win32 int Release" (based on "Win32 (x86) Console Application")
!MESSAGE "pilrc - Win32 int Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "pilrc - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "pilrc - Win32 Release" (based on "Win32 (x86) Console Application")
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

!IF  "$(CFG)" == "pilrc - Win32 int Release"

OUTDIR=.\iRelease
INTDIR=.\iRelease
# Begin Custom Macros
OutDir=.\iRelease
# End Custom Macros

ALL : ".\PilRc_i.exe" "$(OUTDIR)\pilrc.bsc"


CLEAN :
	-@erase "$(INTDIR)\bitmap.obj"
	-@erase "$(INTDIR)\bitmap.sbr"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\font.sbr"
	-@erase "$(INTDIR)\lex.obj"
	-@erase "$(INTDIR)\lex.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\makeKbd.obj"
	-@erase "$(INTDIR)\makeKbd.sbr"
	-@erase "$(INTDIR)\PalmRC.obj"
	-@erase "$(INTDIR)\PalmRC.sbr"
	-@erase "$(INTDIR)\pilrc.obj"
	-@erase "$(INTDIR)\pilrc.sbr"
	-@erase "$(INTDIR)\plex.obj"
	-@erase "$(INTDIR)\plex.sbr"
	-@erase "$(INTDIR)\restype.obj"
	-@erase "$(INTDIR)\restype.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\pilrc.bsc"
	-@erase ".\PilRc_i.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
CPP_PROJ=/nologo /Zp2 /ML /W3 /GX /O2 /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /D "PALM_INTERNAL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\pilrc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pilrc.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bitmap.sbr" \
	"$(INTDIR)\font.sbr" \
	"$(INTDIR)\lex.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\makeKbd.sbr" \
	"$(INTDIR)\PalmRC.sbr" \
	"$(INTDIR)\pilrc.sbr" \
	"$(INTDIR)\plex.sbr" \
	"$(INTDIR)\restype.sbr" \
	"$(INTDIR)\util.sbr"

"$(OUTDIR)\pilrc.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\PilRc_i.pdb" /machine:I386 /out:".\PilRc_i.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bitmap.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\lex.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\makeKbd.obj" \
	"$(INTDIR)\PalmRC.obj" \
	"$(INTDIR)\pilrc.obj" \
	"$(INTDIR)\plex.obj" \
	"$(INTDIR)\restype.obj" \
	"$(INTDIR)\util.obj"

".\PilRc_i.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pilrc - Win32 int Debug"

OUTDIR=.\iDebug
INTDIR=.\iDebug
# Begin Custom Macros
OutDir=.\iDebug
# End Custom Macros

ALL : ".\PilRc_idbg.exe" "$(OUTDIR)\pilrc.bsc"


CLEAN :
	-@erase "$(INTDIR)\bitmap.obj"
	-@erase "$(INTDIR)\bitmap.sbr"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\font.sbr"
	-@erase "$(INTDIR)\lex.obj"
	-@erase "$(INTDIR)\lex.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\makeKbd.obj"
	-@erase "$(INTDIR)\makeKbd.sbr"
	-@erase "$(INTDIR)\PalmRC.obj"
	-@erase "$(INTDIR)\PalmRC.sbr"
	-@erase "$(INTDIR)\pilrc.obj"
	-@erase "$(INTDIR)\pilrc.sbr"
	-@erase "$(INTDIR)\plex.obj"
	-@erase "$(INTDIR)\plex.sbr"
	-@erase "$(INTDIR)\restype.obj"
	-@erase "$(INTDIR)\restype.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\pilrc.bsc"
	-@erase "$(OUTDIR)\PilRc_idbg.pdb"
	-@erase ".\PilRc_idbg.exe"
	-@erase ".\PilRc_idbg.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
CPP_PROJ=/nologo /Zp2 /MLd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /D "PALM_INTERNAL" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\pilrc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pilrc.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bitmap.sbr" \
	"$(INTDIR)\font.sbr" \
	"$(INTDIR)\lex.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\makeKbd.sbr" \
	"$(INTDIR)\PalmRC.sbr" \
	"$(INTDIR)\pilrc.sbr" \
	"$(INTDIR)\plex.sbr" \
	"$(INTDIR)\restype.sbr" \
	"$(INTDIR)\util.sbr"

"$(OUTDIR)\pilrc.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\PilRc_idbg.pdb" /debug /machine:I386 /out:".\PilRc_idbg.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bitmap.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\lex.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\makeKbd.obj" \
	"$(INTDIR)\PalmRC.obj" \
	"$(INTDIR)\pilrc.obj" \
	"$(INTDIR)\plex.obj" \
	"$(INTDIR)\restype.obj" \
	"$(INTDIR)\util.obj"

".\PilRc_idbg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pilrc - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : ".\PilRc_dbg.exe" "$(OUTDIR)\pilrc.bsc"


CLEAN :
	-@erase "$(INTDIR)\bitmap.obj"
	-@erase "$(INTDIR)\bitmap.sbr"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\font.sbr"
	-@erase "$(INTDIR)\lex.obj"
	-@erase "$(INTDIR)\lex.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\pilrc.obj"
	-@erase "$(INTDIR)\pilrc.sbr"
	-@erase "$(INTDIR)\plex.obj"
	-@erase "$(INTDIR)\plex.sbr"
	-@erase "$(INTDIR)\restype.obj"
	-@erase "$(INTDIR)\restype.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\pilrc.bsc"
	-@erase "$(OUTDIR)\PilRc_dbg.pdb"
	-@erase ".\PilRc_dbg.exe"
	-@erase ".\PilRc_dbg.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
CPP_PROJ=/nologo /Zp2 /MLd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "_CONSOLE" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\pilrc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pilrc.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bitmap.sbr" \
	"$(INTDIR)\font.sbr" \
	"$(INTDIR)\lex.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\pilrc.sbr" \
	"$(INTDIR)\plex.sbr" \
	"$(INTDIR)\restype.sbr" \
	"$(INTDIR)\util.sbr"

"$(OUTDIR)\pilrc.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\PilRc_dbg.pdb" /debug /machine:I386 /out:".\PilRc_dbg.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bitmap.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\lex.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\pilrc.obj" \
	"$(INTDIR)\plex.obj" \
	"$(INTDIR)\restype.obj" \
	"$(INTDIR)\util.obj"

".\PilRc_dbg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "pilrc - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : ".\PilRc.exe" "$(OUTDIR)\pilrc.bsc"


CLEAN :
	-@erase "$(INTDIR)\bitmap.obj"
	-@erase "$(INTDIR)\bitmap.sbr"
	-@erase "$(INTDIR)\font.obj"
	-@erase "$(INTDIR)\font.sbr"
	-@erase "$(INTDIR)\lex.obj"
	-@erase "$(INTDIR)\lex.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\pilrc.obj"
	-@erase "$(INTDIR)\pilrc.sbr"
	-@erase "$(INTDIR)\plex.obj"
	-@erase "$(INTDIR)\plex.sbr"
	-@erase "$(INTDIR)\restype.obj"
	-@erase "$(INTDIR)\restype.sbr"
	-@erase "$(INTDIR)\util.obj"
	-@erase "$(INTDIR)\util.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\pilrc.bsc"
	-@erase ".\PilRc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

F90=fl32.exe
CPP_PROJ=/nologo /Zp2 /ML /W3 /GX /O2 /D "NDEBUG" /D "_CONSOLE" /D "WIN32" /D "BINOUT" /D "SWAPBYTES" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\pilrc.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\pilrc.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bitmap.sbr" \
	"$(INTDIR)\font.sbr" \
	"$(INTDIR)\lex.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\pilrc.sbr" \
	"$(INTDIR)\plex.sbr" \
	"$(INTDIR)\restype.sbr" \
	"$(INTDIR)\util.sbr"

"$(OUTDIR)\pilrc.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\PilRc.pdb" /machine:I386 /out:".\PilRc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bitmap.obj" \
	"$(INTDIR)\font.obj" \
	"$(INTDIR)\lex.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\pilrc.obj" \
	"$(INTDIR)\plex.obj" \
	"$(INTDIR)\restype.obj" \
	"$(INTDIR)\util.obj"

".\PilRc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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


!IF "$(CFG)" == "pilrc - Win32 int Release" || "$(CFG)" == "pilrc - Win32 int Debug" || "$(CFG)" == "pilrc - Win32 Debug" || "$(CFG)" == "pilrc - Win32 Release"
SOURCE=.\bitmap.c

"$(INTDIR)\bitmap.obj"	"$(INTDIR)\bitmap.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\font.c

"$(INTDIR)\font.obj"	"$(INTDIR)\font.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\lex.c

"$(INTDIR)\lex.obj"	"$(INTDIR)\lex.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\main.c

"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\makeKbd.c

!IF  "$(CFG)" == "pilrc - Win32 int Release"


"$(INTDIR)\makeKbd.obj"	"$(INTDIR)\makeKbd.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "pilrc - Win32 int Debug"


"$(INTDIR)\makeKbd.obj"	"$(INTDIR)\makeKbd.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "pilrc - Win32 Debug"

!ELSEIF  "$(CFG)" == "pilrc - Win32 Release"

!ENDIF 

SOURCE=.\PalmRC.c

!IF  "$(CFG)" == "pilrc - Win32 int Release"


"$(INTDIR)\PalmRC.obj"	"$(INTDIR)\PalmRC.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "pilrc - Win32 int Debug"


"$(INTDIR)\PalmRC.obj"	"$(INTDIR)\PalmRC.sbr" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "pilrc - Win32 Debug"

!ELSEIF  "$(CFG)" == "pilrc - Win32 Release"

!ENDIF 

SOURCE=.\pilrc.c

"$(INTDIR)\pilrc.obj"	"$(INTDIR)\pilrc.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\plex.c

"$(INTDIR)\plex.obj"	"$(INTDIR)\plex.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\restype.c

"$(INTDIR)\restype.obj"	"$(INTDIR)\restype.sbr" : $(SOURCE) "$(INTDIR)"


SOURCE=.\util.c

"$(INTDIR)\util.obj"	"$(INTDIR)\util.sbr" : $(SOURCE) "$(INTDIR)"



!ENDIF 

