#pragma once

struct Vec2
{
	static const Vec2 right;
	static const Vec2 up;

	float x;
	float y;

	Vec2() : x(0.f), y(0.f) {}
	Vec2(float s) : x(s), y(s) {}
	Vec2(float x, float y) : x(x), y(y) {}

	Vec2 operator+() const { return Vec2(x, y); }
	Vec2 operator-() const { return Vec2(-x, -y); }

	Vec2 operator+(const Vec2& other) const { return Vec2(x + other.x, y + other.y); }
	Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
	Vec2 operator*(float scalar) const { return Vec2(x * scalar, y * scalar); }

	Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
	Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }
	Vec2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
};

struct Vec3
{
	static const Vec3 zero;
	static const Vec3 one;
	static const Vec3 forward;
	static const Vec3 up;
	static const Vec3 right;

	float x;
	float y;
	float z;

	Vec3() : x(0.f), y(0.f), z(0.f) {}
	explicit Vec3(float s) : x(s), y(s), z(s) {}
	Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
	explicit Vec3(const Vec2& v) : x(v.x), y(v.y), z(0.f) {}

	Vec3 operator+() const { return *this; }
	Vec3 operator-() const { return Vec3(-x, -y, -z); }

	Vec3 operator+(const Vec3& other) const { return Vec3(x + other.x, y + other.y, z + other.z); }
	Vec3 operator-(const Vec3& other) const { return Vec3(x - other.x, y - other.y, z - other.z); }
	Vec3 operator*(float scalar) const { return Vec3(x * scalar, y * scalar, z * scalar); }
	Vec3 operator/(float scalar) const { return Vec3(x / scalar, y / scalar, z / scalar); }

	Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
	Vec3& operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	Vec3& operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
};