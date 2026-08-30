# ============================================
# COS214 Prac3 2026 - Mystifare EventFlow
# Makefile for Task 3.1 & 3.2
# ============================================

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
TARGET = eventflow

# Source files (currently only main.cpp - all code is in headers)
SOURCES = main.cpp

# Object files (automatically derived from SOURCES)
OBJECTS = $(SOURCES:.cpp=.o)

# ============================================
# Targets
# ============================================

# Default target: build the executable
all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ============================================
# Utility Targets
# ============================================

# Clean up build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Build and run the program
run: $(TARGET)
	./$(TARGET)

# Run with valgrind to check for memory leaks (optional)
memcheck: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

# ============================================
# Phony Targets (not actual files)
# ============================================

.PHONY: all clean run memcheck