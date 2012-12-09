CC=g++
LD=g++

CFLAGS=-Wall
LDFLAGS=-Wall

SRCS=bruteForce.cpp nearestNeighbour.cpp MST_approx.cpp GA.cpp main.cpp antColony.cpp
HEADERS=bruteForce.h nearestNeighbour.h MST_approx.h GA.h main.h antColony.h
TARGET=prog

OBJS=$(addprefix ./obj/, $(addsuffix .o, $(SRCS)))
HDRS=$(addprefix ./src/, $(HEADERS))

$(TARGET): $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
./obj/%.cpp.o: ./src/%.cpp $(HDRS)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm $(OBJS) $(TARGET)
