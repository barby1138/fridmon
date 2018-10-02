# Makefile - SnmpFwk.dsw

.PHONY: all
all: \
	epilogue \
	SnmpAdapter \
	SnmpFwk

.PHONY: epilogue
epilogue:
	$(MAKE) -C ../../snmpfwk/epilogue/epilogue/project/redhat -f Makefile

.PHONY: SnmpAdapter
SnmpAdapter: epilogue
	$(MAKE) -f SnmpAdapter.mak

.PHONY: SnmpFwk
SnmpFwk: SnmpAdapter
	$(MAKE) -f SnmpFwk.mak

.PHONY: clean
clean:
	$(MAKE) -C ../../snmpfwk/epilogue/epilogue/project/redhat -f Makefile clean
	$(MAKE) -f SnmpAdapter.mak clean
	$(MAKE) -f SnmpFwk.mak clean

