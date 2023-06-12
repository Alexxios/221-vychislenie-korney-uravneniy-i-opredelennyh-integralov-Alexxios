CFLAGS ?= -O2 -g

CFLAGS += -std=gnu99

CFLAGS += -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
	-Wswitch-default -Wpointer-arith -Wtype-limits -Wempty-body \
	-Wstrict-prototypes -Wold-style-declaration -Wold-style-definition \
	-Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs \
	-Wstack-usage=4096 -Wmissing-prototypes -Wfloat-equal -Wabsolute-value

CFLAGS += -fsanitize=undefined -fsanitize-undefined-trap-on-error

CC += -m32 -no-pie -fno-pie

LDLIBS = -lm

.DELETE_ON_ERROR:

.PHONY: all
all: integral

integral: functions.o integral.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

functions.o: functions.asm
	nasm -f elf32 -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o integral


.PHONY: test
test: $(wildcard *.c)
	ls -la $?