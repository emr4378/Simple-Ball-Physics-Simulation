#
# Created by gmakemake (Ubuntu Sep 14 2011) on Sun Jan 29 15:42:48 2012
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

#
# This header.mak file will set up all necessary options for compiling
# and linking C and C++ programs which use OpenGL and/or GLUT on the
# Ubuntu systems.
#
# If you want to take advantage of GDB's extra debugging features,
# change "-g" in the CFLAGS and LIBFLAGS macro definitions to "-ggdb".
#
INCLUDE =
LIBDIRS =

LDLIBS = -lglut -lGLU -lGL -lXext -lX11 -lm

CFLAGS = -g $(INCLUDE)
CCFLAGS =  $(CFLAGS)
CXXFLAGS = $(CFLAGS)

LIBFLAGS = -g $(LIBDIRS) $(LDLIBS)
CLIBFLAGS = $(LIBFLAGS)
CCLIBFLAGS = $(LIBFLAGS)

########## End of flags from header.mak


CPP_FILES =	ball.cpp drawings.cpp plane.cpp project4.cpp
C_FILES =	
PS_FILES =	
S_FILES =	
H_FILES =	ball.h drawings.h plane.h vecmath.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	ball.o drawings.o plane.o 

#
# Main targets
#

all:	project4 

project4:	project4.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o EduardoRodrigues_BallPhysics_Linux project4.o $(OBJFILES) $(CCLIBFLAGS)

#
# Dependencies
#

ball.o:	ball.h plane.h vecmath.h
drawings.o:	ball.h drawings.h plane.h vecmath.h
plane.o:	plane.h vecmath.h
project4.o:	ball.h drawings.h plane.h vecmath.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) project4.o core

realclean:        clean
	-/bin/rm -f EduardoRodrigues_BallPhysics_Linux 
