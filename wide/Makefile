# Makefile for UNIX systems
#
# Found to work fine under Linux with unicode version of wxGTK 2.8

CXX   = g++
STRIP = strip

WX_FLAGS = `wx-config --debug=no --cxxflags`
WX_LIBS  = `wx-config --debug=no --libs aui,stc`

CXXFLAGS = $(WX_FLAGS)
LIBS     = $(WX_LIBS)


all: wide

clean:
	rm -f edit.o main.o prefs.o wide

edit.o: defsext.h edit.cpp edit.h
	$(CXX) $(CXXFLAGS) -c edit.cpp

main.o: defsext.h main.cpp main.h
	$(CXX) $(CXXFLAGS) -c main.cpp

prefs.o: defsext.h prefs.cpp prefs.h
	$(CXX) $(CXXFLAGS) -c prefs.cpp

wide: edit.o main.o prefs.o
	$(CXX) $(LIBS) -o wide edit.o main.o prefs.o
	$(STRIP) -s wide
