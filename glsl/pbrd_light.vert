#version 460 core
layout (location = 0) in vec3 pos_in;
layout (location = 1) in vec3 norm_in;
layout (location = 2) in vec2 tex_coord_in;

out vec3 frag_pos;
out vec3 normal;
out vec2 tex_coord;
