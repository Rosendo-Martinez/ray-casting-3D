#!/bin/sh

./bin/a4 -input ./scenes-mine/skybox-top.txt    -size 500 500 -output ./skybox-images/skybox-top.bmp    -normals ./skybox-images/skybox-top_normals.bmp
./bin/a4 -input ./scenes-mine/skybox-back.txt   -size 500 500 -output ./skybox-images/skybox-back.bmp   -normals ./skybox-images/skybox-back_normals.bmp
./bin/a4 -input ./scenes-mine/skybox-bottom.txt -size 500 500 -output ./skybox-images/skybox-bottom.bmp -normals ./skybox-images/skybox-bottom_normals.bmp
./bin/a4 -input ./scenes-mine/skybox-front.txt  -size 500 500 -output ./skybox-images/skybox-front.bmp  -normals ./skybox-images/skybox-front_normals.bmp
./bin/a4 -input ./scenes-mine/skybox-left.txt   -size 500 500 -output ./skybox-images/skybox-left.bmp   -normals ./skybox-images/skybox-left_normals.bmp
./bin/a4 -input ./scenes-mine/skybox-right.txt  -size 500 500 -output ./skybox-images/skybox-right.bmp  -normals ./skybox-images/skybox-right_normals.bmp
