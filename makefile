
# ============================================
# COS214 Prac3 2026 - Mystifare EventFlow
# Makefile for Task 3.1 testing
# ============================================

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
TARGET = eventflow
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# For marker: just build
.PHONY: all clean run