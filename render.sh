#!/bin/sh

# Remove old images
rm -rf ./images/perspective/*.bmp ./images/perspective/depth/*.bmp ./images/perspective/normals/*.bmp
rm -rf ./images/orthographic/*.bmp ./images/orthographic/depth/*.bmp ./images/orthographic/normals/*.bmp

# Read scenes from a heredoc
while IFS=" " read -r name depth_min depth_max; do
    # Perspective rendering
    ./bin/a4 -input "./scenes/perspective/$name.txt" \
             -size 500 500 \
             -output "./images/perspective/$name.bmp" \
             -depth "./images/perspective/depth/$name.bmp" $depth_min $depth_max \
             -normals "./images/perspective/normals/$name.bmp"

    # Check if an orthographic scene exists before rendering
    if [ -f "./scenes/orthographic/${name}.txt" ]; then
        # Orthographic rendering
        ./bin/a4 -input "./scenes/orthographic/$name.txt" \
                -size 500 500 \
                -output "./images/orthographic/$name.bmp" \
                -depth "./images/orthographic/depth/$name.bmp" $depth_min $depth_max \
                -normals "./images/orthographic/normals/$name.bmp"
    fi
done <<EOF
scene01_plane 6.5 11.25
scene02_cube 10 13
scene03_sphere 8 10
scene04_axes 2 50
scene05_bunny_200 6.5 11.25
scene06_bunny_1k 6.5 11.25
scene07_shine 6.5 11.25
scene08_c 9 14
scene09_s 9 14
scene10_skybox_back 0 1
scene11_skybox_bottom 0 1
scene12_skybox_front 0 1
scene13_skybox_left 0 1
scene14_skybox_right 0 1
scene15_skybox_top 0 1
scene16_rectangle_with_bumps 9.0 11
scene17_earth 8 10
EOF
