#################################################################################
#####                 This makefile generates ELWCHT.DLL                  #####
#################################################################################
!include $(WGDEV)\src\gcrules.mak

PARSEOPTS = $(PARSEOPTS) -I. -DIS30

VIRDIR = .\GCVIRDAT

DEFILE =

VIRFILES =

DLLLIB =            \
     $(DLLLIB)      \
     wgserver.lib   \
     galgsbl.lib

OBJFILES = \
     elwcht.obj 

start:              \
     $(GCBUILD)     \
     elwcht.dll

elwcht.dll:         \
     $(OBJFILES)    \
     $(DLLLIB)      \
     $(DEFILE)      \
     $(VIRFILES)
     $(LINK) $(LIBCMD) @&&|
$(PRODLL) $(OBJFILES)
$(RUNDIR)\$&
$(RUNDIR)\$&
$(DLLLIB) $(LINKDLL)
$(DEFILE)
|
