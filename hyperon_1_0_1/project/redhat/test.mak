# Makefile - test.dsp

CC=gcc
CFLAGS=-MD
CXX=g++
CXXFLAGS=$(CFLAGS) -W -fexceptions -fpermissive
LD=$(CXX) $(CXXFLAGS)
LDFLAGS=

OS=redhat

ROOTDIR=../..
SRCDIR=$(ROOTDIR)/test

OBJDIR=$(ROOTDIR)/obj/$(OS)/$(CFG)
LIBDESTDIR=lib/$(OS)/$(CFG)
DESTDIR=$(ROOTDIR)/bin/$(OS)/$(CFG)
TARGET=$(DESTDIR)/test.exe

COMMONLIBS=-lpthread -lrt
SHAREDLIBS=-lexpat -lhyperon -lquark

QUARKDIR=../$(ROOTDIR)/quark_1_0_1
MESONDIR=../$(ROOTDIR)/meson_1_0_1
NUCLEONDIR=../$(ROOTDIR)/nucleon_1_0_1

DEFINE=-D_REDHAT
INCLUDE=-I$(ROOTDIR) \
	-I$(QUARKDIR) \
	-I$(MESONDIR) \
	-I$(NUCLEONDIR)

LDFLAGS+=-L$(ROOTDIR)/$(LIBDESTDIR) \
	-L$(QUARKDIR)/$(LIBDESTDIR) \
	-L$(MESONDIR)/$(LIBDESTDIR) \
	-L../../hyperon/storage/xml/expat/lib/$(OS)/$(CFG) -Wl

ifndef CFG
CFG=debug
endif

ifeq "$(CFG)"  "release"
CFLAGS+=-Os
DEFINE+=-DNDEBUG
else
ifeq "$(CFG)"  "debug"
CFLAGS+=-g -O0
DEFINE+=-D_DEBUG
endif
endif

CFLAGS+=$(DEFINE) $(INCLUDE)

SOURCE_FILES= \
	$(SRCDIR)/attribute.cpp \
	$(SRCDIR)/element.cpp \
	$(SRCDIR)/ini_storage.cpp \
	$(SRCDIR)/main.cpp \
	$(SRCDIR)/properties.cpp \
	$(SRCDIR)/test.cpp \
	$(SRCDIR)/xml_storage.cpp

HEADER_FILES= \
	$(SRCDIR)/attribute.h \
	$(SRCDIR)/element.h \
	$(SRCDIR)/ini_storage.h \
	$(SRCDIR)/properties.h \
	$(SRCDIR)/test.h \
	$(SRCDIR)/xml_storage.h

SRCS=$(SOURCE_FILES) $(HEADER_FILES) 
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
