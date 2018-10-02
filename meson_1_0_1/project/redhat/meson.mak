# Makefile - meson.dsp

CC=gcc
CFLAGS=-MD
CXX=g++
CXXFLAGS=$(CFLAGS) -W -fexceptions -std=c++11
AR=ar
ARFLAGS=rus

OS=redhat

MESONROOTDIR=../..

QUARKDIR=../$(MESONROOTDIR)/quark_1_0_1
MESONDIR=$(MESONROOTDIR)/meson

OBJDIR=$(MESONROOTDIR)/obj/$(OS)/$(CFG)
DESTDIR=$(MESONROOTDIR)/lib/$(OS)/$(CFG)
TARGET=$(DESTDIR)/libmeson.a

DEFINE=-D_REDHAT -D_REENTRANT
INCLUDE=-I$(MESONROOTDIR) -I$(QUARKDIR)

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

MESON= \
	$(MESONDIR)/*.h \
	$(MESONDIR)/*.c*

PROFILER= \
	$(MESONDIR)/profiler/*.h \
	$(MESONDIR)/profiler/*.c*

TRACER= \
	$(MESONDIR)/tracer/*.h \
	$(MESONDIR)/tracer/*.c*

OSDEPENDENT= \
	$(MESONDIR)/tracer/$(OS)/*.h \
	$(MESONDIR)/tracer/$(OS)/*.c*

SRCS=$(wildcard $(MESON) $(PROFILER) $(TRACER) $(OSDEPENDENT))
OBJS=$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(subst $(MESONROOTDIR),$(OBJDIR),$(filter %.c %.cc %.cpp %.cxx,$(SRCS)))))))
DEPS=$(patsubst %.o,%.d,$(OBJS))

.PHONY: all
all: $(TARGET)

$(DESTDIR): 
	-mkdir -p $(DESTDIR)

$(OBJS):
	-mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $(wildcard $(patsubst %.o,%.c*,$(subst $(OBJDIR),$(MESONROOTDIR),$@)))

$(TARGET): $(OBJS) $(DESTDIR)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) meson.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > meson.dep

-include meson.dep
-include $(DEPS)
