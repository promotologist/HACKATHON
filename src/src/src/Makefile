# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include

# Target executable name
TARGET = atomcamp_lms

# Source files
SRCS = src/main.cpp \
       src/Learner.cpp \
       src/Course.cpp \
       src/AdaptiveEngine.cpp \
       src/InstructorDashboard.cpp \
       src/FeedbackLoop.cpp \
       src/LecturerDashboard.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# For Windows (MinGW)
run-win: $(TARGET)
	$(TARGET).exe

# Help
help:
	@echo "Available commands:"
	@echo "  make       - Build the project"
	@echo "  make run   - Build and run (Linux/Mac)"
	@echo "  make run-win - Build and run (Windows)"
	@echo "  make clean - Remove compiled files"
	@echo "  make help  - Show this help"

.PHONY: all clean run run-win help