CXX := g++
STD := -std=c++17
CXXFLAGS := $(STD) -march=native -I./include
BUILD := build
BIN := bin

SRCS_TOKENIZER := programs/tokenizer.cxx $(shell find src/tokenizer/ -type f -regex '.*\.\(cxx\|cpp\)$$')
OBJS_TOKENIZER := $(patsubst %, $(BUILD)/%.o, $(SRCS_TOKENIZER:/=_))

SRCS_TOKENPARSER := programs/tokenparser.cxx $(shell find src/tokenparser/ src/tokenizer/tokens.cxx -type f -regex '.*\.\(cxx\|cpp\)$$')
OBJS_TOKENPARSER := $(patsubst %, $(BUILD)/%.o, $(SRCS_TOKENPARSER:/=_))

.PHONY: all lexer parser qcc
all: lexer parser qcc

lexer: $(BIN)/tokenizer
parser: $(BIN)/tokenparser
qcc: $(BIN)/qcc

$(BIN)/tokenizer: $(OBJS_TOKENIZER)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/tokenparser: $(OBJS_TOKENPARSER)
	@mkdir -p $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BIN)/qcc: ./programs/compiler.cxx
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/%.o: %
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf bin build
