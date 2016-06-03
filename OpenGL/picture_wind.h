#pragma once
#include "object.h"
#include "math_3d.h"
#include "Renderer.h"
#include "glob.h"
#include "array_2d.h"
#include "animation.h"

// ��������� ������ �������
enum PICTURE_WIND_STATES {
	PWS_B,	// �����
	PWS_Z,	// zoom, unzoom
	PWS_I,	// ���������� ����� ��� ����� � ��������
	PWS_E,	// ������ (������)
	PWS_M,	// ��������� (�������������)
	PWS_V,	// ����������� ������������ �� clipboard ������ ��������, ��������
	PWS_W,	// ��������� �������� ������ �����
	PWS_U,	// ��������� ����� � ���������� �������
	PWS_A,
	PWS_PAUSE	// ����� (����� ��� ����, ����� ������ �� ���������� �� ������� ������ ���� �� ������� ���� � ������� ������� ��������)
};

// �����. ������ ��������� / ��������� ������� � �����-�� ������ �������. ����� ��� Ctrl + Z. �� �������� ������������ �������
struct commit {
	vector <rgba_array> m_layers;	// ����
	v2i m_pos;						// �������� / �������
	int m_scale;					// �������
	int m_cur_layer;				// ����� ��������� ����
	bool m_draw_clipboard;			// �������� �� clipboard?
	bool m_animation_mode;
	array_2d <bool> m_mask;			// ����� ���������
	commit& operator = (const commit& r);	// �������� ������������
	commit () {}					// ������ �����������
	commit (vector <rgba_array >_m_layers, v2i _m_pos, int _m_scale, int _m_cur_layer, array_2d <bool> _m_mask, bool _m_draw_clipboard, bool _m_animation_mode) {
		m_layers = _m_layers;
		m_pos = _m_pos;
		m_scale = _m_scale;
		m_cur_layer = _m_cur_layer;
		m_mask = _m_mask;
		m_draw_clipboard = _m_draw_clipboard;
		m_animation_mode = _m_animation_mode;
	}
};

// ��� ������� � ������� ������
struct picture_wind : Object {
	vector <rgba_array> m_layers;		// ����
	vector <rgba_array> m_layers_miniatures;	// ��������� �����, ��������� ������
	vector <commit> m_history;			// ������� ���������
	rgba_array m_main_sq;				// �������� ������� (� ���� �������� ��� ����. ������� - ��� ���� �����. �� ����� ��������� ������ �� � �� ��� ���� ���������. ��, ��� �� ������ � ���� ��� �� �����)
	rgba_array m_bg;		// ���
	rgba_array m_i_color;	// ��������� ������ �������, ������������ �������� ���� ������� �� ������� � ������ ������ ��������� ������. ������������ ������ �� ����� ��������� PWS_I
	rgba_array m_active_colors[2];	// ����������, ������������ ������� ����� �����
	rgba_array m_color_panel;	// ������ ������. ������� �� ������-������ �� ������� ����� �������� ������� ����� �����
	rgba_array m_bounds;		// ������ ����������� ���������, ������� ���������� ������� �������, ������� �� ������ ������ �������� ������������ � ������� ����������� ������������� ��������� (M)
	rgba_array m_mask_bounds;	// ������ ����������� ���������, ������� ���������� ������� ���������� ������� �������
	rgba_array m_clipboard;		// ����� ������. �� ���� ������ ����� �����������, ������� ����� ����� ��������. �������� ���� ��� ����� ��������� Ctrl + C
	rgba_array m_u_mask;		// ����������, ������ ����������� ��� ��������� ����� ���������� ������� (���������� U)
	array_2d <bool> m_mask;		// ����� ���������� ������� (�������, ������� ������ � ���������� ������� �������� ���������)
	animation m_animation;
	v2i m_pos;				// �������� / ������� ������� ������������ ��������� ��������
	int m_scale;			// �������
	int m_cur_layer;		// �������� ����
	int m_cur_history;		// ������� commit �������
	PICTURE_WIND_STATES m_state;	// ���������
	CLR m_color1, m_old_col1;		// �������� ���� ������ ����������, ������� ������ ��� �� ��������, �� ������� ���� ���� ����� �����
	CLR m_color2, m_old_col2;		// �������������� ���� --//--
	v2i m_i_color_pos;				// �������������� �������� m_i_color
	v2i m_space_pos;				// m_space_pos � m_space_pict_pos ������������ ��� ����������� �������� ��� ������� space
	v2i m_space_pict_pos;
	bool m_commit_need;				// ����� �� ����� � ����� ������� ������� ������?
	bool m_bg_is_dark;				// ������ ���?
	bool m_draw_clipboard;			// �������� �� ����� ���������� ������� ������? (��� �� ������ ��� �� ��������? (Ctrl + V))
	bool m_animation_mode;
	v2i m_clipboard_pos;			// ������������ ������ ������
	int m_brush_size;				// ������ �����
	int m_erase_size;				// ������ �������

	void render (State state);		// �� ��� ������� ��� ��� ������ �� ����...
	void update (State state, float dt);
	void clear ();
	void load ();

	void make_commit ();			// �������������
	void checkout ();				// ��������� ���������� ������
};