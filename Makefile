CC := gcc
CFLAGS := -lX11 -Os -std=c11

all: makehcx

makehcx:
	mkdir -p obj/
	$(CC) src/hcx.c $(CFLAGS) -o obj/hcx

clean:
	rm obj/hcx