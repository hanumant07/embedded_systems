CXX = g++
COMMON_FLAGS = -Iinclude -Wall
CXXFLAGS = -std=c++17 $(COMMON_FLAGS)
CFLAGS = $(COMMON_FLAGS)

SRCS = $(wildcard src/*.cpp) $(wildcard src/*.c)
OBJS = $(SRCS:.cpp=.o)
OBJS := $(OBJS:.c=.o)

TARGET = test_runner

all: $(TARGET)

TEST_SRCS = $(wildcard tests/*.cpp) $(wildcard tests/*.c)

$(TARGET): $(TEST_SRCS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)
