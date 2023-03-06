build_dir := build
bin_dir := bin
lib_dir := lib

xen_src_dir := xen
stb_src_dir := $(lib_dir)/stb
glad_src_dir := $(lib_dir)/glad

cflags := -Wall -Wextra -ggdb -O0
libs := -Llib -lglad -lstb -lm -ldl -lglfw -fopenmp
xen_lib := -Lbuild -lxen

c_comp := gcc
cpp_comp := g++

sh := "/bin/bash"
format_cmd := "/usr/bin/clang-format -i -style=Google"

all: game geom_converter tests

$(build_dir)/%.o: %.c
	@echo "Building object $@"
	@if [ ! -d $(build_dir)/% ]; then mkdir -p $(build_dir)/$(dir $<); fi
	@$(c_comp) $< $(cflags) -Og -I$(glad_src_dir) -I$(stb_src_dir) -I$(dir $<) $(libs) -c -o $@

$(build_dir)/%.o: %.cpp
	@echo "Building object $@"
	@if [ ! -d $(build_dir)/% ]; then mkdir -p $(build_dir)/$(dir $<); fi
	@$(cpp_comp) $< $(cflags) -Og -I$(glad_src_dir) -I$(stb_src_dir) -I$(dir $<) $(libs) -c -o $@

game: libglad.a libstb.a libxen.a
	@echo "building executable $@"
	@g++ game/main.cpp $(cflags) -I$(glad_src_dir) -I$(stb_src_dir) -I$(xen_src_dir) $(xen_lib) $(libs) -o $(bin_dir)/game

geom_converter: libglad.a libstb.a libxen.a
	@$(c_comp) tools/geom_converter.c $(cflags) -I$(glad_src_dir) -I$(stb_src_dir) -I$(xen_src_dir) $(xen_lib) $(libs) -lassimp -o $(bin_dir)/geom_converter

libxen.a: $(patsubst %.c, $(build_dir)/%.o, $(wildcard $(xen_src_dir)/*.c)) $(patsubst %.cpp, $(build_dir)/%.o, $(wildcard $(xen_src_dir)/*.cpp))
	@echo "Building static library $@"
	@ar rcs build/$@ $^

libglad.a: 
	@$(c_comp) $(cflags) -I$(glad_src_dir) $(glad_src_dir)/glad.c -c -o $(build_dir)/glad.o
	@echo "Building static library $@"
	@ar rcs $(lib_dir)/$@ $(build_dir)/glad.o

libstb.a:
	@$(c_comp) $(cflags) -I$(stb_src_dir) $(stb_src_dir)/stb_image.c -c -o $(build_dir)/stb_image.o
	@$(c_comp) $(cflags) -I$(stb_src_dir) $(stb_src_dir)/stb_truetype.c -c -o $(build_dir)/stb_truetype.o
	@echo "Building static library $@"
	@ar rcs $(lib_dir)/$@ $(build_dir)/stb_truetype.o $(build_dir)/stb_image.o

tests: lin_tests database_tests

lin_tests: libxen.a
	@echo "building executable $@"
	@$(c_comp) test/$@.c $(cflags) -I$(xen_src_dir) $(xen_lib) $(libs) -o $(bin_dir)/$@

database_tests: libxen.a
	@echo "building executable $@"
	@$(c_comp) test/$@.c $(cflags) -I$(xen_src_dir) $(xen_lib) $(libs) -o $(bin_dir)/$@

circular_buffer_tests: libxen.a
	@echo "building executable $@"
	@$(cpp_comp) test/$@.cpp $(cflags) -I$(xen_src_dir) $(xen_lib) $(libs) -o $(bin_dir)/$@

.PHONY: clean format
clean:
	@rm -rf build/*
	@rm -rf bin/*

format: $(wildcard $(xen_src_dir)/*.h) $(wildcard $(xen_src_dir)/*.c) $(wildcard $(xen_src_dir)/*.cpp)
	@clang-format -i -style=Google $^
