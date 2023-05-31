CC=gcc
LN=gcc
BUILD_DIR=build

CFLAGS=
ifeq ($(DEBUG), address)
	CFLAGS+=-fsanitize=address
	CFLAGS+=-g
endif

SRC=main.c
SRC+=terminal_f.c

OUTFILE=output.elf

OBJ=$(SRC:%=$(BUILD_DIR)/%.o)

all: dirs compile link

$(OUTFILE): $(OBJ)
	$(LN) $(CFLAGS) $^ -o $@ -lm

$(BUILD_DIR)/%.c.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@


.PHONY: dirs, compile, link, run, clean, help
dirs:
	mkdir -p $(BUILD_DIR)

compile: $(OBJ)

link: $(OUTFILE)

run:
	./$(OUTFILE)

disassemble:
	objdump -d $(OUTFILE) > $(ASMFILE)

clean:
	rm -r $(BUILD_DIR)
	rm -f $(OUTFILE)

help:
	@echo "Terminal GUI demo help"
	@echo "  Usage: make [options] [targets]"
	@echo "    targets: all | dirs | compile | link | clean | help"
	@echo "    options:"
	@echo "        DEBUG=address (address sanitizer)"
