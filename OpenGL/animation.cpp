#include "animation.h"
#include "core.h"
#include "input.h"
#include "picture_wind.h"

void animation::update (float dt) {
	m_time_to_next_frame -= dt;
	picture_wind *par = (picture_wind *)m_parent;
	v2i pos = (in.mouse.pos - v2i (10,10) - par->m_pos) / par->m_scale;
	Rect <int> r (v2i (240, 70), v2i (11, 11));
	if (r << in.mouse.pos && in.mouse.mbutton [MOUSE_LEFT].just_pressed) {
		m_prev_layer_half_transparent = par->m_layers.back ();
		FOR_ARRAY_2D (v, m_prev_layer_half_transparent) {
			m_prev_layer_half_transparent[v].a /= 2;
		}

		par->m_layers.push_back (rgba_array (true));
		par->m_layers.back ().init (D_W, D_H);
		par->m_layers.back ().clear (CLR (0,0,0,0));
		par->m_cur_layer = par->m_layers.size () - 1;
	}
}

void animation::render () {
	D_ADD_SPRITE (m_new_layer_button, v2i (240, 70));

	picture_wind *par = (picture_wind *)m_parent;

	v2i maximum (0,0);

	forstl_p (p, par->m_layers) {
		FOR_ARRAY_2D (v, p) {
			if (p[v].a) {
				if (v.x > maximum.x) {
					maximum.x = v.x;
				}
				if (v.y > maximum.y) {
					maximum.y = v.y;
				}
			}
		}
	}
	maximum += v2i (30,30);
	static set <int> not_rendered_frames;
	FOR (i, 10) {
		if (in.kb['0' + i].just_pressed) {
			if (not_rendered_frames.find (i) == not_rendered_frames.end ()) {
				not_rendered_frames.insert (i);
			} else {
				not_rendered_frames.erase (i);
			}
		}
	}
	if (m_time_to_next_frame < 0) {
		m_time_to_next_frame += 1.0 / m_fps;
		++m_current_frame;
		if (m_current_frame == par->m_layers.size ()) {
			m_current_frame = 0;
		}
		while (not_rendered_frames.find (m_current_frame) != not_rendered_frames.end ()) {
			++m_current_frame;
			if (m_current_frame == par->m_layers.size ()) {
				m_current_frame = 0;
			}
		}
	}
	D_ADD_SPRITE (par->m_layers[m_current_frame], v2i (D_W - maximum.x, D_H - maximum.y));
}

void animation::clear () {
	//PUT YOUR CODE HERE
}

void animation::init (void *pw) {
	m_parent = pw;

	m_current_frame = 0;
	m_fps = 8;
	m_time_to_next_frame = 0;

	m_new_layer_button.init (11,11);
	m_new_layer_button.clear (CLR (100, 230, 100, 255));
	FOR (i, 11) {
		m_new_layer_button[v2i (i,10)] = CLR::Black;
		m_new_layer_button[v2i (i,0)] = CLR::Black;
		m_new_layer_button[v2i (0,i)] = CLR::Black;
		m_new_layer_button[v2i (10,i)] = CLR::Black;
	}
#define tmp(a,b)	m_new_layer_button[v2i (a,b)] = CLR::Black;
#define tmp1(a,b)	loc += v2i (a,b); tmp (loc.x,loc.y);
	v2i loc (3, 8);
	For (5) {tmp1 (0, -1);}
	For (4) {tmp1 (1, 1);}
	For (4) {tmp1 (0, -1);}
#undef tmp
#undef tmp1
}
