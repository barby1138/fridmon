# Makefile - hyperon.dsp

CC=gcc
CFLAGS=-MD
CXX=g++
CXXFLAGS=$(CFLAGS) -W -fexceptions -fpermissive -std=c++11 
AR=ar
ARFLAGS=rus

OS=redhat

HYPERONROOTDIR=../..
HYPERONDIR=$(HYPERONROOTDIR)/hyperon

OBJDIR=$(HYPERONROOTDIR)/obj/$(OS)/$(CFG)
DESTDIR=$(HYPERONROOTDIR)/lib/$(OS)/$(CFG)
TARGET=$(DESTDIR)/libhyperon.a

DEFINE=-D_REDHAT -D_REENTRANT -DXML_STATIC

INCLUDE=-I$(HYPERONROOTDIR) \
	-I../../../quark_1_0_1 \
	-I../../../nucleon_1_0_1

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
	$(HYPERONDIR)/attribute.cpp \
	$(HYPERONDIR)/element.cpp \
	$(HYPERONDIR)/gen_attributes.cpp \
	$(HYPERONDIR)/index.cpp

HEADER_FILES= \
	$(HYPERONDIR)/any.h \
	$(HYPERONDIR)/attribute.h \
	$(HYPERONDIR)/dom.h \
	$(HYPERONDIR)/element.h \
	$(HYPERONDIR)/gen_attributes.h \
	$(HYPERONDIR)/index.h \
	$(HYPERONDIR)/properties.h

STORAGE= \
	$(HYPERONDIR)/storage/dummy_storage.h \
	$(HYPERONDIR)/storage/expat_storage.cpp \
	$(HYPERONDIR)/storage/expat_storage.h \
	$(HYPERONDIR)/storage/ini_storage.cpp \
	$(HYPERONDIR)/storage/ini_storage.h

INI= \
	$(HYPERONDIR)/storage/ini/ini_file.cpp \
	$(HYPERONDIR)/storage/ini/ini_file.h

EXPAT=

XML=

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(STORAGE) $(XML) $(EXPAT) $(INI)

OBJS=$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(subst $(HYPERONROOTDIR),$(OBJDIR),$(filter %.c %.cc %.cpp %.cxx,$(SRCS)))))))
DEPS=$(patsubst %.o,%.d,$(OBJS))

.PHONY: all
all: $(TARGET)

$(DESTDIR): 
	-mkdir -p $(DESTDIR)

$(OBJS):
	-mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $(wildcard $(patsubst %.o,%.c*,$(subst $(OBJDIR),$(HYPERONROOTDIR),$@)))

$(TARGET): $(OBJS) $(DESTDIR)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET) hyperon.dep

.PHONY: depends
depends:
	-$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MM $(filter %.c %.cc %.cpp %.cxx,$(SRCS)) > hyperon.dep

-include hyperon.dep
-include $(DEPS)
