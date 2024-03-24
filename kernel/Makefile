TARGET = kernel.elf
OBJS = main.o graphics.o font.o hankaku.o newlib_support.o console.o
DEPENDS = $(join $(dir $(OBJS)),$(addprefix .,$(notdir $(OBJS:.o=.d))))

BASEDIR = $(HOME)/osbook/devenv/x86_64-elf
EDK2DIR = $(HOME)/edk2

ifeq "$(wildcard $(BASEDIR))" ""
$(error "$(BASEDIR) が存在しません。以下のファイルを手動でダウンロードし、$(dir $(BASEDIR))に展開してください：https://github.com/uchan-nos/mikanos-build/releases/download/v2.0/x86_64-elf.tar.gz")
endif

export CPPFLAGS = -I$(BASEDIR)/include/c++/v1 -I$(BASEDIR)/include -I$(BASEDIR)/include/freetype2 \
    -I$(EDK2DIR)/MdePkg/Include -I$(EDK2DIR)/MdePkg/Include/X64 \
    -nostdlibinc -D__ELF__ -D_LDBL_EQ_DBL -D_GNU_SOURCE -D_POSIX_TIMERS \
    -DEFIAPI='__attribute__((ms_abi))'

export LDFLAGS = -L$(BASEDIR)/lib

CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
            -fno-exceptions -fno-rtti -std=c++17
LDFLAGS  += --entry KernelMain -z norelro --image-base 0x100000 --static


.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf *.d

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS) -lc

console.o: console.cpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $<

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $<

.%.d: %.cpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.cpp=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@

%.o: %.c Makefile
	clang $(CPPFLAGS) $(CFLAGS) -c $<

hankaku.bin: hankaku.txt
	../tools/makefont.py -o $@ $<

hankaku.o: hankaku.bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@

.%.d: %.bin
	touch $@

.PHONY: depends
depends:
	$(MAKE) $(DEPENDS)

-include $(DEPENDS)