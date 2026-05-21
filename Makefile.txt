CXX = g++
CXXFLAGS = -std=c++11 -Wall

TARGET = post

SRCS = main.cpp PochtaThings.cpp OtdelenieStuff.cpp BazaDannih.cpp VremyaMachinery.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
