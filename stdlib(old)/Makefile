I_DIR = include
L_DIR = lib
O_DIR = build
B_DIR = bin
S_DIR = src

CFLAGS=-std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(I_DIR)

AS=i686-elf-as
CC=i686-elf-gcc

default: init stdlib

$(O_DIR)/%.o: $(S_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(O_DIR)/%.o: $(S_DIR)/%.s
	$(AS) $< -o $@ $(AFLAGS)


stdlib: $(addprefix $(O_DIR)/, $(addsuffix .o, $(basename $(shell cd $(S_DIR) && find $@ -name '*.c'))))
	ar rcs $(L_DIR)/stdlib.a $^

init:
	mkdir -p $(O_DIR)
	mkdir -p $(B_DIR)
	mkdir -p $(L_DIR)
clean:
	rm -rf ./$(O_DIR)
	rm -rf ./$(B_DIR)

