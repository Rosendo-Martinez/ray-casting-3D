#!/bin/sh
rm *.tga
rm *.bmp

./a4 -input ./scenes/scene01_plane.txt      -size 200 200 -output ./images/1.bmp
./a4 -input ./scenes/scene02_cube.txt       -size 200 200 -output ./images/2.bmp
./a4 -input ./scenes/scene03_sphere.txt     -size 200 200 -output ./images/3.bmp
./a4 -input ./scenes/scene04_axes.txt       -size 200 200 -output ./images/4.bmp
./a4 -input ./scenes/scene05_bunny_200.txt  -size 200 200 -output ./images/5.bmp
./a4 -input ./scenes/scene06_bunny_1k.txt   -size 200 200 -output ./images/6.bmp
./a4 -input ./scenes/scene07_shine.txt      -size 200 200 -output ./images/7.bmp
./a4 -input ./scenes/scene08_c.txt          -size 200 200 -output ./images/8.bmp
./a4 -input ./scenes/scene09_s.txt          -size 200 200 -output ./images/9.bmp
