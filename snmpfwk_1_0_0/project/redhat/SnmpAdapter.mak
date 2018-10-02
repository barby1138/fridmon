# Makefile - WinSnmpAdapter.dsp

CC=gcc
CFLAGS=-MD
CXX=g++
CXXFLAGS=$(CFLAGS) -W -fexceptions -fno-operator-names -fpermissive
AR=ar
ARFLAGS=rus

OS=redhat

SNMPADPROOTDIR=../..
SNMPADPDIR=$(SNMPADPROOTDIR)/snmpfwk

SRCDIR=$(SNMPADPDIR)/epilogue
EPILOGUEDIR=$(SRCDIR)/epilogue
OBJDIR=$(SNMPADPROOTDIR)/obj/$(OS)/$(CFG)
DESTDIR=$(SNMPADPROOTDIR)/lib/$(OS)/$(CFG)
TARGET=$(DESTDIR)/libSNMPADP.a

QUARKDIR=../$(SNMPADPROOTDIR)/quark_1_0_1
NUCLEONDIR=../$(SNMPADPROOTDIR)/nucleon_1_0_1

DEFINE=-D_REDHAT
INCLUDE=-I$(SNMPADPDIR) \
	-I$(QUARKDIR) \
	-I$(NUCLEONDIR) \
	\
	-I$(EPILOGUEDIR)/include/epilogue \
	-I$(EPILOGUEDIR)/include/epilogue/envoy/h \
	-I$(EPILOGUEDIR)/include/epilogue/common/h \
	-I$(EPILOGUEDIR)/include/epilogue/port/redhat

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
	$(SRCDIR)/EpilogueConvertor.cpp \
	$(SRCDIR)/EpiloguePacket.cpp \
	$(SRCDIR)/PracticalSocket.cpp \
	$(SRCDIR)/SnmpReceiver.cpp \
	$(SRCDIR)/SnmpSenderImpl.cpp \
	$(SNMPADPDIR)/SnmpSender.cpp

HEADER_FILES= \
	$(SRCDIR)/EpilogueConvertor.h \
	$(SRCDIR)/EpiloguePacket.h \
	$(SRCDIR)/PracticalSocket.h \
	$(SRCDIR)/SnmpReceiver.h \
	$(SRCDIR)/SnmpSenderImpl.h \
	$(SNMPADPDIR)/SnmpSender.h

SRCS=$(SOURCE_FILES) $(HEADER_FILES) 
OBJS=$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(subst $(SNMPADPROOTDIR),$(OBJDIR),$(filter %.c %.cc %.cpp %.cxx,$(SRCS)))))))
DEPS=$(patsubst %.o,%.d,$(OBJS))

.PHONY: all
all: $(TARGET)

$(DESTDIR): 
	-mkdir -p $(DESTDIR)

$(OBJS):
	-mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $(wildcard $(patsubst %.o,%.c*,$(subst $(OBJDIR),$(SNMPADPROOTDIR),$@)))

$(TARGET): $(OBJS) $(DESTDIR)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) WinSnmpAdapter.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > WinSnmpAdapter.dep

-include WinSnmpAdapter.dep
-include $(DEPS)
