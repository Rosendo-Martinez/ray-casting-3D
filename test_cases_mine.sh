#!/bin/sh

# rm *.tga
rm ./images-mine/perspective/*.bmp
rm ./images-mine/perspective/depth/*.bmp
rm ./images-mine/perspective/normals/*.bmp

rm ./images-mine/orthographic/*.bmp
rm ./images-mine/orthographic/depth/*.bmp
rm ./images-mine/orthographic/normals/*.bmp

./bin/a4 -input ./scenes/perspective/scene01_plane.txt      -size 500 500 -output ./images-mine/perspective/scene-1.bmp -depth ./images-mine/perspective/depth/scene-1_depth.bmp 6.5 11.25 -normals ./images-mine/perspective/normals/scene-1_normals.bmp
./bin/a4 -input ./scenes/perspective/scene02_cube.txt       -size 500 500 -output ./images-mine/perspective/scene-2.bmp -depth ./images-mine/perspective/depth/scene-2_depth.bmp 10  13    -normals ./images-mine/perspective/normals/scene-2_normals.bmp
./bin/a4 -input ./scenes/perspective/scene03_sphere.txt     -size 500 500 -output ./images-mine/perspective/scene-3.bmp -depth ./images-mine/perspective/depth/scene-3_depth.bmp 8   10    -normals ./images-mine/perspective/normals/scene-3_normals.bmp
./bin/a4 -input ./scenes/perspective/scene04_axes.txt       -size 500 500 -output ./images-mine/perspective/scene-4.bmp -depth ./images-mine/perspective/depth/scene-4_depth.bmp 2   50    -normals ./images-mine/perspective/normals/scene-4_normals.bmp
./bin/a4 -input ./scenes/perspective/scene05_bunny_200.txt  -size 500 500 -output ./images-mine/perspective/scene-5.bmp -depth ./images-mine/perspective/depth/scene-5_depth.bmp 6.5 11.25 -normals ./images-mine/perspective/normals/scene-5_normals.bmp
./bin/a4 -input ./scenes/perspective/scene06_bunny_1k.txt   -size 500 500 -output ./images-mine/perspective/scene-6.bmp -depth ./images-mine/perspective/depth/scene-6_depth.bmp 6.5 11.25 -normals ./images-mine/perspective/normals/scene-6_normals.bmp
./bin/a4 -input ./scenes/perspective/scene07_shine.txt      -size 500 500 -output ./images-mine/perspective/scene-7.bmp -depth ./images-mine/perspective/depth/scene-7_depth.bmp 6.5 11.25 -normals ./images-mine/perspective/normals/scene-7_normals.bmp
./bin/a4 -input ./scenes/perspective/scene08_c.txt          -size 500 500 -output ./images-mine/perspective/scene-8.bmp -depth ./images-mine/perspective/depth/scene-8_depth.bmp 9   14    -normals ./images-mine/perspective/normals/scene-8_normals.bmp
./bin/a4 -input ./scenes/perspective/scene09_s.txt          -size 500 500 -output ./images-mine/perspective/scene-9.bmp -depth ./images-mine/perspective/depth/scene-9_depth.bmp 9   14    -normals ./images-mine/perspective/normals/scene-9_normals.bmp

./bin/a4 -input ./scenes/orthographic/scene01_plane.txt      -size 500 500 -output ./images-mine/orthographic/scene-1.bmp -depth ./images-mine/orthographic/depth/scene-1_depth.bmp 6.5 11.25 -normals ./images-mine/orthographic/normals/scene-1_normals.bmp
./bin/a4 -input ./scenes/orthographic/scene02_cube.txt       -size 500 500 -output ./images-mine/orthographic/scene-2.bmp -depth ./images-mine/orthographic/depth/scene-2_depth.bmp 10  13    -normals ./images-mine/orthographic/normals/scene-2_normals.bmp
./bin/a4 -input ./scenes/orthographic/scene03_sphere.txt     -size 500 500 -output ./images-mine/orthographic/scene-3.bmp -depth ./images-mine/orthographic/depth/scene-3_depth.bmp 8   10    -normals ./images-mine/orthographic/normals/scene-3_normals.bmp
./bin/a4 -input ./scenes/orthographic/scene04_axes.txt       -size 500 500 -output ./images-mine/orthographic/scene-4.bmp -depth ./images-mine/orthographic/depth/scene-4_depth.bmp 2   50    -normals ./images-mine/orthographic/normals/scene-4_normals.bmp
./bin/a4 -input ./scenes/orthographic/scene05_bunny_200.txt  -size 500 500 -output ./images-mine/orthographic/scene-5.bmp -depth ./images-mine/orthographic/depth/scene-5_depth.bmp 6.5 11.25 -normals ./images-mine/orthographic/normals/scene-5_normals.bmp
./bin/a4 -input ./scenes/orthographic/scene06_bunny_1k.txt   -size 500 500 -output ./images-mine/orthographic/scene-6.bmp -depth ./images-mine/orthographic/depth/scene-6_depth.bmp 6.5 11.25 -normals ./images-mine/orthographic/normals/scene-6_normals.bmp
./bin/a4 -input ./scenes/orthographic/scene07_shine.txt      -size 500 500 -output ./images-mine/orthographic/scene-7.bmp -depth ./images-mine/orthographic/depth/scene-7_depth.bmp 6.5 11.25 -normals ./images-mine/orthographic/normals/scene-7_normals.bmp
./bin/a4 -input ./scenes/orthographic/scene08_c.txt          -size 500 500 -output ./images-mine/orthographic/scene-8.bmp -depth ./images-mine/orthographic/depth/scene-8_depth.bmp 9   14    -normals ./images-mine/orthographic/normals/scene-8_normals.bmp
./bin/a4 -input ./scenes/orthographic/scene09_s.txt          -size 500 500 -output ./images-mine/orthographic/scene-9.bmp -depth ./images-mine/orthographic/depth/scene-9_depth.bmp 9   14    -normals ./images-mine/orthographic/normals/scene-9_normals.bmp
