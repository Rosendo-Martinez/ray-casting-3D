#!/bin/sh

# rm *.tga
rm ./images-mine/**.bmp

./bin/a4 -input ./scenes/scene01_plane.txt      -size 500 500 -output ./images-mine/scene-1.bmp -depth ./images-mine/depth/scene-1_depth.bmp 6.5 11.25 -normals ./images-mine/normals/scene-1_normals.bmp
./bin/a4 -input ./scenes/scene02_cube.txt       -size 500 500 -output ./images-mine/scene-2.bmp -depth ./images-mine/depth/scene-2_depth.bmp 10  13    -normals ./images-mine/normals/scene-2_normals.bmp
./bin/a4 -input ./scenes/scene03_sphere.txt     -size 500 500 -output ./images-mine/scene-3.bmp -depth ./images-mine/depth/scene-3_depth.bmp 8   10    -normals ./images-mine/normals/scene-3_normals.bmp
./bin/a4 -input ./scenes/scene04_axes.txt       -size 500 500 -output ./images-mine/scene-4.bmp -depth ./images-mine/depth/scene-4_depth.bmp 2   30    -normals ./images-mine/normals/scene-4_normals.bmp
./bin/a4 -input ./scenes/scene05_bunny_200.txt  -size 500 500 -output ./images-mine/scene-5.bmp -depth ./images-mine/depth/scene-5_depth.bmp 6.5 11.25 -normals ./images-mine/normals/scene-5_normals.bmp
./bin/a4 -input ./scenes/scene06_bunny_1k.txt   -size 500 500 -output ./images-mine/scene-6.bmp -depth ./images-mine/depth/scene-6_depth.bmp 6.5 11.25 -normals ./images-mine/normals/scene-6_normals.bmp
./bin/a4 -input ./scenes/scene07_shine.txt      -size 500 500 -output ./images-mine/scene-7.bmp -depth ./images-mine/depth/scene-7_depth.bmp 6.5 11.25 -normals ./images-mine/normals/scene-7_normals.bmp
./bin/a4 -input ./scenes/scene08_c.txt          -size 500 500 -output ./images-mine/scene-8.bmp -depth ./images-mine/depth/scene-8_depth.bmp 9   14    -normals ./images-mine/normals/scene-8_normals.bmp
./bin/a4 -input ./scenes/scene09_s.txt          -size 500 500 -output ./images-mine/scene-9.bmp -depth ./images-mine/depth/scene-9_depth.bmp 9   14    -normals ./images-mine/normals/scene-9_normals.bmp