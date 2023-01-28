build_dir := build
bin_dir := bin
cflags := -Wall -Wextra
libs := -Lbuild -lglad -lstb -lm -lglfw -fopenmp

xen_src_dir := src
stb_src_dir := lib/stb
glad_src_dir := lib/glad

game: libglad.a libstb.a 
	@echo "Building target $@"
	@gcc $(cflags) -I src -I lib/stb -I lib/glad game/main.c $(wildcard $(xen_src_dir)/*.c) $(libs) -o $(bin_dir)/game

mkbin:
	@if [ ! -d $(build_dir)/$(bin_dir) ]; then mkdir $(build_dir)/$(bin_dir); fi

$(build_dir)/%.o: %.c
	@echo "Building object $@"
	@if [ ! -d $(build_dir)/%.o ]; then mkdir -p $(build_dir)/$(dir $<); fi
	@gcc -c -o $@ $< $(cflags) -I $(dir $<)

libglad.a: $(patsubst %.c, $(build_dir)/%.o, $(wildcard $(glad_src_dir)/*.c))
	@echo "Building static library $@"
	@ar rcs $(build_dir)/$@ $^

libstb.a: $(patsubst %.c, $(build_dir)/%.o, $(wildcard $(stb_src_dir)/*.c))
	@echo "Building static library $@"
	@ar rcs $(build_dir)/$@ $^

