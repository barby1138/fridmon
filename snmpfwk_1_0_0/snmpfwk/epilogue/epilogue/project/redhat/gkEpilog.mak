# Makefile - SnmpFwk.dsp

CC=gcc
CFLAGS=-MD
CXX=g++
CXXFLAGS=$(CFLAGS) #-W -fexceptions -fpermissive -fno-operator-names
AR=ar
ARFLAGS=rus

OS=redhat

EPILOGUEROOTDIR=../..
EPILOGUESRCDIR=$(EPILOGUEROOTDIR)/src/epilogue
EPILOGUEHDRDIR=$(EPILOGUEROOTDIR)/include/epilogue

OBJDIR=$(EPILOGUEROOTDIR)/obj/$(OS)/$(CFG)
DESTDIR=$(EPILOGUEROOTDIR)/lib/$(OS)/$(CFG)
TARGET=$(DESTDIR)/libgkEpilog.a

DEFINE=-D_REDHAT

INCLUDE=-I$(EPILOGUEHDRDIR) \
	-I$(EPILOGUEHDRDIR)/port/redhat \
	-I$(EPILOGUEHDRDIR)/envoy/h

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
	$(EPILOGUESRCDIR)/envoy/snmp/access.c \
	$(EPILOGUESRCDIR)/envoy/snmp/asy_back.c \
	$(EPILOGUESRCDIR)/envoy/snmp/asy_util.c \
	$(EPILOGUESRCDIR)/envoy/snmp/bindint.c \
	$(EPILOGUESRCDIR)/envoy/snmp/bindip.c \
	$(EPILOGUESRCDIR)/envoy/snmp/bindnull.c \
	$(EPILOGUESRCDIR)/envoy/snmp/bindoid.c \
	$(EPILOGUESRCDIR)/envoy/snmp/bindstr.c \
	$(EPILOGUESRCDIR)/envoy/snmp/bindui64.c \
	$(EPILOGUESRCDIR)/envoy/snmp/binduint.c \
	$(EPILOGUESRCDIR)/envoy/snmp/buffer.c \
	$(EPILOGUESRCDIR)/envoy/snmp/buildpkt.c \
	$(EPILOGUESRCDIR)/envoy/snmp/context.c \
	$(EPILOGUESRCDIR)/envoy/snmp/creatint.c \
	$(EPILOGUESRCDIR)/envoy/snmp/creatrap.c \
	$(EPILOGUESRCDIR)/envoy/snmp/creatreq.c \
	$(EPILOGUESRCDIR)/envoy/snmp/decode.c \
	$(EPILOGUESRCDIR)/envoy/snmp/encode.c \
	$(EPILOGUESRCDIR)/envoy/snmp/env_init.c \
	$(EPILOGUESRCDIR)/envoy/snmp/etimer.c \
	$(EPILOGUESRCDIR)/envoy/auxi/exist_as.c \
	$(EPILOGUESRCDIR)/envoy/auxi/exists.c \
	$(EPILOGUESRCDIR)/envoy/auxi/gas_incr.c \
	$(EPILOGUESRCDIR)/envoy/auxi/gas_int.c \
	$(EPILOGUESRCDIR)/envoy/auxi/gas_ip.c \
	$(EPILOGUESRCDIR)/envoy/auxi/gas_oid.c \
	$(EPILOGUESRCDIR)/envoy/auxi/gas_str.c \
	$(EPILOGUESRCDIR)/envoy/auxi/gas_ui64.c \
	$(EPILOGUESRCDIR)/envoy/auxi/gas_uint.c \
	$(EPILOGUESRCDIR)/envoy/auxi/gas_ulng.c \
	$(EPILOGUESRCDIR)/envoy/auxi/get_int.c \
	$(EPILOGUESRCDIR)/envoy/auxi/get_ip.c \
	$(EPILOGUESRCDIR)/envoy/auxi/get_oid.c \
	$(EPILOGUESRCDIR)/envoy/auxi/get_str.c \
	$(EPILOGUESRCDIR)/envoy/auxi/get_ui64.c \
	$(EPILOGUESRCDIR)/envoy/auxi/get_uint.c \
	$(EPILOGUESRCDIR)/envoy/auxi/get_ulng.c \
	$(EPILOGUESRCDIR)/envoy/snmp/getpdu.c \
	$(EPILOGUESRCDIR)/envoy/auxi/ip2llist.c \
	$(EPILOGUESRCDIR)/envoy/auxi/ip2rlist.c \
	$(EPILOGUESRCDIR)/envoy/auxi/llistcmp.c \
	$(EPILOGUESRCDIR)/envoy/snmp/localio.c \
	$(EPILOGUESRCDIR)/envoy/snmp/md5.c \
	$(EPILOGUESRCDIR)/envoy/snmp/mib_fon.c \
	$(EPILOGUESRCDIR)/envoy/snmp/mib_next.c \
	$(EPILOGUESRCDIR)/envoy/snmp/mibutils.c \
	$(EPILOGUESRCDIR)/envoy/snmp/nextpdu.c \
	$(EPILOGUESRCDIR)/envoy/auxi/null_gas.c \
	$(EPILOGUESRCDIR)/envoy/auxi/null_get.c \
	$(EPILOGUESRCDIR)/envoy/auxi/null_nas.c \
	$(EPILOGUESRCDIR)/envoy/auxi/null_nxt.c \
	$(EPILOGUESRCDIR)/envoy/auxi/null_sas.c \
	$(EPILOGUESRCDIR)/envoy/auxi/null_set.c \
	$(EPILOGUESRCDIR)/envoy/auxi/null_tas.c \
	$(EPILOGUESRCDIR)/envoy/auxi/null_tst.c \
	$(EPILOGUESRCDIR)/envoy/snmp/objectid.c \
	$(EPILOGUESRCDIR)/envoy/auxi/oid2ip.c \
	$(EPILOGUESRCDIR)/envoy/auxi/oidcmp.c \
	$(EPILOGUESRCDIR)/envoy/auxi/oidcmp2.c \
	$(EPILOGUESRCDIR)/envoy/auxi/oidorder.c \
	$(EPILOGUESRCDIR)/envoy/snmp/proxy.c \
	$(EPILOGUESRCDIR)/envoy/auxi/proxy_v2.c \
	$(EPILOGUESRCDIR)/envoy/snmp/rcv_pkt.c \
	$(EPILOGUESRCDIR)/envoy/auxi/row.c \
	$(EPILOGUESRCDIR)/envoy/auxi/row_get.c \
	$(EPILOGUESRCDIR)/envoy/auxi/rowloose.c \
	$(EPILOGUESRCDIR)/envoy/snmp/rpktasy.c \
	$(EPILOGUESRCDIR)/envoy/snmp/sa_core.c \
	$(EPILOGUESRCDIR)/envoy/snmp/sa_mast.c \
	$(EPILOGUESRCDIR)/envoy/snmp/sa_mutil.c \
	$(EPILOGUESRCDIR)/envoy/snmp/sa_sagt.c \
	$(EPILOGUESRCDIR)/envoy/auxi/sas_incr.c \
	$(EPILOGUESRCDIR)/envoy/auxi/sas_int.c \
	$(EPILOGUESRCDIR)/envoy/auxi/sas_ui64.c \
	$(EPILOGUESRCDIR)/envoy/auxi/sas_uint.c \
	$(EPILOGUESRCDIR)/envoy/auxi/scan_loc.c \
	$(EPILOGUESRCDIR)/envoy/snmp/serial.c \
	$(EPILOGUESRCDIR)/envoy/auxi/set_int.c \
	$(EPILOGUESRCDIR)/envoy/auxi/set_ui64.c \
	$(EPILOGUESRCDIR)/envoy/auxi/set_uint.c \
	$(EPILOGUESRCDIR)/envoy/snmp/setpdu.c \
	$(EPILOGUESRCDIR)/envoy/snmp/snmp_d.c \
	$(EPILOGUESRCDIR)/envoy/snmp/snmp_e.c \
	$(EPILOGUESRCDIR)/envoy/snmp/snmpvars.c \
	$(EPILOGUESRCDIR)/envoy/snmp/tree.c \
	$(EPILOGUESRCDIR)/envoy/snmp/user.c \
	$(EPILOGUESRCDIR)/envoy/snmp/vb_rtns.c \
	$(EPILOGUESRCDIR)/envoy/snmp/view_chk.c

HEADER_FILES= \
	$(EPILOGUEHDRDIR)/envoy/h/access.h \
	$(EPILOGUEHDRDIR)/envoy/h/agent.h \
	$(EPILOGUEHDRDIR)/envoy/h/asn1.h \
	$(EPILOGUEHDRDIR)/envoy/h/asn1conf.h \
	$(EPILOGUEHDRDIR)/envoy/h/auxfuncs.h \
	$(EPILOGUEHDRDIR)/envoy/h/auxmth.h \
	$(EPILOGUEHDRDIR)/envoy/h/ax_mth.h \
	$(EPILOGUEHDRDIR)/envoy/h/bstring.h \
	$(EPILOGUEHDRDIR)/envoy/h/buffer.h \
	$(EPILOGUEHDRDIR)/bug.h \
	$(EPILOGUEHDRDIR)/common/h/bug.h \
	$(EPILOGUEHDRDIR)/bugdef.h \
	$(EPILOGUEHDRDIR)/common/h/bugdef.h \
	$(EPILOGUEHDRDIR)/envoy/h/buildpkt.h \
	$(EPILOGUEHDRDIR)/common/backstop/common.h \
	$(EPILOGUEHDRDIR)/common/h/config.h \
	$(EPILOGUEHDRDIR)/envoy/h/context.h \
	$(EPILOGUEHDRDIR)/envoy/h/decode.h \
	$(EPILOGUEHDRDIR)/envoy/h/encode.h \
	$(EPILOGUEHDRDIR)/port/winsock/envoy.h \
	$(EPILOGUEHDRDIR)/envoy/h/etimer.h \
	$(EPILOGUEHDRDIR)/common/h/glue.h \
	$(EPILOGUEHDRDIR)/install.h \
	$(EPILOGUEHDRDIR)/envoy/h/localio.h \
	$(EPILOGUEHDRDIR)/envoy/h/md5.h \
	$(EPILOGUEHDRDIR)/envoy/h/mib.h \
	$(EPILOGUEHDRDIR)/envoy/h/objectid.h \
	$(EPILOGUEHDRDIR)/common/backstop/oemtypes.h \
	$(EPILOGUEHDRDIR)/envoy/h/proxy_tr.h \
	$(EPILOGUEHDRDIR)/envoy/h/snmp.h \
	$(EPILOGUEHDRDIR)/envoy/h/snmpdefs.h \
	$(EPILOGUEHDRDIR)/envoy/h/snmpstat.h \
	$(EPILOGUEHDRDIR)/port/winsock/snmptalk.h \
	$(EPILOGUEHDRDIR)/envoy/h/snmpvars.h \
	$(EPILOGUEHDRDIR)/envoy/h/subagent.h \
	$(EPILOGUEHDRDIR)/envoy/h/tree.h \
	$(EPILOGUEHDRDIR)/common/h/types.h \
	$(EPILOGUEHDRDIR)/types.h \
	$(EPILOGUEHDRDIR)/envoy/h/ui64.h \
	$(EPILOGUEHDRDIR)/envoy/h/user.h \
	$(EPILOGUEHDRDIR)/envoy/h/vbdefs.h \
	$(EPILOGUEHDRDIR)/envoy/h/version.h \
	$(EPILOGUEHDRDIR)/envoy/h/view.h \
	$(EPILOGUEHDRDIR)/envoy/h/wkobj.h

SRCS=$(SOURCE_FILES) $(HEADER_FILES)
OBJS=$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(subst $(EPILOGUESRCDIR),$(OBJDIR),$(filter %.c %.cc %.cpp %.cxx,$(SRCS)))))))
DEPS=$(patsubst %.o,%.d,$(OBJS))

.PHONY: all
all: $(TARGET)

$(DESTDIR): 
	-mkdir -p $(DESTDIR)

$(OBJS):
	-mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -o $@ -c $(wildcard $(patsubst %.o,%.c*,$(subst $(OBJDIR),$(EPILOGUESRCDIR),$@)))

$(TARGET): $(OBJS) $(DESTDIR)
	$(AR) $(ARFLAGS) $@ $(OBJS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET)

-include $(DEPS)
