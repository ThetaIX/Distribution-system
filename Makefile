CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
LDFLAGS = -lpcap
#LDFLAGS = $(shell pkg-config --libs libpcap)
#INCLUDES = $(shell pkg-config --cflags libpcap)

SRC = src/main.cpp src/packet_processor.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = data_distributor

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJ) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
#	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

