#ifndef MATH_3D_H
#define	MATH_3D_H

#define v2f Vector2<float> // двухмерный вектор из float
#define v3f Vector3<float> // трехмерный вектор из float
#define v2i Vector2<int>	// двухмерный вектор из int
#define v3i Vector3<int>	// трехмерный вектор из int

template <class T>
struct Vector2;		// предопределение класса двухмерных векторов

template <class T>
struct Vector3		// трехмерные вектора
{
    T x;		// координаты
    T y;
    T z;
	Vector3 () {}	// пустой конструктор
	Vector3 (T _x, T _y, T _z) {	// конструктор 3-х координат
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3 (Vector2 <T> v2);		// конструктор расширения двухмерного вектора до трехмерного
	Vector3& operator = (const Vector3& r);		// операторы присваивания, +=, -=, *=, /=, +, -, *, /, ==, !=
	Vector3& operator += (const Vector3& r); 
	Vector3& operator -= (const Vector3& r); 
	Vector3& operator *= (const float r); 
	Vector3& operator /= (const float r);
	Vector3 operator + (const Vector3& r); 
	Vector3 operator - (const Vector3& r); 
	Vector3 operator * (const float r); 
	Vector3 operator / (const float r); 
	bool operator == (const Vector3& r) {return x == r.x && y == r.y && z == r.z;}
	bool operator != (const Vector3& r) {return x != r.x || y != r.y || z != r.z;}
};

template <class T>
Vector3 <T>& Vector3<T>::operator = (const Vector3 <T>& r) {
	x = r.x;
	y = r.y;
	z = r.z;
	return (*this);
}

template <class T>
Vector3 <T>& Vector3 <T>::operator += (const Vector3 <T>& r) {
	x += r.x;
	y += r.y;
	z += r.z;
	return (*this);
}

template <class T>
Vector3 <T>& Vector3 <T>::operator -= (const Vector3 <T>& r) {
	x -= r.x;
	y -= r.y;
	z -= r.z;
	return (*this);
}

template <class T>
Vector3 <T>& Vector3 <T>::operator *= (const float r) {
	x *= r;
	y *= r;
	z *= r;
	return (*this);
}

template <class T>
Vector3 <T>& Vector3 <T>::operator /= (const float r) {
	x /= r;
	y /= r;
	z /= r;
	return (*this);
}

template <class T>
Vector3 <T> Vector3 <T>::operator + (const Vector3 <T>& r) {
	return Vector3 <T> (x + r.x, y + r.y, z + r.z);
}

template <class T>
Vector3 <T> Vector3 <T>::operator - (const Vector3 <T>& r) {
	return Vector3 <T> (x - r.x, y - r.y, z - r.z);
}

template <class T>
Vector3 <T> Vector3 <T>::operator * (const float r) {
	return Vector3 <T> (x * r, y * r, z * r);
}

template <class T>
Vector3 <T> Vector3 <T>::operator / (const float r) {
	return Vector3 <T> (x / r, y / r, z / r);
}

template <class T>
Vector3 <T>::Vector3 (Vector2 <T> v2) {
	x = v2.x;
	y = v2.y;
	z = 0;
}

template <class T>
struct Vector2		// двухмерные вектора
{
    T x;	// координаты
    T y;
	
	Vector2 () {}		// пустой конструктор
	Vector2 (T _x, T _y) {		//конструктор координат
		x = _x;
		y = _y;
	}
	Vector2 (Vector3 <T> v3) {  // конструктор сужения трехмерного вектора до двухмерного
		x = v3.x;
		y = v3.y;
	}
	Vector2& operator = (const Vector2& r);		// операторы
	Vector2& operator += (const Vector2& r);
	Vector2& operator -= (const Vector2& r);
	Vector2& operator *= (const float r);
	Vector2& operator /= (const float r);
	Vector2 operator + (const Vector2& r);
	Vector2 operator - (const Vector2& r);
	Vector2 operator * (const float r);
	Vector2 operator / (const float r);
	bool operator == (const Vector2& r) {return x == r.x && y == r.y;}
	bool operator != (const Vector2& r) {return x != r.x || y != r.y;}
};

template <class T>
inline bool operator < (const Vector2 <T>& r1, const Vector2 <T>& r2) {
	return r1.x < r2.x || (r1.x == r2.x && r1.y < r2.y);
}

template <class T>
Vector2 <T>& Vector2 <T>::operator = (const Vector2 <T>& r) {
	x = r.x;
	y = r.y;
	return (*this);
}

template <class T>
Vector2 <T>& Vector2 <T>::operator += (const Vector2 <T>& r) {
	x += r.x;
	y += r.y;
	return (*this);
}

template <class T>
Vector2 <T>& Vector2 <T>::operator -= (const Vector2 <T>& r) {
	x -= r.x;
	y -= r.y;
	return (*this);
}

template <class T>
Vector2 <T>& Vector2 <T>::operator *= (const float r) {
	x *= r;
	y *= r;
	return (*this);
}

template <class T>
Vector2 <T>& Vector2 <T>::operator /= (const float r) {
	x /= r;
	y /= r;
	return (*this);
}

template <class T>
Vector2 <T> Vector2 <T>::operator + (const Vector2 <T>& r) {
	return Vector2 <T> (x + r.x, y + r.y);
}

template <class T>
Vector2 <T> Vector2 <T>::operator - (const Vector2 <T>& r) {
	return Vector2 <T> (x - r.x, y - r.y);
}

template <class T>
Vector2 <T> Vector2 <T>::operator * (const float r) {
	return Vector2 <T> (x * r, y * r);
}

template <class T>
Vector2 <T> Vector2 <T>::operator / (const float r) {
	return Vector2 <T> (x / r, y / r);
}

#define me (*this)
#define FOR(z, N)   for (int z = 0; z < N; ++z)

// матрица 4 на 4
struct Matrix4f
{
    float m[4][4];
    void operator *= (Matrix4f w);
};

// вершина, помнящая координаты текстуры. Скармливается опенжиэлю
struct Vertex_tex {
	v3f pos;
	v2f tex_coord;
	Vertex_tex () {}
	Vertex_tex (v3f _pos, v2f _tex_coord) {
		pos = _pos;
		tex_coord = _tex_coord;
	}
};

// прямоугольник со сторонами, параллельными осям координат. Просто геометрич. фигура
template <class T>
struct Rect {
	Vector2 <T> pos;  // верхняя левая точка
	Vector2 <T> size;	// ширина высота
	Rect () {}
	Rect (Vector2 <T> _pos, Vector2 <T> _size) {
		pos = _pos;
		size = _size;
	}
	//принадлежит ли точка прямоугольнику
	bool operator << (Vector2 <T> v) {
		return v.x >= pos.x && v.x < pos.x + size.x &&
			v.y >= pos.y && v.y < pos.y + size.y;
	}
};

#endif	/* MATH_3D_H */