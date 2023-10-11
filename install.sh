#!/bin/sh
sudo apt-get install libglew-dev libglfw3-dev meson 
git clone https://github.com/recp/cglm
cd cglm
meson build
cd build
ninja
sudo ninja install
