# Microsoft Developer Studio Project File - Name="prekin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=prekin - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "prekin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "prekin.mak" CFG="prekin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "prekin - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "prekin - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "prekin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "prekin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "prekin - Win32 Release"
# Name "prekin - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\PKINANGL.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINCOUT.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINCRTL.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINCSBS.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINCSUB.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINDLOG.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINFILE.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINHBND.cpp
# End Source File
# Begin Source File

SOURCE=.\PKININIT.cpp
# End Source File
# Begin Source File

SOURCE=.\PKININPT.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINMENU.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINRIBB.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINRIBN.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINRNGE.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINROTL.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINRSUB.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINSCRT.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINSPOS.cpp
# End Source File
# Begin Source File

SOURCE=.\PKINUTIL.cpp
# End Source File
# Begin Source File

SOURCE=.\PKPCDLOG.cpp
# End Source File
# Begin Source File

SOURCE=.\PKPCFILE.cpp
# End Source File
# Begin Source File

SOURCE=.\PKPCINIT.cpp
# End Source File
# Begin Source File

SOURCE=.\PKPCMAIN.cpp
# End Source File
# Begin Source File

SOURCE=.\PKPCMENU.cpp
# End Source File
# Begin Source File

SOURCE=.\PKPCOUTP.cpp
# End Source File
# Begin Source File

SOURCE=.\PKPCTEXT.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\PKIN.h
# End Source File
# Begin Source File

SOURCE=.\PKINDLOG.h
# End Source File
# Begin Source File

SOURCE=.\PKINMENU.h
# End Source File
# Begin Source File

SOURCE=.\PKINRIBB.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\PKPCPKIN.ico
# End Source File
# Begin Source File

SOURCE=.\PKPCPKIN.rc
# End Source File
# End Group
# End Target
# End Project
