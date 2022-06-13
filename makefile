#
# Build file

APPNAME=camapp

CC=g++
BUILDFLAGS=-Wall -Wextra -pedantic -fpic -std=c++2a -c -O2 -I src/include `pkg-config Qt5Widgets --cflags` -DQT_NO_VERSION_TAGGING -msse -msse2 -msse3 -mfpmath=sse `pylon-config --cflags`
LINKERFLAGS=`pkg-config Qt5Widgets --libs` `pylon-config --libs --libs-rpath`

SRCDIR=src
BUILDDIR=bin
INTBUILDDIR=bin-int

debug: BUILDFLAGS+= -D_DEBUG

all: force $(BUILDDIR)/$(APPNAME)
	@echo Build System Exiting

$(BUILDDIR)/$(APPNAME): $(INTBUILDDIR)/main.o $(INTBUILDDIR)/pylon-video.o
	$(CC) $(INTBUILDDIR)/main.o $(INTBUILDDIR)/pylon-video.o $(LINKERFLAGS) -o $(BUILDDIR)/$(APPNAME)

$(INTBUILDDIR)/main.o: $(SRCDIR)/main.cpp
	$(CC) $(SRCDIR)/main.cpp -o $(INTBUILDDIR)/main.o $(BUILDFLAGS)

$(INTBUILDDIR)/pylon-video.o: $(SRCDIR)/pylon-video.cpp
	$(CC) $(SRCDIR)/pylon-video.cpp -o $(INTBUILDDIR)/pylon-video.o $(BUILDFLAGS)

debug: all

clean:
	rm -rf bin/* bin-int/*

force:
	mkdir -p bin bin-int