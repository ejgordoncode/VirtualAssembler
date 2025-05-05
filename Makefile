CXX = g++
CXXFLAGS = -std=c++11 -Wall
OBJS = main.o assembler.o parser.o
TARGET = essy

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp assembler.h parser.h
	$(CXX) $(CXXFLAGS) -c main.cpp

assembler.o: assembler.cpp assembler.h parser.h
	$(CXX) $(CXXFLAGS) -c assembler.cpp

parser.o: parser.cpp parser.h
	$(CXX) $(CXXFLAGS) -c parser.cpp

clean:
	rm -f *.o $(TARGET) *.l *.st
