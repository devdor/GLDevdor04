WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = -I/usr/local/include/GLFW -I/usr/local/include/assimp -I/usr/local/include/freetype2 -I/usr/local/include/glad/include -I/usr/local/include/stb_image -Iincludes -Iincludes/spline3d -Iincludes/metaballs
CFLAGS = -Wall -fexceptions
RESINC = 
LIBDIR = 
LIB = -lGL -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lassimp -lfreetype
LDFLAGS = 

INC_DEBUG = $(INC)
CFLAGS_DEBUG = $(CFLAGS) -g
RESINC_DEBUG = $(RESINC)
RCFLAGS_DEBUG = $(RCFLAGS)
LIBDIR_DEBUG = $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG = $(LDFLAGS)
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/GLDevdor04

INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/GLDevdor04

OBJ_DEBUG = $(OBJDIR_DEBUG)/src/glad.o $(OBJDIR_DEBUG)/src/util.o $(OBJDIR_DEBUG)/src/main.o \
$(OBJDIR_DEBUG)/src/spline3d/splinegl.o $(OBJDIR_DEBUG)/src/spline3d/interpol_spline.o \
$(OBJDIR_DEBUG)/src/spline3d/interpol_non_fonct.o $(OBJDIR_DEBUG)/src/spline3d/interpol3d.o \
$(OBJDIR_DEBUG)/src/spline3d/interpol.o \
$(OBJDIR_DEBUG)/src/metaballs/field.o $(OBJDIR_DEBUG)/src/metaballs/metaball.o \
$(OBJDIR_DEBUG)/src/camera_util.o $(OBJDIR_DEBUG)/src/camera.o \
$(OBJDIR_DEBUG)/src/base_scene.o $(OBJDIR_DEBUG)/src/scene01.o $(OBJDIR_DEBUG)/src/scene02.o \
$(OBJDIR_DEBUG)/src/text_layer.o $(OBJDIR_DEBUG)/src/wave_func.o $(OBJDIR_DEBUG)/src/texture_base.o \
$(OBJDIR_DEBUG)/src/texture2d.o $(OBJDIR_DEBUG)/src/texture_cube_map.o \
$(OBJDIR_DEBUG)/src/particle_emitter.o \
$(OBJDIR_DEBUG)/src/screen_plane.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/src/glad.o $(OBJDIR_RELEASE)/src/util.o $(OBJDIR_RELEASE)/src/main.o \
$(OBJDIR_RELEASE)/src/spline3d/splinegl.o $(OBJDIR_RELEASE)/src/spline3d/interpol_spline.o \
$(OBJDIR_RELEASE)/src/spline3d/interpol_non_fonct.o $(OBJDIR_RELEASE)/src/spline3d/interpol3d.o \
$(OBJDIR_RELEASE)/src/spline3d/interpol.o \
$(OBJDIR_RELEASE)/src/metaballs/field.o $(OBJDIR_RELEASE)/src/metaballs/metaball.o \
$(OBJDIR_RELEASE)/src/camera_util.o $(OBJDIR_RELEASE)/src/camera.o \
$(OBJDIR_RELEASE)/src/base_scene.o $(OBJDIR_RELEASE)/src/scene01.o $(OBJDIR_RELEASE)/src/scene02.o \
$(OBJDIR_RELEASE)/src/text_layer.o $(OBJDIR_RELEASE)/src/wave_func.o $(OBJDIR_RELEASE)/src/texture_base.o \
$(OBJDIR_RELEASE)/src/texture2d.o $(OBJDIR_RELEASE)/src/texture_cube_map.o \
$(OBJDIR_RELEASE)/src/particle_emitter.o \
$(OBJDIR_RELEASE)/src/screen_plane.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/src || mkdir -p $(OBJDIR_DEBUG)/src
	test -d $(OBJDIR_DEBUG)/src/spline3d || mkdir -p $(OBJDIR_DEBUG)/src/spline3d
	test -d $(OBJDIR_DEBUG)/src/metaballs || mkdir -p $(OBJDIR_DEBUG)/src/metaballs

after_debug: 

debug: before_debug out_debug after_debug

out_debug: before_debug $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) -o $(OUT_DEBUG) $(OBJ_DEBUG)  $(LDFLAGS_DEBUG) $(LIB_DEBUG)

$(OBJDIR_DEBUG)/src/glad.o: src/glad.c
	$(CC) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/glad.c -o $(OBJDIR_DEBUG)/src/glad.o
	
$(OBJDIR_DEBUG)/src/util.o: src/util.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/util.cpp -o $(OBJDIR_DEBUG)/src/util.o

$(OBJDIR_DEBUG)/src/spline3d/splinegl.o: src/spline3d/splinegl.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/spline3d/splinegl.cpp -o $(OBJDIR_DEBUG)/src/spline3d/splinegl.o

$(OBJDIR_DEBUG)/src/spline3d/interpol_spline.o: src/spline3d/interpol_spline.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/spline3d/interpol_spline.cpp -o $(OBJDIR_DEBUG)/src/spline3d/interpol_spline.o

$(OBJDIR_DEBUG)/src/spline3d/interpol_non_fonct.o: src/spline3d/interpol_non_fonct.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/spline3d/interpol_non_fonct.cpp -o $(OBJDIR_DEBUG)/src/spline3d/interpol_non_fonct.o

$(OBJDIR_DEBUG)/src/spline3d/interpol3d.o: src/spline3d/interpol3d.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/spline3d/interpol3d.cpp -o $(OBJDIR_DEBUG)/src/spline3d/interpol3d.o

$(OBJDIR_DEBUG)/src/spline3d/interpol.o: src/spline3d/interpol.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/spline3d/interpol.cpp -o $(OBJDIR_DEBUG)/src/spline3d/interpol.o

$(OBJDIR_DEBUG)/src/metaballs/field.o: src/metaballs/field.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/metaballs/field.cpp -o $(OBJDIR_DEBUG)/src/metaballs/field.o

$(OBJDIR_DEBUG)/src/metaballs/metaball.o: src/metaballs/metaball.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/metaballs/metaball.cpp -o $(OBJDIR_DEBUG)/src/metaballs/metaball.o

$(OBJDIR_DEBUG)/src/camera_util.o: src/camera_util.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/camera_util.cpp -o $(OBJDIR_DEBUG)/src/camera_util.o

$(OBJDIR_DEBUG)/src/camera.o: src/camera.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/camera.cpp -o $(OBJDIR_DEBUG)/src/camera.o

$(OBJDIR_DEBUG)/src/text_layer.o: src/text_layer.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/text_layer.cpp -o $(OBJDIR_DEBUG)/src/text_layer.o

$(OBJDIR_DEBUG)/src/wave_func.o: src/wave_func.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/wave_func.cpp -o $(OBJDIR_DEBUG)/src/wave_func.o

$(OBJDIR_DEBUG)/src/texture_base.o: src/texture_base.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/texture_base.cpp -o $(OBJDIR_DEBUG)/src/texture_base.o

$(OBJDIR_DEBUG)/src/texture2d.o: src/texture2d.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/texture2d.cpp -o $(OBJDIR_DEBUG)/src/texture2d.o

$(OBJDIR_DEBUG)/src/texture_cube_map.o: src/texture_cube_map.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/texture_cube_map.cpp -o $(OBJDIR_DEBUG)/src/texture_cube_map.o

$(OBJDIR_DEBUG)/src/main.o: src/main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/main.cpp -o $(OBJDIR_DEBUG)/src/main.o

$(OBJDIR_DEBUG)/src/base_scene.o: src/base_scene.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/base_scene.cpp -o $(OBJDIR_DEBUG)/src/base_scene.o

$(OBJDIR_DEBUG)/src/particle_emitter.o: src/particle_emitter.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/particle_emitter.cpp -o $(OBJDIR_DEBUG)/src/particle_emitter.o

$(OBJDIR_DEBUG)/src/screen_plane.o: src/screen_plane.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/screen_plane.cpp -o $(OBJDIR_DEBUG)/src/screen_plane.o

$(OBJDIR_DEBUG)/src/scene01.o: src/scene01.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/scene01.cpp -o $(OBJDIR_DEBUG)/src/scene01.o

$(OBJDIR_DEBUG)/src/scene02.o: src/scene02.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c src/scene02.cpp -o $(OBJDIR_DEBUG)/src/scene02.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/src
	rm -rf $(OBJDIR_DEBUG)/src/spline3d
	rm -rf $(OBJDIR_DEBUG)/src/metaballs

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE)/src || mkdir -p $(OBJDIR_RELEASE)/src
	test -d $(OBJDIR_RELEASE)/src/spline3d || mkdir -p $(OBJDIR_RELEASE)/src/spline3d
	test -d $(OBJDIR_RELEASE)/src/metaballs || mkdir -p $(OBJDIR_RELEASE)/src/metaballs

after_release: 

release: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/src/glad.o: src/glad.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/glad.c -o $(OBJDIR_RELEASE)/src/glad.o

$(OBJDIR_RELEASE)/src/util.o: src/util.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/util.cpp -o $(OBJDIR_RELEASE)/src/util.o

$(OBJDIR_RELEASE)/src/spline3d/splinegl.o: src/spline3d/splinegl.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/spline3d/splinegl.cpp -o $(OBJDIR_RELEASE)/src/spline3d/splinegl.o

$(OBJDIR_RELEASE)/src/spline3d/interpol_spline.o: src/spline3d/interpol_spline.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/spline3d/interpol_spline.cpp -o $(OBJDIR_RELEASE)/src/spline3d/interpol_spline.o

$(OBJDIR_RELEASE)/src/spline3d/interpol_non_fonct.o: src/spline3d/interpol_non_fonct.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/spline3d/interpol_non_fonct.cpp -o $(OBJDIR_RELEASE)/src/spline3d/interpol_non_fonct.o

$(OBJDIR_RELEASE)/src/spline3d/interpol3d.o: src/spline3d/interpol3d.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/spline3d/interpol3d.cpp -o $(OBJDIR_RELEASE)/src/spline3d/interpol3d.o

$(OBJDIR_RELEASE)/src/spline3d/interpol.o: src/spline3d/interpol.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/spline3d/interpol.cpp -o $(OBJDIR_RELEASE)/src/spline3d/interpol.o

$(OBJDIR_RELEASE)/src/metaballs/field.o: src/metaballs/field.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/metaballs/field.cpp -o $(OBJDIR_RELEASE)/src/metaballs/field.o
	
$(OBJDIR_RELEASE)/src/metaballs/metaball.o: src/metaballs/metaball.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/metaballs/metaball.cpp -o $(OBJDIR_RELEASE)/src/metaballs/metaball.o

$(OBJDIR_RELEASE)/src/camera_util.o: src/camera_util.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/camera_util.cpp -o $(OBJDIR_RELEASE)/src/camera_util.o

$(OBJDIR_RELEASE)/src/camera.o: src/camera.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/camera.cpp -o $(OBJDIR_RELEASE)/src/camera.o

$(OBJDIR_RELEASE)/src/text_layer.o: src/text_layer.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/text_layer.cpp -o $(OBJDIR_RELEASE)/src/text_layer.o

$(OBJDIR_RELEASE)/src/wave_func.o: src/wave_func.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/wave_func.cpp -o $(OBJDIR_RELEASE)/src/wave_func.o

$(OBJDIR_RELEASE)/src/texture_base.o: src/texture_base.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/texture_base.cpp -o $(OBJDIR_RELEASE)/src/texture_base.o

$(OBJDIR_RELEASE)/src/texture2d.o: src/texture2d.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/texture2d.cpp -o $(OBJDIR_RELEASE)/src/texture2d.o

$(OBJDIR_RELEASE)/src/texture_cube_map.o: src/texture_cube_map.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/texture_cube_map.cpp -o $(OBJDIR_RELEASE)/src/texture_cube_map.o

$(OBJDIR_RELEASE)/src/main.o: src/main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/main.cpp -o $(OBJDIR_RELEASE)/src/main.o

$(OBJDIR_RELEASE)/src/base_scene.o: src/base_scene.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/base_scene.cpp -o $(OBJDIR_RELEASE)/src/base_scene.o

$(OBJDIR_RELEASE)/src/particle_emitter.o: src/particle_emitter.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/particle_emitter.cpp -o $(OBJDIR_RELEASE)/src/particle_emitter.o

$(OBJDIR_RELEASE)/src/screen_plane.o: src/screen_plane.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/screen_plane.cpp -o $(OBJDIR_RELEASE)/src/screen_plane.o

$(OBJDIR_RELEASE)/src/scene01.o: src/scene01.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/scene01.cpp -o $(OBJDIR_RELEASE)/src/scene01.o

$(OBJDIR_RELEASE)/src/scene02.o: src/scene02.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c src/scene02.cpp -o $(OBJDIR_RELEASE)/src/scene02.o

clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)/src
	rm -rf $(OBJDIR_RELEASE)/src/spline3d
	rm -rf $(OBJDIR_RELEASE)/src/metaballs

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

