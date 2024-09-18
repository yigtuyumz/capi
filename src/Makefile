CC := clang
CFLAGS = -Wall -Wextra
# LDFLAGS = 
OUT_DIR := ./out
PROG_NAME := capi
SRCS := $(shell find . -maxdepth 1 -name '*.c' | sed 's/^\.\///')
OBJS := $(patsubst %.c,$(OUT_DIR)/%.o,$(SRCS))

COLOR_RED    := \033[0;31m
COLOR_GREEN  := \033[0;32m
COLOR_YELLOW := \033[0;33m
COLOR_BLUE   := \033[0;34m
COLOR_RESET  := \033[0m

# default target is DEBUG
DEBUG ?= 1

ifeq ($(DEBUG),1)
	LDFLAGS += -L../build/lib/ -ljson-c -Wl,-rpath=../build/lib -fsanitize=address -fno-omit-frame-pointer -O1
	CFLAGS += -DCAPI_DEBUG=1 -g -fsanitize=address -fno-omit-frame-pointer -O1
endif


$(OUT_DIR)/%.o: %.c | directories
	@printf "$(COLOR_YELLOW)[ . ]$(COLOR_RESET) '%s'" "$<"
	@$(CC) $(CFLAGS) -c $< -o $@
	@printf "\r$(COLOR_GREEN)[ + ]$(COLOR_RESET) '%s'\n" "$(OUT_DIR)/$@"

$(PROG_NAME): $(OBJS)
#	@echo "Linking: $@"
	@printf "$(COLOR_RED)[ L ]$(COLOR_RESET) $(PROG_NAME)"
	@gcc -o $@ $(OBJS) $(LDFLAGS)
	@printf "\r$(COLOR_GREEN)[ DONE ] $(COLOR_RED)$@$(COLOR_RESET) with\nCFLAGS  > $(COLOR_YELLOW)$(CFLAGS)$(COLOR_RESET)\nLDFLAGS > $(COLOR_BLUE)$(LDFLAGS)$(COLOR_RESET)\n"

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
