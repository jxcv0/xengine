build_dir := build
bin_dir := bin
lib_dir := lib

xen_src_dir := xen
stb_src_dir := $(lib_dir)/stb
glad_src_dir := $(lib_dir)/glad


cflags := -Wall -Wextra -ggdb -Og
libs := -Llib -lglad -lstb -lm -ldl -lglfw -fopenmp
xen_lib := -Lbuild -lxen

$(build_dir)/%.o: %.c
	@echo "Building object $@"
	@if [ ! -d $(build_dir)/% ]; then mkdir -p $(build_dir)/$(dir $<); fi
	@gcc $< $(cflags) -Og -I$(glad_src_dir) -I$(stb_src_dir) -I$(dir $<) $(libs) -c -o $@

game: libglad.a libstb.a libxen.a
	@gcc game/main.c $(cflags) -I$(glad_src_dir) -I$(stb_src_dir) -I$(xen_src_dir) $(xen_lib) $(libs) -o $(bin_dir)/game

libxen.a: $(patsubst %.c, $(build_dir)/%.o, $(wildcard $(xen_src_dir)/*.c))
	@echo "Building target $@"
	@ar rcs build/$@ $^

converter: libglad.a libstb.a 
	@echo "Building target $@"
	@gcc tools/converter.c $(cflags) -I src tools/converter.c $(wildcard $(xen_src_dir)/*.c) $(libs) -lassimp -o $(bin_dir)/converter

libglad.a: 
	@gcc $(cflags) -I$(glad_src_dir) $(glad_src_dir)/glad.c -c -o $(build_dir)/glad.o
	@echo "Building static library $@"
	@ar rcs $(lib_dir)/$@ $(build_dir)/glad.o

libstb.a:
	@gcc $(cflags) -I$(stb_src_dir) $(stb_src_dir)/stb_image.c -c -o $(build_dir)/stb_image.o
	@gcc $(cflags) -I$(stb_src_dir) $(stb_src_dir)/stb_truetype.c -c -o $(build_dir)/stb_truetype.o
	@echo "Building static library $@"
	@ar rcs $(lib_dir)/$@ $(build_dir)/stb_truetype.o $(build_dir)/stb_image.o

clean:
	@rm -rf build/*
	@rm -rf bin/*

