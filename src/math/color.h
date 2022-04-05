#pragma once

struct Color
{
	static const Color black;
	static const Color white;
	static const Color red;
	static const Color green;
	static const Color blue;

	float r;
	float g;
	float b;
	float a;

	Color() {}
	Color(float r, float g, float b) : r(r), g(g), b(b), a(1.f) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};