#ifndef VECTOR2
#define VECTOR2
#include <string>

struct Vector2 {
	float x;
	float y;

	Vector2(float x, float y);

	float magnitude();
	Vector2 normalized();

	bool Equals(Vector2 other);
	void Normalize();
	void Set(float x, float y);
	std::string ToString();

	Vector2 operator-(Vector2 &rhs);
	Vector2 operator*(float &rhs);
	Vector2 operator/(float &rhs);
	Vector2 operator+(Vector2 &rhs);
	bool operator==(Vector2 &rhs);
	bool operator!=(Vector2 &rhs);

	static float Dot(Vector2 a, Vector2 b);
	static float Angle(Vector2 from, Vector2 to);
	static float Distance(Vector2 a, Vector2 b);
	static Vector2 Min(Vector2 a, Vector2 b);
	static Vector2 Max(Vector2 a, Vector2 b);
	static Vector2 Scale(Vector2 a, Vector2 b);

	static const Vector2 zero;
	static const Vector2 one;
	static const Vector2 up;
	static const Vector2 down;
	static const Vector2 left;
	static const Vector2 right;
};
#endif