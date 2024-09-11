CC := gcc
CFLAGS := -Wall -Wextra -std=c99
SOURCES := $(shell find . -maxdepth 1 -name '*.c' | sed 's/ /\\ /g')
OBJECTS := $(SOURCES:.c=.o)
EXECUTABLES := $(SOURCES:.c=)
OUT_DIR := ./out

COLOR_RED    := \033[0;31m
COLOR_GREEN  := \033[0;32m
COLOR_YELLOW := \033[0;33m
COLOR_BLUE   := \033[0;34m
COLOR_RESET  := \033[0m

%: %.c
	@printf "$(COLOR_YELLOW)[ . ]$(COLOR_RESET) '%s'" "$<"
	@$(CC) $(CFLAGS) "$<" -o "$(OUT_DIR)/$@"
	@printf "\r$(COLOR_GREEN)[ + ]$(COLOR_RESET) '%s'\n" "$(OUT_DIR)/$@"

all: directories $(EXECUTABLES)


clean:
	@echo "Cleaned previous build."
	@rm -rf $(OUT_DIR)

directories:
	@if [ ! -d "$(OUT_DIR)" ]; then \
		echo "Created $(OUT_DIR)/"; \
		mkdir -p $(OUT_DIR); \
	fi

re: clean all

.PHONY: all clean re directories
