SUBDIRS = lib/glad lib/stb
AM_CPPFLAGS = -I$(srcdir)/lib/glad -I$(srcdir)/lib/stb -I$(srcdir)
AM_LDFLAGS = -fopenmp

bin_PROGRAMS = xen
xen_SOURCES = shader.c assets.c gamestate.c mmapfile.c checkerr.c lin.c window.c input.c pbr_deferred.c systems.c camera.c main.c utils.c
xen_LDADD = lib/glad/libglad.a lib/stb/libstb.a -lm -ldl -lglfw
