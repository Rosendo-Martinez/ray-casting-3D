CC = g++
SRCS = $(wildcard src/*.cpp) $(wildcard vecmath/src/*.cpp)
OBJS = $(SRCS:src/%.cpp=bin/%.o)
OBJS := $(OBJS:vecmath/src/%.cpp=bin/%.o)  # Correctly rename vecmath objects
PROG = ./bin/a4
CFLAGS = -O2 -Wall -Wextra -g
INCFLAGS = -Ivecmath/include -Iinclude/


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
	$(PROG) -input ./scenes/perspective/scene21_moon_earth.txt -output ./image.bmp -size 600 600 -depth ./depth.bmp 8 9.7 -normals ./normals.bmp