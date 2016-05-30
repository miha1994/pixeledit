#include "picture_wind.h"
#include "read_write.h"
#include "core.h"
#include "glob.h"
#include "fadma.h"
#include "lodepng.h"
#include "input.h"
#include "hot_vars.h"
#include <cmath>

float b_time = 0;
int dt1000 = 0, count_ = 0;
CLR g_8_colors[8] = {
	CLR::Black,
	CLR::White,
	CLR::Red,
	CLR::Green,
	CLR::Blue,
	CLR::Yellow,
	CLR::Magenta,
	CLR::Cyan
};
Rect <int> g_sq_r (v2i (10,10), v2i (220, 220));

#define D_CUR	(m_layers[m_cur_layer])
#define D_ACT	g_sq_r << in.mouse.pos

rgba_array lrs[20];

commit& commit::operator = (const commit& r) {
	m_layers = r.m_layers;
	m_pos = r.m_pos;
	m_scale = r.m_scale;
	m_cur_layer = r.m_cur_layer;
	m_mask = r.m_mask;
	m_draw_clipboard = r.m_draw_clipboard;
	return (*this);
}

void picture_wind::render (State state) {
	FOR_ARRAY_2D (v, m_main_sq) {
		m_main_sq[v] = m_bg_is_dark ? (((v.x/3 + v.y/3)%2) ? CLR (40,40,40) : CLR (20,20,20)) : (((v.x/3 + v.y/3)%2) ? CLR (240,240,240) : CLR (200,200,200));
	}
	
	if (m_layers_miniatures.size() != m_layers.size ()) {
		if (m_layers_miniatures.size() < m_layers.size ()) {
			m_layers_miniatures.push_back (rgba_array ());
			m_layers_miniatures.back ().init (220,220);
			m_layers_miniatures.back ().alpha_matters = true;
		} else {
			m_layers_miniatures.pop_back ();
		}
	}
	int count = 0;
	forstl_p (p, m_layers) {
		m_main_sq.draw (&p, m_pos, m_scale);
		if (m_state == PWS_U && count == m_cur_layer) {
			m_main_sq.draw (&m_u_mask, m_pos, m_scale);
		}
		m_layers_miniatures[count].clear (CLR (0,0,0,0));
		m_layers_miniatures[count].draw (&p, m_pos, m_scale);

		++count;
	}
	if (m_draw_clipboard) {
		m_main_sq.draw (&m_clipboard, m_pos + m_clipboard_pos * m_scale, m_scale);
	}
	
	if (m_i_color_show) {
		m_main_sq.draw (&m_i_color, m_i_color_pos);
	}
	
	button_state &left = in.mouse.mbutton[MOUSE_LEFT],
		&right = in.mouse.mbutton[MOUSE_RIGHT];

	D_ADD_SPRITE (m_bg, v2i (0,0));
	D_ADD_SPRITE (m_main_sq, v2i (10,10));
	D_ADD_SPRITE (m_active_colors[0], v2i(10, 0));
	D_ADD_SPRITE (m_active_colors[1], v2i(19, 0));
	D_ADD_SPRITE (m_color_panel, v2i (240,0));
	D_ADD_SPRITE (m_mask_bounds, v2i (0,0));
	D_ADD_SPRITE (m_bounds, v2i (0,0));
	count = 0;
	forstl_p (p, m_layers_miniatures) {
		lrs[count].clear (count == m_cur_layer ? CLR::Red : CLR::Black);
		int r, g, b, a;
		FOR_2D (v, 22, 22) {
			v2i start = v * 10;
			r = g = b = a = 0;
			FOR_2D (w, 10, 10) {
				r += p[start + w].r;
				g += p[start + w].g;
				b += p[start + w].b;
				a += p[start + w].a;
			}
			lrs[count][v+ v2i (1,1)] = CLR (r/ 100.0, g/ 100.0, b / 100.0, a / 100.0);
		}
		D_ADD_SPRITE (lrs[count], v2i (296, 67 + count * 24));
		if (left.just_pressed && Rect <int> (v2i (296, 67 + count * 24), v2i (24,24)) << in.mouse.pos) {
			m_cur_layer = count;
		}
		++count;
	}
}

void picture_wind::update (State state, float dt) {
	sf::sleep (sf::milliseconds (10));
	b_time += dt + 0.015;
	++count_;
	bool dont_enter_pause = false;
	if (m_state == PWS_PAUSE) {
		if (in.kb.ctrl.pressed_now && in.kb['P'].just_pressed) {
			m_state = PWS_B;
			dont_enter_pause = true;
		} else {
			return;
		}
	}
	if (in.kb.escape.pressed_now) {
		exit (count_ / b_time);
	}

	button_state &left = in.mouse.mbutton[MOUSE_LEFT],
		&right = in.mouse.mbutton[MOUSE_RIGHT];

	v2i pos = (in.mouse.pos - v2i (10,10) - m_pos) / m_scale;
	///////////////////////////////////////////  INPUT  //////////////////////////  HANDLING  ///////////////////////////////////////
	if (!in.kb.ctrl.pressed_now) {
		if (in.kb['B'].just_pressed) {
			m_state = PWS_B;}
		if (in.kb['I'].just_pressed) {
			m_state = PWS_I;}
		if (in.kb['Z'].just_pressed) {
			m_state = PWS_Z;}
		if (in.kb['E'].just_pressed) {
			m_state = PWS_E;}
		if (in.kb['M'].just_pressed) {
			m_state = PWS_M;}
		if (in.kb['V'].just_pressed) {
			m_state = PWS_V;}
		if (in.kb['W'].just_pressed) {
			m_state = PWS_W;}
		if (in.kb['U'].just_pressed) {
			m_state = PWS_U;}
		if (in.kb['C'].just_pressed) {
			m_pos = v2i(0,0); make_commit ();}
		if (in.kb['H'].just_pressed) {
			m_bg_is_dark = !m_bg_is_dark;}
	} else {
		if (in.kb['Z'].just_pressed) {
			checkout ();
		}
		if (in.kb['Y'].just_pressed && m_cur_history < m_history.size () - 1) {
			m_cur_history += 2;
			checkout ();
		}
		if (in.kb['A'].just_pressed || in.kb['D'].just_pressed) {
			FOR_ARRAY_2D (v, m_mask) {
				m_mask[v] = in.kb['A'].just_pressed;
			}
			make_commit ();
		}
		if (in.kb['C'].just_pressed) {
			v2i v1 (320, 240), v2 (0,0);
			FOR_ARRAY_2D (v, m_mask) {
				if (m_mask[v] && D_CUR[v].a) {
					if (v1.x > v.x) {
						v1.x = v.x;
					}
					if (v1.y > v.y) {
						v1.y = v.y;
					}
					if (v2.x < v.x) {
						v2.x = v.x;
					}
					if (v2.y < v.y) {
						v2.y = v.y;
					}
				}
			}
			if (v1.x <= v2.x) {
				v2i sz = v2-v1 + v2i(1,1);
				m_clipboard.init (sz.x, sz.y);
				FOR_2D (v, sz.x, sz.y) {
					if (m_mask[v1 + v]) {
						m_clipboard[v] = D_CUR[v1 + v];
					} else {
						m_clipboard[v].a = 0;
					}
				}
			}
		}
		if (in.kb['X'].just_pressed) {
			v2i v1 (320, 240), v2 (0,0);
			FOR_ARRAY_2D (v, m_mask) {
				if (m_mask[v] && D_CUR[v].a) {
					if (v1.x > v.x) {
						v1.x = v.x;
					}
					if (v1.y > v.y) {
						v1.y = v.y;
					}
					if (v2.x < v.x) {
						v2.x = v.x;
					}
					if (v2.y < v.y) {
						v2.y = v.y;
					}
				}
			}
			if (v1.x <= v2.x) {
				v2i sz = v2-v1 + v2i(1,1);
				m_clipboard.init (sz.x, sz.y);
				FOR_2D (v, sz.x, sz.y) {
					if (m_mask[v1 + v]) {
						m_clipboard[v] = D_CUR[v1 + v];
						D_CUR[v1 + v].a = 0;
					} else {
						m_clipboard[v].a = 0;
					}
				}
			}
		}
		if (in.kb['V'].just_pressed) {
			if (m_clipboard.m_initiated) {
				if (!in.kb.shift.pressed_now) {
					m_layers.push_back (rgba_array ());
					m_cur_layer = m_layers.size () - 1;
					D_CUR.alpha_matters = true;
					D_CUR.init (D_W, D_H);
					D_CUR.clear (CLR(0,0,0,0));
				}
				v2i st = m_pos == v2i (0,0) ? v2i (0,0) : (in.mouse.pos - v2i (10,10) - m_pos) / m_scale - m_clipboard.get_WH () / 2;
				//D_CUR.draw (&m_clipboard, st);
				m_draw_clipboard = true;
				m_clipboard_pos = st;
				m_mask.clear (false);
				FOR_ARRAY_2D (v, m_clipboard) {
					if (m_clipboard[v].a) {
						m_mask[st + v] = true;
					}
				}
			}
		}
		if (in.kb['S'].just_pressed) {
			vector <unsigned char> image;
			rgba_array rgb;
			rgb.init (D_W, D_H);
			rgb.alpha_matters = true;
			rgb.clear (CLR(0,0,0,0));
			forstl_p (p, m_layers) {
				rgb.draw (&p, v2i(0,0));
			}
			image.assign (D_W * D_H * 4, 0);
			memcpy (&image[0], rgb, D_W * D_H * 4);
			auto err = lodepng::encode (Texture_name ("out"), image, D_W, D_H);
		}
		if (in.kb['M'].just_pressed) {
			if (m_cur_layer < m_layers.size () - 1) {
				D_CUR.draw (&m_layers[m_cur_layer+1], v2i(0,0));
				for (int i = m_cur_layer + 1; i < m_layers.size ()-1; ++i) {
					m_layers[i] = m_layers[i+1];
				}
				m_layers.pop_back ();
				make_commit ();
			}
		}
		if (in.kb['P'].just_pressed && !dont_enter_pause) {
			m_state = PWS_PAUSE;
		}
	}
	if (in.kb.delete_.just_pressed) {
		FOR_ARRAY_2D (v, m_mask) {
			if (m_mask[v]) {
				D_CUR[v].a = 0;
			}
		}
		make_commit ();
	}
	static v2i shift_pos (0,0);
	{
		button_state &s1 = in.kb.shift, &s2 = in.mouse.mbutton[MOUSE_LEFT], &s3 = in.mouse.mbutton[MOUSE_RIGHT];
		if ((s1.just_pressed && (s2.pressed_now || s3.pressed_now)) ||
			(s1.pressed_now && (s2.just_pressed || s3.just_pressed)) ||
			(s1.just_pressed && (s2.just_pressed || s3.just_pressed))) {
				shift_pos = pos;
		}
		static v2i abs_dif (0,0);
		if (s1.pressed_now) {
			if (abs_dif == v2i (0,0)) {
				abs_dif = pos - shift_pos;
				abs_dif.x = abs (abs_dif.x);
				abs_dif.y = abs (abs_dif.y);
			}
			if (abs_dif.x > abs_dif.y) {
				pos.y = shift_pos.y;
			} else {
				pos.x = shift_pos.x;
			}
		} else {
			abs_dif = v2i (0,0);
		}
	}
	////////////////////////  END  ////////  OF  //////////////  INPUT  /////////////////////////  HANDLING  //////////////////////

	m_i_color_show = false;
	if (in.kb.space.just_pressed) {
		m_space_pos = in.mouse.pos;
		m_space_pict_pos = m_pos;
	}
	if (in.kb.space.just_released) {
		make_commit ();
	}
	if (in.kb.space.pressed_now) {
		m_pos = m_space_pict_pos + in.mouse.pos - m_space_pos;
	}
	if ((in.mouse.mbutton[MOUSE_LEFT].just_released || in.mouse.mbutton[MOUSE_RIGHT].just_released) && m_commit_need) {
		make_commit ();
		m_commit_need = false;
	}
	bool mask = false;
	FOR_ARRAY_2D (v, m_mask) {
		if (m_mask[v]) {
			mask = true;
		}
	}
	///////////////////////// SWITCH /////////////////////////////////////////////////////////////////////////////////////////////////////
	switch (m_state) {
	case PWS_B:
		if (D_CUR << pos && D_ACT) {
			if (in.mouse.mbutton[MOUSE_LEFT].pressed_now || in.mouse.mbutton[MOUSE_RIGHT].pressed_now) {
				v2i shift = v2i ((m_brush_size)/2, (m_brush_size)/2);
				FOR_2D (v, m_brush_size, m_brush_size) {
					if (m_draw_clipboard) {
						v2i tm = pos + v - shift - m_clipboard_pos;
						if (m_clipboard << tm) m_clipboard[tm] = in.mouse.mbutton[MOUSE_LEFT].pressed_now ? m_color1 : m_color2;
					} else {
						v2i tm = pos + v - shift;
						if (D_CUR << tm && (!mask || m_mask[tm])) D_CUR[tm] = in.mouse.mbutton[MOUSE_LEFT].pressed_now ? m_color1 : m_color2;
					}
				}
				m_commit_need = true;
			}
		}
		if (in.kb.dirs[D_UP].just_pressed || in.kb.dirs[D_DOWN].just_pressed) {
			m_brush_size += (in.kb.ctrl.pressed_now ? 5 : 1) * (in.kb.dirs[D_UP].just_pressed ? 1 : -1);
			m_brush_size = Max (m_brush_size, 1);
		}
		if (in.kb.dirs[D_RIGHT].just_pressed || in.kb.dirs[D_LEFT].just_pressed) {
			int a = m_color1.a;
			a += (in.kb.ctrl.pressed_now ? 5 : 1) * (in.kb.dirs[D_RIGHT].just_pressed ? 10 : -10);
			m_color2.a = m_color1.a = Min (255, Max (0, a));
		}
		break;
	case PWS_E:
		if (D_CUR << pos && D_ACT) {
			if (in.mouse.mbutton[MOUSE_LEFT].pressed_now) {
				v2i shift = v2i ((m_erase_size)/2, (m_erase_size)/2);
				FOR_2D (v, m_erase_size, m_erase_size) {
					if (m_draw_clipboard) {
						v2i tm = pos + v - shift - m_clipboard_pos;
						if (m_clipboard << tm) m_clipboard[tm].a = 0;
					} else {
						v2i tm = pos + v - shift;
						if (D_CUR << tm && (!mask || m_mask[tm])) D_CUR[tm].a = 0;
					}
				}
				m_commit_need = true;
			}
		}
		if (in.kb.dirs[D_UP].just_pressed || in.kb.dirs[D_DOWN].just_pressed) {
			m_erase_size += (in.kb.ctrl.pressed_now ? 5 : 1) * (in.kb.dirs[D_UP].just_pressed ? 1 : -1);
			m_erase_size = Max (m_erase_size, 1);
		}
		break;
	case PWS_I:
		if (D_CUR << pos && D_ACT) {
			CLR rc = D_CUR[pos];
			rc.a = 255;
			if (in.mouse.mbutton[MOUSE_LEFT].pressed_now) {
				m_color1 = rc;
			}
			if (in.mouse.mbutton[MOUSE_RIGHT].pressed_now) {
				m_color2 = rc;
			}
			m_i_color_show = true;
			FOR_ARRAY_2D (v, m_i_color) {
				int w = m_i_color.get_H (), w4 = w/4;
				if (v.x < w4 || v.x >= (w - w4) || v.y < w4 || v.y >= (w - w4)) {
					m_i_color[v] = rc;
				} else {
					m_i_color[v].a = 0;
				}
			}
			m_i_color_pos = in.mouse.pos - v2i(10,10) - v2i (m_i_color.get_H ()/2, m_i_color.get_H ()/2);
		}
		break;
	case PWS_Z:
		if (D_CUR << pos && D_ACT) {
			v2i ps = in.mouse.pos - v2i (10,10);
			if (in.mouse.mbutton[MOUSE_LEFT].just_pressed) {
				int old = m_scale;
				m_scale *= 2;
				v2i vec = m_pos - ps;
				vec = vec * m_scale / old;
				m_pos = ps + vec;
				m_commit_need = true;
			}
			if (in.mouse.mbutton[MOUSE_RIGHT].just_pressed) {
				if (m_scale > 1) {
					int old = m_scale;
					m_scale /= 2;
					v2i vec = m_pos - ps;
					vec = vec * m_scale / old;
					m_pos = ps + vec;
					m_commit_need = true;
				}
			}
		}
		break;
	case PWS_M:
		if (D_CUR << pos && D_ACT) {
			static v2i start;
			if (in.mouse.mbutton[MOUSE_LEFT].just_pressed || in.mouse.mbutton[MOUSE_RIGHT].just_pressed) {
				start = pos;
			} else if (in.mouse.mbutton[MOUSE_LEFT].just_released || in.mouse.mbutton[MOUSE_RIGHT].just_released) {
				m_bounds.clear (CLR (0,0,0,0));
				v2i st = start, p = pos;
				if (st.x > p.x) {
					swap (st.x, p.x);
				}
				if (st.y > p.y) {
					swap (st.y, p.y);
				}
				p += v2i (1,1);
				FOR_2D (v, (p-st).x, (p-st).y) {
					m_mask[st + v] = in.mouse.mbutton[MOUSE_LEFT].just_released;
				}
				make_commit ();
			}
			CLR bc;
			v2i bv;
#define add bc = int((bv.x+b_time)/2 + (bv.y + b_time)/2) % 2 ? CLR (0,0,0,124) : CLR (255,255,255,124); if (m_bounds << bv) m_bounds[bv] = bc;
			if (in.mouse.mbutton[MOUSE_LEFT].pressed_now || in.mouse.mbutton[MOUSE_RIGHT].pressed_now) {
				m_bounds.clear (CLR (0,0,0,0));
				v2i st, p;
				st = v2i(9, 9) + m_pos + start * m_scale;
				p = v2i(9, 9) + m_pos + pos * m_scale;
				if (st.x > p.x) {
					swap (st.x, p.x);
				}
				if (st.y > p.y) {
					swap (st.y, p.y);
				}
				p += v2i(1+m_scale,1+m_scale);
				FOR (i, p.x - st.x) {
					bv = v2i (st.x + i, st.y);
					add;
					bv = v2i (st.x + i, p.y);
					add;
				}
				FOR (i, p.y - st.y) {
					bv = v2i (st.x, st.y + i);
					add;
					bv = v2i (p.x, st.y + i);
					add;
				}
			}
#undef add
		}
		break;
	case PWS_V:
		if (!m_draw_clipboard) {
			break;
		} else {
			static v2i start;
			static v2i cb_start;
			static bool end = false;
			if (in.mouse.mbutton[MOUSE_LEFT].just_pressed) {
				if (m_mask[pos]) {
					start = pos;
					cb_start = m_clipboard_pos;
					end = false;
				} else {
					D_CUR.draw (&m_clipboard, m_clipboard_pos);
					end = true;
					m_mask.clear (false);
					m_draw_clipboard = false;
					make_commit ();
				}
			}
			if (in.mouse.mbutton[MOUSE_LEFT].pressed_now && !end) {
				m_clipboard_pos = cb_start + pos - start;
				m_mask.clear (false);
				FOR_ARRAY_2D (v, m_clipboard) {
					if (m_clipboard[v].a) {
						m_mask[m_clipboard_pos + v] = true;
					}
				}
			}
		}
		break;
	case PWS_W:
		if (in.mouse.mbutton[MOUSE_LEFT].just_pressed || in.mouse.mbutton[MOUSE_RIGHT].just_pressed) {
			CLR sm = D_CUR[pos];

			if (in.kb.ctrl.pressed_now) {
				FOR_ARRAY_2D (v, D_CUR) {
					if (D_CUR[v] == sm) {
						m_mask[v] = left.just_pressed;
					}
				}
			} else {
				set <v2i> ta;   // built set of same color cells
				set <v2i> nw, next_nw;
				v2i tv;
				ta.insert (pos);
				nw.insert (pos);
				while (!nw.empty ()) {
					forstl_p (p, nw) {
						FOR (i, 4) {
							tv = p + g_n_to_dir[i];
							if (D_CUR << tv && D_CUR [tv] == sm) {
								int os = ta.size ();
								ta.insert (tv);
								if (ta.size () > os) {
									next_nw.insert (tv);
								}
							}
						}
					}
					nw = next_nw;
					next_nw.clear ();
				}

				forstl_p (p, ta) {
					m_mask[p] = left.just_pressed;
				}
			}
		}
		break;
	case PWS_U:
		Rect <int> r (v2i (240, 0), m_color_panel.get_WH ());
		if (r << in.mouse.pos) {
			v2i v = in.mouse.pos - r.pos;
			CLR c = g_8_colors[v.y/8];
			c.a = 255.0 * (v.x / 80.0);
			m_u_mask.clear (c);
			FOR_ARRAY_2D (v, m_mask) {
				m_u_mask[v].a *= m_mask[v] * D_CUR[v].a / 255.0;
			}
			if (right.just_pressed) {
				D_CUR.draw (&m_u_mask, v2i (0,0));
				make_commit ();
			}
		} else {
			m_u_mask.clear (CLR(0,0,0,0));
		}
		break;
	}
	///////////////////////////////////////// END /////////////////////////// OF //////////////////////// SWITCH ////////////////////////////////////
	if (in.mouse.mbutton[MOUSE_LEFT].just_pressed) {
		Rect <int> r (v2i (240, 0), m_color_panel.get_WH ());
		if (r << in.mouse.pos) {
			v2i v = in.mouse.pos - r.pos;
			CLR &c = v.x > 39 ? m_color2 : m_color1;
			c = m_color_panel[v];
		}
	}

	if (m_old_col1 != m_color1 || m_old_col2 != m_color2) {
		FOR_2D (v, 8,8) {
			m_active_colors[0][v + v2i(1,1)] = m_color1;
			m_active_colors[1][v + v2i(1,1)] = m_color2;
		}
		if (!in.kb.ctrl.pressed_now) {
			CLR nc[2];
			float s, d;
			FOR (i, 8) {
				FOR (z, 38) {
					d = z / 38.0f;
					s = 1 - d;
					nc[0] = m_color1 * s ^ g_8_colors[i] * d;
					nc[1] = m_color2 * s ^ g_8_colors[i] * d;
					FOR (x, 7) {
						m_color_panel[v2i (1+z,i*8 + 1 + x)] = nc[0];
						m_color_panel[v2i (40+z,i*8 + 1 + x)] = nc[1];
					}
				}
			}
		}
	}
#define add  if (m_mask_bounds << bv) {bc = int((bv.x+b_time)/2 + (bv.y + b_time)/2) % 2 ? CLR (0,0,0,194) : CLR (255,255,255,194); \
	m_mask_bounds[bv] = bc; }
	{
		m_mask_bounds.clear (CLR(0,0,0,0));
		v2i r;
		CLR bc;
		v2i bv;
		FOR_ARRAY_2D (v, m_mask) {
			if (m_mask[v]) {
				r = v + g_n_to_dir[D_UP];
				if (m_mask << r && !m_mask[r] || !(m_mask << r)) {
					v2i mv = v2i(10, 9) + m_pos + v * m_scale;
					FOR (i, m_scale) {
						bv = v2i(mv.x+i,mv.y);
						add;
					}
				}
				r = v + g_n_to_dir[D_LEFT];
				if (m_mask << r && !m_mask[r] || !(m_mask << r)) {
					v2i mv = v2i(9, 10) + m_pos + v * m_scale;
					FOR (i, m_scale) {
						bv = v2i(mv.x,mv.y+i);
						add;
					}
				}
				r = v + g_n_to_dir[D_DOWN];
				if (m_mask << r && !m_mask[r] || !(m_mask << r)) {
					v2i mv = v2i(10, 10 + m_scale) + m_pos + v * m_scale;
					FOR (i, m_scale) {
						bv = v2i(mv.x+i,mv.y);
						add;
					}
				}
				r = v + g_n_to_dir[D_RIGHT];
				if (m_mask << r && !m_mask[r] || !(m_mask << r)) {
					v2i mv = v2i(10 + m_scale, 10) + m_pos + v * m_scale;
					FOR (i, m_scale) {
						bv = v2i(mv.x,mv.y+i);
						add;
					}
				}
			}
		}
	}
#undef add

	m_old_col1 = m_color1, m_old_col2 = m_color2;
}

void picture_wind::clean () {
	//PUT YOUR CODE HERE
}

void picture_wind::load () {
	m_layers.push_back (rgba_array ());
	m_layers[0].alpha_matters = true;
	m_cur_layer = 0;
	D_CUR.init (D_W, D_H);
	m_bg.init (D_W, D_H);
	m_main_sq.init (220, 220);
	m_i_color.init (32, 32);
	m_i_color.alpha_matters = true;

	FOR_ARRAY_2D (v, m_bg) {
		m_bg[v] = ((v.x/3 + v.y/3)%2) ? CLR (50,50,50) : CLR (80,80,80);
	}
	FOR (i, 2) {
		int h = 10;
		m_active_colors[i].init (h,h);
		m_active_colors[i].clear (CLR::Black);
	}
	m_color_panel.init (80, 65);
	m_color_panel.clear (CLR::Black);

	m_bounds.init (D_W, D_H);
	m_bounds.alpha_matters = true;
	m_bounds.clear (CLR(0,0,0,0));
	m_mask_bounds.init (D_W, D_H);
	m_mask_bounds.alpha_matters = true;
	m_mask_bounds.clear (CLR(0,0,0,0));
	m_mask.init (D_W, D_H);
	m_clipboard.alpha_matters = true;
	FOR_ARRAY_2D (v, m_mask) {
		m_mask [v] = false;
	}
	FOR (i, 20) {
		lrs[i].init (24, 24);
		lrs[i].alpha_matters = true;
		lrs[i].clear (CLR::Black);
	}
	m_u_mask.init (D_W, D_H);
	m_u_mask.alpha_matters = true;

	m_scale = 1;
	m_state = PWS_B;
	m_color1 = CLR::Black;
	m_color2 = CLR::White;
	m_pos = v2i (0,0);
	m_bg_is_dark = false;
	m_draw_clipboard = false;
	m_brush_size = 1;
	m_erase_size = 1;

	vector <unsigned char> cpy;
	unsigned int w, h;
	auto err = lodepng::decode (cpy, w, h, Texture_name (S_[0]));
	if(err) std::cout << "decoder error " << err << ": " << lodepng_error_text(err) << std::endl;
	memcpy (D_CUR, &cpy[0], w*h*4);
	//D_CUR.clear (CLR(0,0,0,0));
	read_string (); // "}"

	m_history.push_back (commit (m_layers, m_pos, m_scale, m_cur_layer, m_mask, m_draw_clipboard));
	m_cur_history = 0;
}

void picture_wind::make_commit () {
	commit nw (m_layers, m_pos, m_scale, m_cur_layer, m_mask, m_draw_clipboard);
	++m_cur_history;
	if (m_cur_history == m_history.size ()) {
		m_history.push_back (nw);
	} else {
		m_history[m_cur_history] = nw;
	}
}

void picture_wind::checkout () {
	--m_cur_history;
	if (m_cur_history >= 0){
		m_layers = m_history[m_cur_history].m_layers;
		m_pos = m_history[m_cur_history].m_pos;
		m_scale = m_history[m_cur_history].m_scale;
		m_cur_layer = m_history[m_cur_history].m_cur_layer;
		m_mask = m_history[m_cur_history].m_mask;
		m_draw_clipboard = m_history[m_cur_history].m_draw_clipboard;
	} else {
		m_cur_history = 0;
	}
}