BUILD_FLAGS		= -O3 -Wall
BUILD_PATH		= ./bin
OTH_SRC			= ./src/othello.cpp ./src/capture.cpp
BINS			= $(BUILD_PATH)/othello


all: $(BINS)
install: clean $(BINS)

.PHONY: all clean install

$(BINS): | $(BUILD_PATH)

$(BUILD_PATH):
	mkdir -p $(BUILD_PATH)

clean:
	rm -rf $(BUILD_PATH)

$(BUILD_PATH)/othello: $(OTH_SRC)
	g++ $^ $(BUILD_FLAGS) -o $@
