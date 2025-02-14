# Define the compiler
CXX = g++

# Define compiler flags
CXXFLAGS = -std=c++17 -Wall -O2

# Define the target executable
TARGET = chess_game

# Define the build directory
BUILD_DIR = build

# Define the source file
SRC = test.cpp

# Define the object file (placed in the build directory)
OBJ = $(BUILD_DIR)/$(SRC:.cpp=.o)

# Define the dependency file (placed in the build directory)
DEP = $(BUILD_DIR)/$(SRC:.cpp=.d)

# Rule to build the final executable (placed in the build directory)
$(BUILD_DIR)/$(TARGET): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

# Rule to compile the source file into an object file and generate a dependency file
$(OBJ): $(SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include the dependency file
-include $(DEP)

# Rule to clean up build files
clean:
	rm -rf $(BUILD_DIR)
