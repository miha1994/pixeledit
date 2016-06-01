#pragma once
#include "object.h"
#include "math_3d.h"
#include "Renderer.h"
#include "glob.h"
#include "array_2d.h"

// состояния нашего эдитора
enum PICTURE_WIND_STATES {
	PWS_B,	// кисть
	PWS_Z,	// zoom, unzoom
	PWS_I,	// считывание цвета для кисти с картинки
	PWS_E,	// стерка (ластик)
	PWS_M,	// выделение (прямоугольник)
	PWS_V,	// перемещение вставленного из clipboard набора пикселей, текстуры
	PWS_W,	// выделение пикселей одного цвета
	PWS_U,	// изменение цвета в выделенной области
	PWS_PAUSE	// пауза (нужна для того, чтобы эдитор не реагировал на нажатия кнопок пока мы открыли гугл и смотрим смешные видосики)
};

// комит. Помнит положение / состояние эдитора в какой-то момент времени. Нужен для Ctrl + Z. Из коммитов составляется история
struct commit {
	vector <rgba_array> m_layers;	// слои
	v2i m_pos;						// смещение / позиция
	int m_scale;					// масштаб
	int m_cur_layer;				// номер активного слоя
	bool m_draw_clipboard;			// рисовать ли clipboard?
	array_2d <bool> m_mask;			// маска выделения
	commit& operator = (const commit& r);	// оператор присваивания
	commit () {}					// пустой конструктор
	commit (vector <rgba_array >_m_layers, v2i _m_pos, int _m_scale, int _m_cur_layer, array_2d <bool> _m_mask, bool _m_draw_clipboard) {
		m_layers = _m_layers;
		m_pos = _m_pos;
		m_scale = _m_scale;
		m_cur_layer = _m_cur_layer;
		m_mask = _m_mask;
		m_draw_clipboard = _m_draw_clipboard;
	}
};

// наш великий и могучий эдитор
struct picture_wind : Object {
	vector <rgba_array> m_layers;		// слои
	vector <rgba_array> m_layers_miniatures;	// миниатюры слоев, находятся справа
	vector <commit> m_history;			// история изменений
	rgba_array m_main_sq;				// основной квадрат (в него рисуются все слои. Квадрат - это наши глаза. на экран выводится только он а не все слои полностью. То, что не попало в него нам не видно)
	rgba_array m_bg;		// фон
	rgba_array m_i_color;	// квадратик вокруг курсора, показывающий наглядно цвет пикселя на который в данный момент указывает курсор. Активируется только во время состояния PWS_I
	rgba_array m_active_colors[2];	// квадратики, показывающие текущие цвета кисти
	rgba_array m_color_panel;	// панель цветов. кликнув по какому-нибудь ее пикселю можно изменить текущие цвета кисти
	rgba_array m_bounds;		// хранит изображения пунктиров, которые изображают границу области, которую на данный момент выделяет пользователь с помощью инструмента прямоугольное выделение (M)
	rgba_array m_mask_bounds;	// хранит изображения пунктиров, которые изображают границу выделенной области области
	rgba_array m_clipboard;		// буфер обмена. по сути хранит куски изображений, которые можно затем вставить. Попадают туда эти куски благодаря Ctrl + C
	rgba_array m_u_mask;		// псевдослой, неявно создающийся при изменении цвета выделенной области (инструмент U)
	array_2d <bool> m_mask;		// маска выделенной области (пиксели, которые входят в выделенную область помечены единицами)
	v2i m_pos;				// смещение / позиция рисунка относительно основного квадрата
	int m_scale;			// масштаб
	int m_cur_layer;		// активный слой
	int m_cur_history;		// текущий commit истории
	PICTURE_WIND_STATES m_state;	// состояние
	CLR m_color1, m_old_col1;		// основной цвет кистии переменная, которая хранит это же значение, но которое было один фрейм назад
	CLR m_color2, m_old_col2;		// второстепенный цвет --//--
	v2i m_i_color_pos;				// местоположение квадрата m_i_color
	v2i m_space_pos;				// m_space_pos и m_space_pict_pos используются для перемещения картинки при нажатии space
	v2i m_space_pict_pos;
	bool m_commit_need;				// нужно ли будет в конце апдэйта сделать коммит?
	bool m_bg_is_dark;				// темный фот?
	bool m_draw_clipboard;			// выводить на экран содержимое буффера обмена? (был ли только что он вставлен? (Ctrl + V))
	v2i m_clipboard_pos;			// расположение буфера обмена
	int m_brush_size;				// размер кисти
	int m_erase_size;				// размер ластика

	void render (State state);		// ну эти функции уже где только не были...
	void update (State state, float dt);
	void clean ();
	void load ();

	void make_commit ();			// закоммититься
	void checkout ();				// загрузить предыдущий коммит
};