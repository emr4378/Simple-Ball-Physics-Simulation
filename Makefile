#
# Created by gmakemake
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
# and linking C and C++ programs which use OpenGL, GLUT, and/or SOIL.
#
# To use the GNU C or C++ compiler, you will need to uncomment one of
# the following lines:
#
#   CC = gcc
#   CCC = g++
#   CXX = g++
#

#
# Include file locations
#
GLGLUTINC = /opt/csw/include

INCLUDE = -I$(GLGLUTINC)

#
# If you are working on a SPARC-based system (e.g., the compute server
# 'queeg'), replace -lSOIL with -lSOIL-sparc in the LDLIBS macro.
#
GLGLUTLIB = /opt/csw/lib
GLGLUTLIBDIRS = -L$(GLGLUTLIB)

LIBDIRS = $(GLGLUTLIBDIRS)

LDLIBS = -lglut -lGLU -lGL -lXmu -lXext -lX11 -lm

#
# Compilation and linking flags
#
# If you want to take advantage of GDB's extra debugging features,
# change "-g" in the CFLAGS and LIBFLAGS macro definitions to "-ggdb".
#
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
	$(CXX) $(CXXFLAGS) -o project4 project4.o $(OBJFILES) $(CCLIBFLAGS)

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
	-/bin/rm $(OBJFILES) project4.o core 2> /dev/null

realclean:        clean
	-/bin/rm -rf project4 
