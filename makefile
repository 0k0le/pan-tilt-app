# Matthew Todd Geiger
# Build file

APPNAME=camapp

CC=g++
BUILDFLAGS=-Wall -Wextra -pedantic -fpic -std=c++2a -c -O2 -I src/include `pkg-config Qt5Widgets --cflags` -DQT_NO_VERSION_TAGGING -msse -msse2 -msse3 -mfpmath=sse `pylon-config --cflags` `pkg-config Qt5Gui --cflags` `pkg-config Qt5Core --cflags`
LINKERFLAGS=`pkg-config Qt5Widgets --libs` `pylon-config --libs --libs-rpath` -lpthread `pkg-config Qt5Gui --libs` `pkg-config Qt5Core --libs`

SRCDIR=src
BUILDDIR=bin
INTBUILDDIR=bin-int

debug: BUILDFLAGS+= -D_DEBUG

all: force $(BUILDDIR)/$(APPNAME)
	@echo Build System Exiting

$(BUILDDIR)/$(APPNAME): $(INTBUILDDIR)/main.o $(INTBUILDDIR)/pylon-video.o $(INTBUILDDIR)/gui.o
	$(CC) $(INTBUILDDIR)/main.o $(INTBUILDDIR)/pylon-video.o $(INTBUILDDIR)/gui.o $(LINKERFLAGS) -o $(BUILDDIR)/$(APPNAME)

$(INTBUILDDIR)/gui.o: $(SRCDIR)/gui.cpp
	moc $(SRCDIR)/include/gui.hpp > $(SRCDIR)/moc/gui.moc
	$(CC) $(SRCDIR)/gui.cpp -o $(INTBUILDDIR)/gui.o -I ./ $(BUILDFLAGS)

$(INTBUILDDIR)/main.o: $(SRCDIR)/main.cpp
	$(CC) $(SRCDIR)/main.cpp -o $(INTBUILDDIR)/main.o $(BUILDFLAGS)

$(INTBUILDDIR)/pylon-video.o: $(SRCDIR)/pylon-video.cpp
	$(CC) $(SRCDIR)/pylon-video.cpp -o $(INTBUILDDIR)/pylon-video.o $(BUILDFLAGS)

debug: all

clean:
	rm -rf bin/* bin-int/*

force:
	mkdir -p bin bin-int
