CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC
AR = ar
ARFLAGS = rcs
PREFIX = /usr/local

LIB_NAME = rio
STATIC_LIB = lib$(LIB_NAME).a
SHARED_LIB = lib$(LIB_NAME).so

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY: all clean install

all: $(OBJ_DIR) $(STATIC_LIB) $(SHARED_LIB)
$(OBJ_DIR):
	mkdir -p bin
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(STATIC_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(SHARED_LIB): $(OBJS)
	$(CC) -shared -o $@ $^
	gcc examples/raw_reads.c -o bin/raw
	gcc examples/rio_reads.c -o bin/rior ./$(SHARED_LIB) -I include

install: all
	install -d $(PREFIX)/lib
	install -d $(PREFIX)/include
	install -m 644 $(STATIC_LIB) $(PREFIX)/lib
	install -m 644 $(SHARED_LIB) $(PREFIX)/lib
	install -m 644 $(INC_DIR)/*.h $(PREFIX)/include

clean:
	rm -rf $(OBJ_DIR) $(STATIC_LIB) $(SHARED_LIB)
	rm -rf bin