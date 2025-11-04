CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = Lab10

all: $(TARGET)

$(TARGET): Lab10.cpp
	$(CXX) $(CXXFLAGS) Lab10.cpp -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
