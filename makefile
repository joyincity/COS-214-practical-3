# ============================================
# COS214 Prac3 2026 - Mystifare EventFlow
# Makefile for Tasks 3.1, 3.2, 3.3, 4.2
# ============================================

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g
TARGET = eventflow

# All code is in headers, so we only compile main.cpp
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run
run: $(TARGET)
	./$(TARGET)

# Check for memory leaks (optional)
memcheck: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

.PHONY: all clean run memcheck