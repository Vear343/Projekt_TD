#pragma once
#include <cmath>

class Vector2D {
public:
    Vector2D(float setX, float setY) : x(setX), y(setY) {}
	Vector2D(const Vector2D& other) : x(other.x), y(other.y) {}
	Vector2D(float angleRad) : x(cos(angleRad)), y(sin(angleRad)) {}
	Vector2D() : x(0.0f), y(0.0f) {}

	float angle() { return atan2(y, x); }

	float magnitude() { return sqrt(x * x + y * y); }
	Vector2D normalize();
	Vector2D getNegativeReciprocal() { return Vector2D(-y, x); }

	float dot(const Vector2D& other) { return x * other.x + y * other.y; }
	float cross(const Vector2D& other) { return x * other.y - y * other.x; }
	float angleBetween(const Vector2D& other) { return atan2(cross(other), dot(other)); }


	Vector2D operator+(const float amount) { return Vector2D(x + amount, y + amount); }
	Vector2D operator-(const float amount) { return Vector2D(x - amount, y - amount); }
	Vector2D operator*(const float amount) { return Vector2D(x * amount, y * amount); }
	Vector2D operator/(const float amount) { return Vector2D(x / amount, y / amount); }

	Vector2D operator+(const Vector2D& other) { return Vector2D(x + other.x, y + other.y); }
	Vector2D operator-(const Vector2D& other) { return Vector2D(x - other.x, y - other.y); }
	Vector2D operator*(const Vector2D& other) { return Vector2D(x * other.x, y * other.y); }
	Vector2D operator/(const Vector2D& other) { return Vector2D(x / other.x, y / other.y); }

	Vector2D& operator+=(const float amount) { x += amount; y += amount; return *this; }
	Vector2D& operator-=(const float amount) { x -= amount; y -= amount; return *this; }
	Vector2D& operator*=(const float amount) { x *= amount; y *= amount; return *this; }
	Vector2D& operator/=(const float amount) { x /= amount; y /= amount; return *this; }

	Vector2D& operator+=(const Vector2D& other) { x += other.x; y += other.y; return *this; }
	Vector2D& operator-=(const Vector2D& other) { x -= other.x; y -= other.y; return *this; }
	Vector2D& operator*=(const Vector2D& other) { x *= other.x; y *= other.y; return *this; }
	Vector2D& operator/=(const Vector2D& other) { x /= other.x; y /= other.y; return *this; }

	float x, y;
};