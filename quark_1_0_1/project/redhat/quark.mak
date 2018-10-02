# Makefile - quark.dsp

CC=gcc
CFLAGS=-MD
CXX=g++
CXXFLAGS=$(CFLAGS) -W -fexceptions -std=c++11
AR=ar
ARFLAGS=rus

OS=redhat

ROOTDIR=../..
QUARKDIR=$(ROOTDIR)/quark

OBJDIR=$(ROOTDIR)/obj/$(OS)/$(CFG)
DESTDIR=$(ROOTDIR)/lib/$(OS)/$(CFG)
TARGET=$(DESTDIR)/libquark.a

DEFINE=-D_REDHAT -D_REENTRANT
INCLUDE=-I$(ROOTDIR)

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

QUARK=  \
	$(QUARKDIR)/*.h \
	$(QUARKDIR)/*.c*

SERVICE= \
	$(QUARKDIR)/mempool/service/*.h \
	$(QUARKDIR)/mempool/service/*.c*

COMPILER= \
	$(QUARKDIR)/config/compiler/*.h \
	$(QUARKDIR)/config/compiler/*.c*

PLATFORM= \
	$(QUARKDIR)/config/platform/*.h \
	$(QUARKDIR)/config/platform/*.c*

MEMPOOL= \
	$(QUARKDIR)/mempool/*.h \
	$(QUARKDIR)/mempool/*.c*

CONFIG= \
	$(QUARKDIR)/config/*.h \
	$(QUARKDIR)/config/*.c*

STL= \
	$(QUARKDIR)/config/stl/*.h \
	$(QUARKDIR)/config/stl/*.c*

DINKUM= \
	$(QUARKDIR)/config/stl/dinkum/_deque_ \
	$(QUARKDIR)/config/stl/dinkum/_list_ \
	$(QUARKDIR)/config/stl/dinkum/_xtree_ \
	$(QUARKDIR)/config/stl/dinkum/msvc6/allocator.h

SGI=

GLIB= \
	$(QUARKDIR)/config/stl/glib/allocator.h

OSDEPENDENT= \
	$(QUARKDIR)/$(OS)/*.h \
	$(QUARKDIR)/$(OS)/*.c*

SRCS=$(wildcard $(QUARK) $(SERVICE) $(COMPILER) $(PLATFORM) $(MEMPOOL) $(CONFIG) $(STL) $(DINKUM) $(SGI) $(GLIB) $(OSDEPENDENT))
OBJS=$(patsubst %.cxx,$(OBJDIR)/%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(subst $(ROOTDIR),$(OBJDIR),$(filter %.c %.cc %.cpp %.cxx,$(SRCS)))))))
DEPS=$(patsubst %.o,%.d,$(OBJS))

.PHONY: all
all: $(TARGET)

$(DESTDIR): 
	-mkdir -p $(DESTDIR)

$(OBJS):
	-mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $(wildcard $(patsubst %.o,%.c*,$(subst $(OBJDIR),$(ROOTDIR),$@)))

$(TARGET): $(OBJS) $(DESTDIR)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
