TARGET := sel
BUILD := build
SRC_DIR := src
INCLUDE_DIR := include

CC := gcc
CFLAGS := -std=c11 -Wall -Wswitch -Wextra -g -I$(INCLUDE_DIR)
DEPFLAGS := -MMD -MP

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all test fmt clean

all: $(BUILD)/$(TARGET)

# link
$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# compile .c to .o
$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CFLAGS) $(DEPFLAGS) -c -o $@ $<

$(BUILD):
	mkdir -p $@

test: $(BUILD)/$(TARGET)
	@sh tests/run.sh

fmt:
	clang-format -i $(SRC_DIR)/*.c $(INCLUDE_DIR)/app/*.h

clean:
	rm -rf $(BUILD)

-include $(DEPS)
