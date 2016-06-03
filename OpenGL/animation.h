#pragma once
#include "object.h"
#include "math_3d.h"
#include "Renderer.h"
#include "glob.h"
#include "array_2d.h"

struct animation {
	void *m_parent;
	rgba_array m_prev_layer_half_transparent;
	rgba_array m_new_layer_button;
	float m_fps;
	int m_current_frame;
	float m_time_to_next_frame;
	void init (void *pw);
	void update (float dt);  
	void render ();  
	void clear ();
};