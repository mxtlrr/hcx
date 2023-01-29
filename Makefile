CC := gcc
CFLAGS := -lX11 -Os -std=c11

all: hcx

hcx:
	$(CC) src/hcx.c $(CFLAGS) -o hcx