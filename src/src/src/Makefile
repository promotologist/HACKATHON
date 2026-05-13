CXX = g++
CXXFLAGS = -std=c++11 -Wall -I include
TARGET = lms
SRCS = src/main.cpp src/Learner.cpp src/Course.cpp src/AdaptiveEngine.cpp src/InstructorDashboard.cpp src/FeedbackLoop.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)