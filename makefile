MAKEFLAGS := --jobs=$(shell nproc)

build_dir := build
bin_dir := bin
lib_dir := lib

xen_src_dir := src
xen_include_dir := include
xen_test_dir := test
stb_src_dir := $(lib_dir)/stb
glad_src_dir := $(lib_dir)/glad

cflags := -Wall -Wextra -ggdb -O0
libs := -Llib -lglad -lstb -lm -ldl -lglfw -fopenmp
xen_lib := -Lbuild -lxen

c_comp := gcc

sh := "/bin/bash"
format_cmd := "/usr/bin/clang-format -i -style=Google"

# TODO make tools target
all: geom_converter tests

$(build_dir)/%.o: %.c
	@echo "Building object $@"
	@if [ ! -d $(build_dir)/% ]; then mkdir -p $(build_dir)/$(dir $<); fi
	@$(c_comp) $< $(cflags) -I$(glad_src_dir) -I$(stb_src_dir) -I$(xen_include_dir) $(libs) -c -o $@

geom_converter: libglad.a libstb.a libxen.a
	@$(c_comp) tools/geom_converter.c $(cflags) -I$(glad_src_dir) -I$(stb_src_dir) -I$(xen_include_dir) $(xen_lib) $(libs) -lassimp -o $(bin_dir)/geom_converter

libxen.a: $(patsubst %.c, $(build_dir)/%.o, $(wildcard $(xen_src_dir)/*.c))
	@echo "Building static library $@"
	@ar rcs build/$@ $^

libglad.a: 
	@$(c_comp) $(cflags) -I$(glad_src_dir) $(glad_src_dir)/glad.c -c -o $(build_dir)/glad.o
	@echo "Building static library $@"
	@ar rcs $(build_dir)/$@ $(build_dir)/glad.o

libstb.a:
	@$(c_comp) $(cflags) -I$(stb_src_dir) $(stb_src_dir)/stb_image.c -c -o $(build_dir)/stb_image.o
	@$(c_comp) $(cflags) -I$(stb_src_dir) $(stb_src_dir)/stb_truetype.c -c -o $(build_dir)/stb_truetype.o
	@echo "Building static library $@"
	@ar rcs $(build_dir)/$@ $(build_dir)/stb_truetype.o $(build_dir)/stb_image.o

tests: test_game lin_tests buffer_tests mem_tests asset_tests

test_game: libglad.a libstb.a libxen.a
	@echo "building executable $@"
	@$(c_comp) $(xen_test_dir)/$@.c $(cflags) -I$(glad_src_dir) -I$(stb_src_dir) -I$(xen_include_dir) $(xen_lib) $(libs) -o $(bin_dir)/$@

lin_tests: libxen.a
	@echo "building executable $@"
	@$(c_comp) $(xen_test_dir)/$@.c $(cflags) -I$(xen_include_dir) $(xen_lib) $(libs) -o $(bin_dir)/$@

buffer_tests: libxen.a
	@echo "building executable $@"
	@$(c_comp) $(xen_test_dir)/$@.c $(cflags) -I$(xen_include_dir) $(xen_lib) $(libs) -o $(bin_dir)/$@

mem_tests: libxen.a
	@echo "building executable $@"
	@$(c_comp) $(xen_test_dir)/$@.c $(cflags) -I$(xen_include_dir) $(xen_lib) $(libs) -o $(bin_dir)/$@

asset_tests: libxen.a
	@echo "building executable $@"
	@$(c_comp) $(xen_test_dir)/$@.c $(cflags) -I$(xen_include_dir) $(xen_lib) $(libs) -o $(bin_dir)/$@


.PHONY: clean format
clean:
	@rm -rf build/*
	@rm -rf bin/*

format: $(wildcard $(xen_include_dir)/*.h) $(wildcard $(xen_src_dir)/*.c) $(wildcard test/*.c)
	@clang-format -i -style=google $^
