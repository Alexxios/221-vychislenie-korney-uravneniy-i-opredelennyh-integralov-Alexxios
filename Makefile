CFLAGS += -std=gnu99
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

test: integral
	./integral -R 5:6:0.0:2.0:0.0001:1.6481
	./integral -R 3:4:-0.5:3.0:0.0001:0.669
	./integral -R 4:6:-0.5:2.0:0.0001:1.7296

	./integral -I 4:0.0:3.0:0.000001:2.545177
	./integral -I 5:1.0:4.0:0.000001:3.725483
	./integral -I 6:-2.0:0.0:0.000001:3.141593
	