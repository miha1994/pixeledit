#ifndef MATH_3D_H
#define	MATH_3D_H

#define v2f Vector2<float>
#define v3f Vector3<float>
#define v2i Vector2<int>
#define v3i Vector3<int>

template <class T>
struct Vector2;

template <class T>
struct Vector3
{
    T x;
    T y;
    T z;
	Vector3 () {}
	Vector3 (T _x, T _y, T _z) {
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3 (Vector2 <T> v2);
	Vector3& operator = (const Vector3& r); 
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
struct Vector2
{
    T x;
    T y;
	
	Vector2 () {}
	Vector2 (T _x, T _y) {
		x = _x;
		y = _y;
	}
	Vector2 (Vector3 <T> v3) {
		x = v3.x;
		y = v3.y;
	}
	Vector2& operator = (const Vector2& r);
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

struct Matrix4f
{
    float m[4][4];
    void operator *= (Matrix4f w);
};

struct Vertex_tex {
	v3f pos;
	v2f tex_coord;
	Vertex_tex () {}
	Vertex_tex (v3f _pos, v2f _tex_coord) {
		pos = _pos;
		tex_coord = _tex_coord;
	}
};

template <class T>
struct Rect {
	Vector2 <T> pos;
	Vector2 <T> size;
	Rect () {}
	Rect (Vector2 <T> _pos, Vector2 <T> _size) {
		pos = _pos;
		size = _size;
	}
	bool operator << (Vector2 <T> v) {
		return v.x >= pos.x && v.x < pos.x + size.x &&
			v.y >= pos.y && v.y < pos.y + size.y;
	}
};

#endif	/* MATH_3D_H */