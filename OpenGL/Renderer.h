#pragma once
#include "math_3d.h"
#include "fadma.h"
#include <vector>
#include <list>
#include <map>
#include "array_2d.h"

#define D_W		320
#define D_H		240
#define D_W_norm	(2.0/D_W)
#define D_H_norm	(2.0/D_H)
#define D_X_INIT	100
#define D_Y_INIT	30
#define D_SCALE		4
#define D_ADD_SPRITE(_array, _pos)	do {ren.sprite.rgba_ar = &_array; \
									ren.sprite.pos = _pos; \
									ren.add_sprite_to_render_list (); } while (0)
#define D_ADD_SPRITE_SCALE(_array, _pos, _scale)	\
									do {ren.sprite.rgba_ar = &_array; \
									ren.sprite.pos = _pos; \
									ren.sprite.scale = _scale; \
									ren.add_sprite_to_render_list (); \
									ren.sprite.scale = 1; } while (0)
#define ren	core.m_renderer

extern bool g_fullscreen;
extern GLfloat colors[D_W*D_H];
extern GLuint colors_location;

struct Renderer {
	GLuint m_VBO;
	GLuint m_IBO;
	GLuint m_textureObj;
	rgba_array m_tex_array;
	struct sprite_info {
		rgba_array *rgba_ar;
		v2i pos;
		int scale;
	};
	list <sprite_info> m_render_list;
	sprite_info sprite;
	Vertex_tex m_screen[4];
	void draw_everything ();
	void init ();
	void add_sprite_to_render_list ();
};