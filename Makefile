SHELL=/bin/bash
CC = g++
project_name = linux_game
build_path = build/
src_path = src/game.cpp src/game.hpp src/logger.hpp src/main.cpp src/rendering/batch.cpp src/rendering/batch.hpp src/rendering/buffers.cpp src/rendering/buffers.hpp src/rendering/camera.cpp src/rendering/camera.hpp src/rendering/material.cpp src/rendering/material.hpp src/rendering/mesh.cpp src/rendering/mesh.hpp src/rendering/model.cpp src/rendering/model.hpp src/rendering/shader.cpp src/rendering/shader.hpp src/rendering/texture.cpp src/rendering/texture.hpp src/rendering/vertex_array.cpp src/rendering/vertex_array.hpp src/window.cpp src/window.hpp
executable_path = ${build_path}${project_name}.o
build_path = build/
compiler_args = -lassimp -lcglm -lglfw -lGLEW -lGL -lm -lX11 -lpthread -lXrandr -ldl# -O3 -fvisibility=hidden -s -static

.PHONY : build run clean install

run: build
	@echo Running..
	@clear
	@${executable_path}
build:
	@echo Building..
	@#${CC} main.cpp -o ${executable_path}
	${CC} ./${src_path} ${compiler_args} -o ${executable_path} 
clean:
	@echo Cleaning..
	@rm -rfv ${build_path}*.o
install:
	@sh ./install.sh
