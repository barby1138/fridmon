# Makefile - nucleon.dsp

CC=gcc
CFLAGS=-MD
CXX=g++
CXXFLAGS=$(CFLAGS) -W -Wno-pmf-conversions -Weffc++ -fexceptions -fms-extensions -fpermissive
AR=ar
ARFLAGS=rus

OS=redhat

ROOTDIR=../..
QUARKDIR=../$(ROOTDIR)/quark_1_0_1
MESONDIR=../$(ROOTDIR)/meson_1_0_1
NUCLEONDIR=$(ROOTDIR)/nucleon

OBJDIR=$(ROOTDIR)/obj/$(OS)/$(CFG)
DESTDIR=$(ROOTDIR)/lib/$(OS)/$(CFG)
TARGET=$(DESTDIR)/libnucleon.a

DEFINE=-D_REDHAT -D_REENTRANT
INCLUDE=-I$(ROOTDIR) -I$(QUARKDIR)

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

SRCS= \
	$(NUCLEONDIR)/*.h \
	$(NUCLEONDIR)/*.c*

SRCS=$(wildcard $(QUARK) $(SERVICE) $(COMPILER) $(PLATFORM) $(MEMPOOL) $(CONFIG) $(STL) $(DINKUM) $(SGI) $(GLIB) $(OSDEPENDENT))
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
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) nucleon.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > nucleon.dep

-include nucleon.dep
-include $(DEPS)
