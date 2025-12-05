CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

TARGET = test_runner

all: $(TARGET)

TEST_SRCS = $(wildcard tests/*.cpp)

$(TARGET): $(TEST_SRCS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
