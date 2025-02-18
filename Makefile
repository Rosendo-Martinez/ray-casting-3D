CC = g++
SRCS = $(wildcard src/*.cpp) $(wildcard vecmath/src/*.cpp)
OBJS = $(SRCS:src/%.cpp=bin/%.o)
OBJS := $(OBJS:vecmath/src/%.cpp=bin/%.o)  # Correctly rename vecmath objects
PROG = ./a4
CFLAGS = -O2 -Wall -Wextra
INCFLAGS = -Ivecmath/include

# Print variables to debug
# $(info SRCS = $(SRCS))
# $(info OBJS = $(OBJS))
# $(info PROG = $(PROG))
# $(info CFLAGS = $(CFLAGS))
# $(info INCFLAGS = $(INCFLAGS))

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

bin/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

bin/%.o: vecmath/src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCFLAGS)

clean:
	rm -f *.bak vecmath/src/*.o bin/*.o *.o core.* $(PROG)

render1: $(PROG)
	$(PROG) -input ./scenes/scene01_plane.txt -output ./images-mine/image.bmp -size 500 500 -depth ./images-mine/depth.bmp 6.5 11.25 -normals ./images-mine/normals.bmp

render2: $(PROG)
	$(PROG) -input ./scenes/scene02_cube.txt -output ./images-mine/image.bmp -size 500 500 -depth ./images-mine/depth.bmp 10 13 -normals ./images-mine/normals.bmp

render3: $(PROG)
	$(PROG) -input ./scenes/scene03_sphere.txt -output ./images-mine/image.bmp -size 500 500 -depth ./images-mine/depth.bmp 8 10 -normals ./images-mine/normals.bmp

render5: $(PROG)
	$(PROG) -input ./scenes/scene05_bunny_200.txt -output ./images-mine/image.bmp -size 500 500 -depth ./images-mine/depth.bmp 8 10 -normals ./images-mine/normals.bmp

render8: $(PROG)
	$(PROG) -input ./scenes/scene08_c.txt -output ./images-mine/image.bmp -size 500 500 -depth ./images-mine/depth.bmp 8 10 -normals ./images-mine/normals.bmp

renderMy: $(PROG)
	$(PROG) -input ./scenes/my_scene.txt -output ./images-mine/image.bmp -size 500 500 -depth ./images-mine/depth.bmp 6.5 11.25 -normals ./images-mine/normals.bmp