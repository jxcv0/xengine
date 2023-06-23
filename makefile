build_dir := build
bin_dir := bin
lib_dir := lib
tests_dir := $(bin_dir)/tests

xen_src_dir := src
xen_include_dir := include
xen_test_dir := test
xen_tools_dir := tools
stb_src_dir := $(lib_dir)/stb
glad_src_dir := $(lib_dir)/glad

cflags := -std=c17 -Wall -Wextra -ggdb -O0
libs := -L$(build_dir) -lglad -lstb -lm -ldl -lglfw -fopenmp -lassimp
xen_lib := -L$(build_dir) -lxen

c_comp := gcc
format_cmd := "/usr/bin/clang-format -i -style=Google"

all: format tests tools test_game

$(build_dir)/%.o: %.c
	@mkdir -p $(@D)
	@echo "Building object $(notdir $@)"
	@$(c_comp) $< $(cflags) -I$(glad_src_dir) -I$(stb_src_dir) -I$(xen_include_dir) $(libs) -c -o $(build_dir)/$(notdir $@)

$(bin_dir)/%: %.c
	@mkdir -p $(@D)
	@echo "Building executable $(notdir $@)"
	@$(c_comp) $^ $(cflags) -I$(glad_src_dir) -I$(stb_src_dir) -I$(xen_include_dir) $(xen_lib) $(libs) -o $@

libxen.a: $(patsubst %.c, $(build_dir)/%.o, $(wildcard $(xen_src_dir)/*.c))
	@echo "Building static library $@"
	@ar rcs $(build_dir)/$@ $(patsubst %, $(build_dir)/%, $(notdir $^))

libglad.a: $(patsubst %.c, $(build_dir)/%.o, $(wildcard $(glad_src_dir)/*.c))
	@echo "Building static library $@"
	@ar rcs $(build_dir)/$@ $(build_dir)/glad.o

libstb.a: $(patsubst %.c, $(build_dir)/%.o, $(wildcard $(stb_src_dir)/*.c))
	@echo "Building static library $@"
	@ar rcs $(build_dir)/$@ $(build_dir)/stb_truetype.o $(build_dir)/stb_image.o

tools: libxen.a libstb.a libglad.a $(patsubst %.c, $(bin_dir)/%, $(wildcard $(xen_tools_dir)/*.c))

tests: libxen.a libstb.a libglad.a $(patsubst %.c, $(bin_dir)/%, $(wildcard $(xen_test_dir)/*.c))

test_game: libglad.a libstb.a libxen.a
	@echo "building executable $@"
	@$(c_comp) $(xen_test_dir)/$@.c $(cflags) -I$(glad_src_dir) -I$(stb_src_dir) -I$(xen_include_dir) $(xen_lib) $(libs) -o $(bin_dir)/$@

.PHONY: clean format
clean:
	@rm -rfd build/*
	@rm -rfd bin/*

format: $(wildcard $(xen_include_dir)/*.h) $(wildcard $(xen_src_dir)/*.c) $(wildcard test/*.c) $(wildcard tools/*.c)
	@clang-format -i -style=google $^
