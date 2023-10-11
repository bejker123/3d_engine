SHELL=/bin/bash
CC = g++
project_name = linux_game
build_path = build/
src_path = src
executable_path = ${build_path}${project_name}.o
build_path = build/
compiler_args = -lcglm -lglfw -lGLEW -lGL -lm -lX11 -lpthread -lXrandr -ldl# -O3 -fvisibility=hidden -s -static

.PHONY : build run clean install

run: build
	@echo Running..
	@clear
	@${executable_path}
build:
	@echo Building..
	@#${CC} main.cpp -o ${executable_path}
	${CC} ./${src_path}/* ${compiler_args} -o ${executable_path} 
clean:
	@echo Cleaning..
	@rm -rfv ${build_path}*.o
install:
	@sh ./install.sh
