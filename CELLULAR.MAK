# Microsoft Developer Studio Generated NMAKE File, Based on CELLULAR.DSP
!IF "$(CFG)" == ""
CFG=CELLULAR - Win32 Debug
!MESSAGE No configuration specified. Defaulting to CELLULAR - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "CELLULAR - Win32 Release" && "$(CFG)" !=\
 "CELLULAR - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CELLULAR.MAK" CFG="CELLULAR - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CELLULAR - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CELLULAR - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\cell.exe"

!ELSE 

ALL : ".\cell.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Area.obj"
	-@erase "$(INTDIR)\CELLULAR.obj"
	-@erase "$(INTDIR)\CELLULAR.pch"
	-@erase "$(INTDIR)\CELLULAR.res"
	-@erase "$(INTDIR)\CELLULARDlg.obj"
	-@erase "$(INTDIR)\Comint.obj"
	-@erase "$(INTDIR)\Datalink.obj"
	-@erase "$(INTDIR)\Dmcache.obj"
	-@erase "$(INTDIR)\Filemgr.obj"
	-@erase "$(INTDIR)\Indexmgr.obj"
	-@erase "$(INTDIR)\Isammgr.obj"
	-@erase "$(INTDIR)\Isammsg.obj"
	-@erase "$(INTDIR)\Manufact.obj"
	-@erase "$(INTDIR)\Modem.obj"
	-@erase "$(INTDIR)\Nodemgr.obj"
	-@erase "$(INTDIR)\Phonelst.obj"
	-@erase "$(INTDIR)\Sdiskmgr.obj"
	-@erase "$(INTDIR)\Sidxmgr.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Transpor.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase ".\cell.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fp"$(INTDIR)\CELLULAR.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CELLULAR.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CELLULAR.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\cell.pdb" /machine:I386 /out:"cell.exe" 
LINK32_OBJS= \
	"$(INTDIR)\Area.obj" \
	"$(INTDIR)\CELLULAR.obj" \
	"$(INTDIR)\CELLULAR.res" \
	"$(INTDIR)\CELLULARDlg.obj" \
	"$(INTDIR)\Comint.obj" \
	"$(INTDIR)\Datalink.obj" \
	"$(INTDIR)\Dmcache.obj" \
	"$(INTDIR)\Filemgr.obj" \
	"$(INTDIR)\Indexmgr.obj" \
	"$(INTDIR)\Isammgr.obj" \
	"$(INTDIR)\Isammsg.obj" \
	"$(INTDIR)\Manufact.obj" \
	"$(INTDIR)\Modem.obj" \
	"$(INTDIR)\Nodemgr.obj" \
	"$(INTDIR)\Phonelst.obj" \
	"$(INTDIR)\Sdiskmgr.obj" \
	"$(INTDIR)\Sidxmgr.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Transpor.obj"

".\cell.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\CELLULAR.exe"

!ELSE 

ALL : "$(OUTDIR)\CELLULAR.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Area.obj"
	-@erase "$(INTDIR)\CELLULAR.obj"
	-@erase "$(INTDIR)\CELLULAR.pch"
	-@erase "$(INTDIR)\CELLULAR.res"
	-@erase "$(INTDIR)\CELLULARDlg.obj"
	-@erase "$(INTDIR)\Comint.obj"
	-@erase "$(INTDIR)\Datalink.obj"
	-@erase "$(INTDIR)\Dmcache.obj"
	-@erase "$(INTDIR)\Filemgr.obj"
	-@erase "$(INTDIR)\Indexmgr.obj"
	-@erase "$(INTDIR)\Isammgr.obj"
	-@erase "$(INTDIR)\Isammsg.obj"
	-@erase "$(INTDIR)\Manufact.obj"
	-@erase "$(INTDIR)\Modem.obj"
	-@erase "$(INTDIR)\Nodemgr.obj"
	-@erase "$(INTDIR)\Phonelst.obj"
	-@erase "$(INTDIR)\Sdiskmgr.obj"
	-@erase "$(INTDIR)\Sidxmgr.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\Transpor.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(OUTDIR)\CELLULAR.exe"
	-@erase "$(OUTDIR)\CELLULAR.ilk"
	-@erase "$(OUTDIR)\CELLULAR.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "_OS_NT_" /Fp"$(INTDIR)\CELLULAR.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\CELLULAR.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\CELLULAR.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)\CELLULAR.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)\CELLULAR.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Area.obj" \
	"$(INTDIR)\CELLULAR.obj" \
	"$(INTDIR)\CELLULAR.res" \
	"$(INTDIR)\CELLULARDlg.obj" \
	"$(INTDIR)\Comint.obj" \
	"$(INTDIR)\Datalink.obj" \
	"$(INTDIR)\Dmcache.obj" \
	"$(INTDIR)\Filemgr.obj" \
	"$(INTDIR)\Indexmgr.obj" \
	"$(INTDIR)\Isammgr.obj" \
	"$(INTDIR)\Isammsg.obj" \
	"$(INTDIR)\Manufact.obj" \
	"$(INTDIR)\Modem.obj" \
	"$(INTDIR)\Nodemgr.obj" \
	"$(INTDIR)\Phonelst.obj" \
	"$(INTDIR)\Sdiskmgr.obj" \
	"$(INTDIR)\Sidxmgr.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Transpor.obj"

"$(OUTDIR)\CELLULAR.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "CELLULAR - Win32 Release" || "$(CFG)" ==\
 "CELLULAR - Win32 Debug"
SOURCE=.\CELLULAR.cpp

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_CELLU=\
	".\CELLULAR.h"\
	".\CELLULARDlg.h"\
	

"$(INTDIR)\CELLULAR.obj" : $(SOURCE) $(DEP_CPP_CELLU) "$(INTDIR)"\
 "$(INTDIR)\CELLULAR.pch"


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_CELLU=\
	".\CELLULAR.h"\
	".\CELLULARDlg.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\CELLULAR.obj" : $(SOURCE) $(DEP_CPP_CELLU) "$(INTDIR)"\
 "$(INTDIR)\CELLULAR.pch"


!ENDIF 

SOURCE=.\CELLULAR.rc
DEP_RSC_CELLUL=\
	".\res\CELLULAR.ico"\
	".\res\CELLULAR.rc2"\
	

"$(INTDIR)\CELLULAR.res" : $(SOURCE) $(DEP_RSC_CELLUL) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\CELLULARDlg.cpp

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_CELLULA=\
	".\area.h"\
	".\CELLULAR.h"\
	".\CELLULARDlg.h"\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\isammgr.h"\
	".\isammgr\nodemgr.h"\
	".\manufact.h"\
	".\phonelst.h"\
	".\smrtheap.h"\
	".\smrtheap.hpp"\
	".\supercom\supercom.h"\
	

"$(INTDIR)\CELLULARDlg.obj" : $(SOURCE) $(DEP_CPP_CELLULA) "$(INTDIR)"\
 "$(INTDIR)\CELLULAR.pch"


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_CELLULA=\
	".\area.h"\
	".\CELLULAR.h"\
	".\CELLULARDlg.h"\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\isammgr.h"\
	".\isammgr\nodemgr.h"\
	".\manufact.h"\
	".\phonelst.h"\
	".\smrtheap.h"\
	".\smrtheap.hpp"\
	".\StdAfx.h"\
	".\supercom\supercom.h"\
	{$(INCLUDE)}"sys\stat.h"\
	{$(INCLUDE)}"sys\types.h"\
	
NODEP_CPP_CELLULA=\
	".\heapagnt.h"\
	

"$(INTDIR)\CELLULARDlg.obj" : $(SOURCE) $(DEP_CPP_CELLULA) "$(INTDIR)"\
 "$(INTDIR)\CELLULAR.pch"


!ENDIF 

SOURCE=.\Supercom\Comint.c

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_COMIN=\
	".\smrtheap.h"\
	".\supercom\supercom.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Comint.obj" : $(SOURCE) $(DEP_CPP_COMIN) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_COMIN=\
	".\smrtheap.h"\
	".\supercom\supercom.h"\
	
NODEP_CPP_COMIN=\
	".\heapagnt.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Comint.obj" : $(SOURCE) $(DEP_CPP_COMIN) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Supercom\Datalink.c

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_DATAL=\
	".\smrtheap.h"\
	".\supercom\supercom.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Datalink.obj" : $(SOURCE) $(DEP_CPP_DATAL) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_DATAL=\
	".\smrtheap.h"\
	".\supercom\supercom.h"\
	
NODEP_CPP_DATAL=\
	".\heapagnt.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Datalink.obj" : $(SOURCE) $(DEP_CPP_DATAL) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Supercom\Modem.c

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_MODEM=\
	".\smrtheap.h"\
	".\supercom\supercom.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Modem.obj" : $(SOURCE) $(DEP_CPP_MODEM) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_MODEM=\
	".\smrtheap.h"\
	".\supercom\supercom.h"\
	
NODEP_CPP_MODEM=\
	".\heapagnt.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Modem.obj" : $(SOURCE) $(DEP_CPP_MODEM) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fp"$(INTDIR)\CELLULAR.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\CELLULAR.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fp"$(INTDIR)\CELLULAR.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\CELLULAR.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Supercom\Transpor.c

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_TRANS=\
	".\smrtheap.h"\
	".\supercom\supercom.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Transpor.obj" : $(SOURCE) $(DEP_CPP_TRANS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_TRANS=\
	".\smrtheap.h"\
	".\supercom\supercom.h"\
	
NODEP_CPP_TRANS=\
	".\heapagnt.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Transpor.obj" : $(SOURCE) $(DEP_CPP_TRANS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Area.cpp

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_AREA_=\
	".\area.h"\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\isammgr.h"\
	".\isammgr\nodemgr.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Area.obj" : $(SOURCE) $(DEP_CPP_AREA_) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_AREA_=\
	".\area.h"\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\isammgr.h"\
	".\isammgr\nodemgr.h"\
	
CPP_SWITCHES=/nologo /Zp8 /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Area.obj" : $(SOURCE) $(DEP_CPP_AREA_) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Isammgr\Dmcache.cpp

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Dmcache.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

CPP_SWITCHES=/nologo /Zp8 /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Dmcache.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Isammgr\Filemgr.cpp

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_FILEM=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\isammsg.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Filemgr.obj" : $(SOURCE) $(DEP_CPP_FILEM) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_FILEM=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\isammsg.h"\
	
CPP_SWITCHES=/nologo /Zp8 /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Filemgr.obj" : $(SOURCE) $(DEP_CPP_FILEM) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Isammgr\Indexmgr.cpp

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_INDEX=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\nodemgr.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Indexmgr.obj" : $(SOURCE) $(DEP_CPP_INDEX) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_INDEX=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\nodemgr.h"\
	
CPP_SWITCHES=/nologo /Zp8 /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Indexmgr.obj" : $(SOURCE) $(DEP_CPP_INDEX) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Isammgr\Isammgr.cpp

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_ISAMM=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\isammgr.h"\
	".\isammgr\isammsg.h"\
	".\isammgr\nodemgr.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Isammgr.obj" : $(SOURCE) $(DEP_CPP_ISAMM) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_ISAMM=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\isammgr.h"\
	".\isammgr\isammsg.h"\
	".\isammgr\nodemgr.h"\
	
CPP_SWITCHES=/nologo /Zp8 /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Isammgr.obj" : $(SOURCE) $(DEP_CPP_ISAMM) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Isammgr\Isammsg.cpp
DEP_CPP_ISAMMS=\
	".\isammgr\isammsg.h"\
	

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Isammsg.obj" : $(SOURCE) $(DEP_CPP_ISAMMS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

CPP_SWITCHES=/nologo /Zp8 /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Isammsg.obj" : $(SOURCE) $(DEP_CPP_ISAMMS) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Manufact.cpp

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_MANUF=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\isammgr.h"\
	".\isammgr\nodemgr.h"\
	".\manufact.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Manufact.obj" : $(SOURCE) $(DEP_CPP_MANUF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_MANUF=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\isammgr.h"\
	".\isammgr\nodemgr.h"\
	".\manufact.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Manufact.obj" : $(SOURCE) $(DEP_CPP_MANUF) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Isammgr\Nodemgr.cpp
DEP_CPP_NODEM=\
	".\isammgr\isammsg.h"\
	".\isammgr\nodemgr.h"\
	

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Nodemgr.obj" : $(SOURCE) $(DEP_CPP_NODEM) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

CPP_SWITCHES=/nologo /Zp8 /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Nodemgr.obj" : $(SOURCE) $(DEP_CPP_NODEM) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Phonelst.cpp

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_PHONE=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\isammgr.h"\
	".\isammgr\nodemgr.h"\
	".\phonelst.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Phonelst.obj" : $(SOURCE) $(DEP_CPP_PHONE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_PHONE=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\isammgr.h"\
	".\isammgr\nodemgr.h"\
	".\phonelst.h"\
	
CPP_SWITCHES=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Phonelst.obj" : $(SOURCE) $(DEP_CPP_PHONE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Isammgr\Sdiskmgr.cpp
DEP_CPP_SDISK=\
	".\isammgr\diskmgr.h"\
	

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sdiskmgr.obj" : $(SOURCE) $(DEP_CPP_SDISK) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

CPP_SWITCHES=/nologo /Zp8 /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sdiskmgr.obj" : $(SOURCE) $(DEP_CPP_SDISK) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Isammgr\Sidxmgr.cpp

!IF  "$(CFG)" == "CELLULAR - Win32 Release"

DEP_CPP_SIDXM=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\nodemgr.h"\
	
CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D\
 "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sidxmgr.obj" : $(SOURCE) $(DEP_CPP_SIDXM) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "CELLULAR - Win32 Debug"

DEP_CPP_SIDXM=\
	".\isammgr\diskmgr.h"\
	".\isammgr\filemgr.h"\
	".\isammgr\indexmgr.h"\
	".\isammgr\nodemgr.h"\
	
CPP_SWITCHES=/nologo /Zp8 /MTd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D\
 "_WINDOWS" /D "_OS_NT_" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\Sidxmgr.obj" : $(SOURCE) $(DEP_CPP_SIDXM) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

