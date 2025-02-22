CC = g++
SRCS = $(wildcard src/*.cpp) $(wildcard vecmath/src/*.cpp)
OBJS = $(SRCS:src/%.cpp=bin/%.o)
OBJS := $(OBJS:vecmath/src/%.cpp=bin/%.o)  # Correctly rename vecmath objects
PROG = ./bin/a4
CFLAGS = -O2 -Wall -Wextra
INCFLAGS = -Ivecmath/include


all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

bin/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

bin/%.o: vecmath/src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

clean:
	rm -f *.bak vecmath/src/*.o bin/*.o *.o core.* $(PROG)

render: $(PROG)
	$(PROG) -input ./scenes-mine/planet.txt -output ./image.bmp -size 500 500 -depth ./depth.bmp 5 13.5 -normals ./normals.bmp