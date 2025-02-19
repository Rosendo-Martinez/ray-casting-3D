#!/bin/sh

# rm *.tga
rm ./images-solution/*.bmp

./bin/a4soln -input ./scenes/scene01_plane.txt      -size 500 500 -output ./images-solution/scene-1.bmp
./bin/a4soln -input ./scenes/scene02_cube.txt       -size 500 500 -output ./images-solution/scene-2.bmp
./bin/a4soln -input ./scenes/scene03_sphere.txt     -size 500 500 -output ./images-solution/scene-3.bmp
./bin/a4soln -input ./scenes/scene04_axes.txt       -size 500 500 -output ./images-solution/scene-4.bmp
./bin/a4soln -input ./scenes/scene05_bunny_200.txt  -size 500 500 -output ./images-solution/scene-5.bmp
./bin/a4soln -input ./scenes/scene06_bunny_1k.txt   -size 500 500 -output ./images-solution/scene-6.bmp
./bin/a4soln -input ./scenes/scene07_shine.txt      -size 500 500 -output ./images-solution/scene-7.bmp
./bin/a4soln -input ./scenes/scene08_c.txt          -size 500 500 -output ./images-solution/scene-8.bmp
./bin/a4soln -input ./scenes/scene09_s.txt          -size 500 500 -output ./images-solution/scene-9.bmp
