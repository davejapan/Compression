IDIR = header
ODIR = src
LIBS = -pthread
BDIR = build

CC=gcc
LD=gcc

CFLAGS=-Wall -I $(IDIR)

_DEPS = rle.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_OBJS = main.o decode.o encode.o
OBJS = $(patsubst %, $(ODIR)/%, $(_OBJS))

all: run_length_encoding

$(ODIR)/%.o: src/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run_length_encoding: $(OBJS)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -rf build/*
	rm -rf src/*.o
rebuild: clean all

.PHONY : clean
.SILENT : clean
