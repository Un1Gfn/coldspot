include ./transferconf
N:=ninja -j4 -v -C $(O)

default: pingtransfer

# run:
# env G_DEBUG=fatal-warnings GTK_DEBUG=interactive $(BIN)

pingtransfer: build
	[ aarch64 = "$(shell uname -m)" ]
	@echo $@...
	@echo 'foo' | busybox nc -w0 127.0.0.1 $(PORT)

# order-only prerequisite
# no rebuild on outdate
build: | $(O)
	[ aarch64 = "$(shell uname -m)" ]
# 	meson compile -v -C $(O)
	touch buildtime.c
	$(N)

setup: | $(O)

$(O):
	[ aarch64 = "$(shell uname -m)" ]
	@echo "/Makefile --> /meson.build --> /builddir/build.ninja"
	@rm -rf $(O)
	meson setup $(O) .

# gdb: build
# 	gdb -ex run $(O)/backlight_gui

# https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/Makefile?h=v5.19#n1561
clean:
	{ [ -e $(O) ] && $(N) -t clean; } || true
	@rm -fv cscope.out
distclean:
	@rm -rfv $(O)/
mrproper:
	@rm -rfv $(O)/
