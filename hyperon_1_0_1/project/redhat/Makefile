# Makefile - hyperon.dsw

.PHONY: all
all: \
	hyperon \
	quark \
	test

.PHONY: hyperon
hyperon:
	$(MAKE) -f hyperon.mak

.PHONY: quark
quark:
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak

.PHONY: test
test: hyperon quark
	$(MAKE) -f test.mak

.PHONY: clean
clean:
	$(MAKE) -f hyperon.mak clean
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak clean
	$(MAKE) -f test.mak clean

.PHONY: depends
depends:
	$(MAKE) -f hyperon.mak depends
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak depends
	$(MAKE) -f test.mak depends

