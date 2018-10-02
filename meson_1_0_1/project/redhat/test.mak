# Makefile - test.dsp

CC=gcc
CFLAGS=-MD
CXX=g++
CXXFLAGS=$(CFLAGS) -W -fexceptions -std=c++11
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=

OS=redhat

ROOTDIR=../..
SRCDIR=$(ROOTDIR)/test

OBJDIR=$(ROOTDIR)/obj/$(OS)/$(CFG)
LIBDESTDIR=lib/$(OS)/$(CFG)
DESTDIR=$(ROOTDIR)/bin/$(OS)/$(CFG)
TARGET=$(DESTDIR)/test.exe

QUARKDIR=../$(ROOTDIR)/quark_1_0_1
MESONDIR=$(ROOTDIR)/meson

COMMONLIBS=-lpthread -lrt
SHAREDLIBS=-lmeson -lquark

DEFINE=-D_REDHAT
INCLUDE=-I$(ROOTDIR) -I$(QUARKDIR)

LDFLAGS+=-L/usr/lib -L$(ROOTDIR)/$(LIBDESTDIR) -L$(QUARKDIR)/$(LIBDESTDIR) -Wl

ifndef CFG
CFG=debug
endif

ifeq "$(CFG)"  "release"
CFLAGS+=-O2
DEFINE+=-DNDEBUG
else
ifeq "$(CFG)"  "debug"
CFLAGS+=-g -O0
DEFINE+=-D_DEBUG
endif
endif

CFLAGS+=$(DEFINE) $(INCLUDE)

SRC=$(SRCDIR)/*.cpp
HDR=$(SRCDIR)/*.h

SRCS=$(wildcard $(SRC) $(HDR))
OBJS=$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(subst $(ROOTDIR),$(OBJDIR),$(filter %.c %.cc %.cpp %.cxx,$(SRCS)))))))
DEPS=$(patsubst %.o,%.d,$(OBJS))

.PHONY: all
all: $(TARGET)

$(DESTDIR): 
	-mkdir -p $(DESTDIR)

$(OBJS):
	-mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $(wildcard $(patsubst %.o,%.c*,$(subst $(OBJDIR),$(ROOTDIR),$@)))

$(TARGET): $(OBJS) $(DESTDIR) 
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(SHAREDLIBS) $(COMMONLIBS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) test.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > test.dep

-include test.dep
-include $(DEPS)
