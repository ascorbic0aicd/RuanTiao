default: all
CXX = g++  
CXXFLAGS = -Wall -g 
  
SRC_DIR = srcs
BUILD_DIR = build
  
SRCS = $(wildcard $(SRC_DIR)/*.cpp)  
  
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)  
  
TARGET = $(BUILD_DIR)/main  

IDX = 1
$(shell mkdir -p $(BUILD_DIR))  
  
all: $(TARGET)
$(TARGET): $(OBJS)  
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@  

clean:  
	rm -f $(BUILD_DIR)/*  

RAND = 1
run: $(TARGET)
	./PreliminaryJudge ./$(TARGET) -m ./maps/map$(IDX).txt -l NONE -s $(RAND)
replay:
	./replayer/CodeCraft_2024_Replay.x86_64
rmrply:
	rm -f replay/*
.PHONY: all clean help replay rmrply