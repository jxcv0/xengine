add_library(stb STATIC stb_image.c stb_truetype.c)
target_include_directories(stb PUBLIC .)