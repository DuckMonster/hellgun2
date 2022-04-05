#pragma once
#include <intrin.h>

#define PI (3.14159265359f)
#define HALF_PI (1.57079632679f)
#define TAU (6.28318530718f)
#define KINDA_SMALL_NUMBER (0.0001f)
#define SMALL_NUMBER (0.0000001f)
#define BIG_NUMBER (3.4e+38f)

extern "C"
{
	float sinf(float);
	float cosf(float);
	float tanf(float);
	float fabsf(float);
	float powf(float, float);
}

class Math
{
public:
	static inline bool is_nearly_zero(float val, float check = SMALL_NUMBER) { return val > -check && val < check; }

	static inline float radians(float degrees) { return (degrees / 180.f) * PI; }
	static inline float degrees(float radians) { return (radians / PI) * 180.f; }

	static inline float sin(float rad) { return sinf(rad); }
	static inline float cos(float rad) { return cosf(rad); }
	static inline float tan(float rad) { return tanf(rad); }

	static u32 ceil_po2(u32 val)
	{
		// Edge case - 0 given
		if (val == 0)
			return 0;

		unsigned long msb_index;
		_BitScanReverse(&msb_index, val);

		u32 msb = 1 << msb_index;

		// Edge case - val is already a po2
		if (msb == val)
			return val;

		return msb << 1;
	}

	static float sign(float v) { return v > 0.f ? 1.f : -1.f; }
	static float abs(float v) { return fabsf(v); }

 	static float min(float a, float b) { return a < b ? a : b; }
 	static float min(float a, float b, float c) { return a < b ? (a < c ? a : c) : (b < c ? b : c); }
 	// Selects number with the minimal magnitude (distance from 0)
	static float min_mag(float a, float b) { return abs(a) < abs(b) ? a : b; }

 	static float max(float a, float b) { return a > b ? a : b; }
 	static float max(float a, float b, float c) { return a > b ? (a > c ? a : c) : (b > c ? b : c); }

	template<typename T>
	static void swap(T& a, T& b)
	{
		T temp = a;
		a = b;
		b = temp;
	}

	template<typename T>
	static T clamp(T value, T min, T max)
	{
		if (min > max)
			swap(min, max);

		if (value < min)
			return min;
		if (value > max)
			return max;

		return value;
	}

	static float pow(float base, float exponent)
	{
		return powf(base, exponent);
	}
};