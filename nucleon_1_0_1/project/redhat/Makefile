# Makefile - nucleon.dsw

.PHONY: all
all: \
	meson \
	nucleon \
	quark \
	test

.PHONY: meson
meson:
	$(MAKE) -C ../../../meson_1_0_1/project/redhat -f meson.mak

.PHONY: nucleon
nucleon:
	$(MAKE) -f nucleon.mak

.PHONY: quark
quark:
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak

.PHONY: test
test: nucleon quark meson
	$(MAKE) -f test.mak

.PHONY: meson_clean
meson_clean:
	$(MAKE) -C ../../../meson_1_0_1/project/redhat -f meson.mak clean

.PHONY: nucleon_clean
nucleon_clean:
	$(MAKE) -f nucleon.mak clean

.PHONY: quark_clean
quark_clean:
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak clean

.PHONY: test_clean
test_clean:
	$(MAKE) -f test.mak clean

.PHONY: clean
clean:
	$(MAKE) -C ../../../meson_1_0_1/project/redhat -f meson.mak clean
	$(MAKE) -f nucleon.mak clean
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak clean
	$(MAKE) -f test.mak clean

.PHONY: depends
depends:
	$(MAKE) -C ../../../meson_1_0_1/project/redhat -f meson.mak depends
	$(MAKE) -f nucleon.mak depends
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak depends
	$(MAKE) -f test.mak depends

