#
# Build file

APPNAME=camapp

CC=g++
BUILDFLAGS=-Wall -Wextra -pedantic -std=c++2a -c -O2 -I src/include
LINKERFLAGS=

SRCDIR=src
BUILDDIR=bin
INTBUILDDIR=bin-int

debug: BUILDFLAGS+= -D_DEBUG

all: force $(BUILDDIR)/$(APPNAME)
	@echo Build System Exiting

$(BUILDDIR)/$(APPNAME): $(INTBUILDDIR)/main.o
	$(CC) $(LINKERFLAGS) $(INTBUILDDIR)/main.o -o $(BUILDDIR)/$(APPNAME)

$(INTBUILDDIR)/main.o: $(SRCDIR)/main.cpp
	$(CC) $(SRCDIR)/main.cpp -o $(INTBUILDDIR)/main.o $(BUILDFLAGS)

debug: all

clean:
	rm -rf bin/* bin-int/*

force:
	mkdir -p bin bin-int