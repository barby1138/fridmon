# Makefile - quark.dsw

.PHONY: all
all: \
	meson \
	quark \
	test

.PHONY: meson
meson:
	$(MAKE) -C ../../../meson_1_0_1/project/redhat -f meson.mak

.PHONY: quark
quark:
	$(MAKE) -f quark.mak

.PHONY: test
test: quark meson
	$(MAKE) -f test.mak

.PHONY: meson_clean
meson_clean:
	$(MAKE) -C ../../../meson_1_0_1/project/redhat -f meson.mak clean

.PHONY: quark_clean
quark_clean:
	$(MAKE) -f quark.mak clean

.PHONY: test_clean
test_clean:
	$(MAKE) -f test.mak clean

.PHONY: clean
clean:
	$(MAKE) -C ../../../meson_1_0_1/project/redhat -f meson.mak clean
	$(MAKE) -f quark.mak clean
	$(MAKE) -f test.mak clean

