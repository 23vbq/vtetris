#include "vector2.h"
#include <cmath>
/*
 * Constructor
 */
Vector2::Vector2(float x, float y) {
	this->x = x;
	this->y = y;
}

// Returns magnitude of vector
float Vector2::magnitude() {
	return sqrt(this->x * this->x + this->y * this->y);
}
// Returns normalized copy of vector
Vector2 Vector2::normalized() {
	float magnitude = Vector2::magnitude();
	return Vector2(this->x / magnitude, this->y / magnitude);
}

// Compare with second vector
bool Vector2::Equals(Vector2 other) {
	return (this->x == other.x && this->y == other.y);
}
// Makes existing vector normalized
void Vector2::Normalize() {
	float magnitude = Vector2::magnitude();
	this->x /= magnitude;
	this->y /= magnitude;
}
// Sets x and y of existing vector
void Vector2::Set(float x, float y) {
	this->x = x;
	this->y = y;
}
// Converts to string
std::string Vector2::ToString() {
	return (std::string) "( " + std::to_string(this->x) + ", " + std::to_string(this->y) + " )";
}

Vector2 Vector2::operator-(Vector2 &rhs) {
	return Vector2(this->x - rhs.x, this->y - rhs.y);
}
Vector2 Vector2::operator*(float &rhs) {
	return Vector2(this->x * rhs, this->y * rhs);
}
Vector2 Vector2::operator/(float &rhs) {
	return Vector2(this->x / rhs, this->y / rhs);
}
Vector2 Vector2::operator+(Vector2 &rhs) {
	return Vector2(this->x + rhs.x, this->y + rhs.y);
}
bool Vector2::operator==(Vector2 &rhs) {
	return (this->x == rhs.x && this->y == rhs.y);
}
bool Vector2::operator!=(Vector2 &rhs) {
	return (this->x != rhs.x || this->y != rhs.y);
}

float Vector2::Dot(Vector2 a, Vector2 b) {
	return (a.x * b.x + a.y * b.y);
}
float Vector2::Angle(Vector2 from, Vector2 to) {
	return acos(Dot(from, to) / (from.magnitude() * to.magnitude())) * 180.0 / M_PI;
}
float Vector2::Distance(Vector2 a, Vector2 b) {
	return (a - b).magnitude();
}
Vector2 Vector2::Min(Vector2 a, Vector2 b) {
	return Vector2((a.x < b.x ? a.x : b.x), (a.y < b.y ? a.y : b.y));
}
Vector2 Vector2::Max(Vector2 a, Vector2 b) {
	return Vector2((a.x > b.x ? a.x : b.x), (a.y > b.y ? a.y : b.y));
}
Vector2 Vector2::Scale(Vector2 a, Vector2 b) {
	return Vector2(a.x * b.x, a.y * b.y);
}

const Vector2 Vector2::zero = Vector2(0, 0);
const Vector2 Vector2::one = Vector2(1, 1);
const Vector2 Vector2::up = Vector2(0, 1);
const Vector2 Vector2::down = Vector2(0, -1);
const Vector2 Vector2::left = Vector2(-1, 0);
const Vector2 Vector2::right = Vector2(1, 0);