OUT_DIR := ./out
PROG_NAME := capi

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,$(OUT_DIR)/%.o,$(SRCS))

$(OUT_DIR)/%.o: %.c | directories | beautify

#	@echo "Compiling: $<"
	gcc -c $< -o $@

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

.PHONY: directories all clean re