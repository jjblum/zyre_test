CXX = g++
CPPFLAGS += -I/usr/local/include
CXXFLAGS += -std=c++11
LDFLAGS += -L/usr/local/lib \
           -lzyre -lczmq -lzmq \
           -lpthread
ALLFLAGS += $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS)

ROBOT_EXECUTABLE = robot

ROBOT_PATH = .
ROBOT_SOURCES := $(shell find $(ROBOT_PATH) -name '*.cpp')
ROBOT_OBJECTS=$(ROBOT_SOURCES:.cpp=.o)

all: clean $(ROBOT_EXECUTABLE)

$(ROBOT_EXECUTABLE): $(ROBOT_OBJECTS)
	$(CXX) $^ $(ALLFLAGS) -o $@

clean:
	rm -f $(ROBOT_EXECUTABLE) $(ROBOT_OBJECTS)
