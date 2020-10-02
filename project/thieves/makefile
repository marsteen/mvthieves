#**********************************************************************
#
# thieves - makefile
#
# written 2009 by Martin Steen
# martin@martin-steen.de
#
#**********************************************************************
#
# C++ : Name of C++ Compiler
#
C++       = g++
#
#
#
INCPATH       = -I../include -I./src/2.0/include -I./src/2.0/shaders
LIBBASE       = ../lib
SRCBASE       = ./src
#
#
#
.SUFFIXES : .c .cpp .o
#
#
#

include cppfiles.mk
include cfiles.mk

#************************************************************
#
# Liste der C SourceFiles
#




CCFLAGS = -O2 $(INCPATH) -s
#
TARGET   = thieves
#
default : $(TARGET)
#
#
.cpp.o:
	$(C++) -c $(CCFLAGS) -o$@ $<
#
#
#
.c.o:
	gcc -c $(CCFLAGS) -o$@ $<


#
clean:
	rm $(OBJECTS1) $(OBJECTS2)
#
OBJECTS1 = $(CPPFILES:.cpp=.o)
OBJECTS2 = $(CFILES:.c=.o)
#
LDFLAGS = -static-libstdc++ -lGL -lGLEW -lSDL -Wl,-static -lpng -lz
#
#
# Making the target-exe
	#
$(TARGET) : $(OBJECTS1) $(OBJECTS2)
	$(C++) -o $(TARGET) $(OBJECTS1)  $(OBJECTS2) $(LDFLAGS) $(CCFLAGS)
#
#

