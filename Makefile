CXX := g++
CXXFLAGS := -std=gnu++17 -O2 -Wall -Wextra 
TARGET := shell 

SRC := main.cpp display.cpp builtin.cpp  \
       ls.cpp syscmd.cpp pinfo.cpp search.cpp \
       redirect.cpp pipeline.cpp  \
       signals.cpp autocomplete.cpp history.cpp 

OBJ := $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) -lreadline

%.o: %.cpp header.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
