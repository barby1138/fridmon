# Makefile - meson.dsw

.PHONY: all
all: \
	meson \
	quark \
	test

.PHONY: meson
meson:
	$(MAKE) -f meson.mak

.PHONY: quark
quark:
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak

.PHONY: test
test: meson quark
	$(MAKE) -f test.mak

.PHONY: meson_clean
meson_clean:
	$(MAKE) -f meson.mak clean

.PHONY: quark_clean
quark_clean:
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak clean

.PHONY: test_clean
test_clean:
	$(MAKE) -f test.mak clean

.PHONY: clean
clean:
	$(MAKE) -f meson.mak clean
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak clean
	$(MAKE) -f test.mak clean

.PHONY: depends
depends:
	$(MAKE) -f meson.mak depends
	$(MAKE) -C ../../../quark_1_0_1/project/redhat -f quark.mak depends
	$(MAKE) -f test.mak depends

