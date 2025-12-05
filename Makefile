CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

TARGET = test_runner

all: $(TARGET)

$(TARGET): tests/test_runner.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
