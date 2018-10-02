# Makefile - SnmpFwk.dsp

CC=gcc
CFLAGS=-MD
CXX=g++
CXXFLAGS=$(CFLAGS) -W -fexceptions -fpermissive
AR=ar
ARFLAGS=rus

OS=redhat

SNMPFWKROOTDIR=../..
SNMPFWKDIR=$(SNMPFWKROOTDIR)/snmpfwk

OBJDIR=$(SNMPFWKROOTDIR)/obj/$(OS)/$(CFG)
DESTDIR=$(SNMPFWKROOTDIR)/lib/$(OS)/$(CFG)
TARGET=$(DESTDIR)/libSnmpFwk.a

QUARKDIR=../$(SNMPFWKROOTDIR)/quark_1_0_1
NUCLEONDIR=../$(SNMPFWKROOTDIR)/nucleon_1_0_1

DEFINE=-D_REDHAT -D_EPILOGUE -D__LITTLE_ENDIAN__
INCLUDE=-I$(SNMPFWKDIR) \
	-I$(QUARKDIR) \
	-I$(NUCLEONDIR)

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

SOURCE_FILES= \
	$(SNMPFWKDIR)/SnmpMultiRoot.cpp \
	$(SNMPFWKDIR)/SnmpNode.cpp \
	$(SNMPFWKDIR)/SnmpTable.cpp \
	$(SNMPFWKDIR)/SnmpTextCon.cpp \
	$(SNMPFWKDIR)/SnmpTypes.cpp \
	$(SNMPFWKDIR)/SnmpUserTypes.cpp
#	$(SNMPFWKDIR)/SnmpSender.cpp

HEADER_FILES= \
	$(SNMPFWKDIR)/SnmpAny.h \
	$(SNMPFWKDIR)/SnmpInterfaces.h \
	$(SNMPFWKDIR)/SnmpLeaf.h \
	$(SNMPFWKDIR)/SnmpMultiRoot.h \
	$(SNMPFWKDIR)/SnmpNode.h \
	$(SNMPFWKDIR)/SnmpSender.h \
	$(SNMPFWKDIR)/SnmpTable.h \
	$(SNMPFWKDIR)/SnmpTextCon.h \
	$(SNMPFWKDIR)/SnmpTypes.h \
	$(SNMPFWKDIR)/SnmpUserTypes.h

WIN32= \
	$(SNMPFWKDIR)/win32/AsnConvertor.h

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(WIN32) 
OBJS=$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(subst $(SNMPFWKDIR),$(OBJDIR),$(filter %.c %.cc %.cpp %.cxx,$(SRCS)))))))
DEPS=$(patsubst %.o,%.d,$(OBJS))

.PHONY: all
all: $(TARGET)

$(DESTDIR): 
	-mkdir -p $(DESTDIR)

$(OBJS):
	-mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $(wildcard $(patsubst %.o,%.c*,$(subst $(OBJDIR),$(SNMPFWKDIR),$@)))

$(TARGET): $(OBJS) $(DESTDIR)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(DEPS) $(TARGET)

-include $(DEPS)
