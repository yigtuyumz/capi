CC := gcc
CFLAGS = -Wall -Wextra
OUT_DIR := ./out
PROG_NAME := capi
SRCS := $(shell find . -maxdepth 1 -name '*.c' | sed 's/ /\\ /g')
OBJS := $(patsubst %.c,$(OUT_DIR)/%.o,$(SRCS))

COLOR_RED    := \033[0;31m
COLOR_GREEN  := \033[0;32m
COLOR_YELLOW := \033[0;33m
COLOR_BLUE   := \033[0;34m
COLOR_RESET  := \033[0m

$(OUT_DIR)/%.o: %.c | directories
	@printf "$(COLOR_YELLOW)[ . ]$(COLOR_RESET) '%s'" "$<"
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "\r$(COLOR_GREEN)[ + ]$(COLOR_RESET) '%s'\n" "$(OUT_DIR)/$@"

$(PROG_NAME): $(OBJS)
#	@echo "Linking: $@"
	gcc -o $@ $(OBJS)

all: $(PROG_NAME)

directories:
	@mkdir -p $(OUT_DIR)

clean:
	@rm -rf $(OUT_DIR)
	@rm -rf $(PROG_NAME)

re: clean all

tests:
	@cd ../tests && make -s re

.PHONY: directories all clean re tests
