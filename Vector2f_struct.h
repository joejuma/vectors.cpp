#pragma once
/*
	# 2D Vector Structure
	By Joseph Juma

	## About
	A 2D Euclidean 32-bit floating-point vector structure implementation in
	C++. Useful for all your geometry needs.

	## License
	Feel free to use this code as you see fit, but please just give me some
	credit.

	## To Do

	1.	Look into implementing divide-by-zero logic for many of the methods
		wrapped in the `SAFE` feature flag.

	2.	Look into implementing a `lengthPrecise` that uses float64 for the
		multiplication, then down-casts to float32. This stops overflow unless
		it would overflow a float64.

	## LLM Disclosure
	-	LLMs were used to generate the three additional binary operator impls
		from a hand-written addition operator. So, every human-written `+` was
		passed to an LLM which generated the `-`,`*` and `/` variants.

	-	LLMs were not used for any other aspect of this code.

	## Copyright
	Copyright 2024-2026, Joseph Juma. All rights reserved.

*/
#ifndef VECTOR_2F_STRUCT__H
#define VECTOR_2F_STRUCT__H
/* Deps */
#include <cstdint>
#include <cmath>
#include <initializer_list>
#include <string>

/* Type Aliases */
#ifndef _TYPEDEF_FLOAT32_
#define _TYPEDEF_FLOAT32_
typedef float float32;
#endif
#ifndef _TYPEDEF_FLOAT64_
#define _TYPEDEF_FLOAT64_
typedef double float64;
#endif

/* Macros */

// Constant Macros
#ifndef MATH_PI
#define MATH_PI 3.14159265f
#endif

// Feature Flag Macros
#ifndef _VECTOR2F_TARGET_BEHAVIOR
#define _VECTOR2F_TARGET_BEHAVIOR_SAFE 0x01
#define _VECTOR2F_TARGET_BEHAVIOR_FAST 0x02
#define _VECTOR2F_TARGET_BEHAVIOR _VECTOR2F_TARGET_BEHAVIOR_FAST
#endif


/* Functions */

// Radian-Degree Conversion Macros
#ifndef CONVERT_RADIANS_TO_DEGREES_F32
#define CONVERT_RADIANS_TO_DEGRESS_F32
inline float32 convert_rads_to_degs(const float32 r)
{
	return (r / MATH_PI) * 180.0f;
};
#endif

#ifndef CONVERT_DEGREES_TO_RADIANS_F32
#define CONVERT_DEGREES_TO_RADIANS_F32
inline float32 convert_degs_to_rads(const float32 d)
{
	return (d / 180.0f) * MATH_PI;
};
#endif

/* Structure */
struct Vector2f
{
	/*
		# 2D Vector  (struct)

		## About
		A 2D Euclidean vector structure comprised of 32-bit floating-point
		elements.

		## Notes
		-	Every method is `inline` because they're all less than 8
			functional loc ("rule of 8").

		-	Man this feels like the most thoroughly engineered version of
			Vector2f I've ever made despite lacking the rotation methods.
	*/

	/* Elements */
	float32 values[2];

	/* Methods */

	// Constructors & Destructor
	Vector2f() : values{ 0.0f, 0.0f }
	{};
	Vector2f(const float32 _x, const float32 _y) : values { _x, _y }
	{};
	Vector2f(const int32_t _x, const int32_t _y) : values { static_cast<float32>(_x), static_cast<float32>(_y) }
	{};
	Vector2f(const Vector2f& src) : values { src[0], src[1] }
	{};
	~Vector2f()
	{
		values[0] = 0.0f;
		values[1] = 0.0f;
	};

	// Assignment Operator
	inline Vector2f& operator=(const Vector2f& B)
	{
		values[0] = B.values[0];
		values[1] = B.values[1];
		return (*this);
	};

	// Comparison Operators
	inline bool operator==(const Vector2f& B) const
	{
		return ((values[0] == B.values[0]) && (values[1] == B.values[1]));
	};
	inline bool operator!=(const Vector2f& B) const
	{
		return !((*this) == B);
	};

	// Access Operators
	inline float32& operator[](const uint8_t i)
	{
		#if (_VECTOR2F_TARGET_BEHAVIOR == _VECTOR2F_TARGET_BEHAVIOR_FAST)
		return (values[i]);
		#elif (_VECTOR2F_TARGET_BEHAVIOR == _VECTOR2F_TARGET_BEHAVIOR_SAFE)
		return *(values + (i & static_cast<uint8_t>(1)));
		#endif
		// @note: If neither is true this *should* throw a compiler error.
	};
	inline float32 operator[](const uint8_t i) const
	{
		#if (_VECTOR2F_TARGET_BEHAVIOR == _VECTOR2F_TARGET_BEHAVIOR_FAST)
		return (values[i]);
		#elif (_VECTOR2F_TARGET_BEHAVIOR == _VECTOR2F_TARGET_BEHAVIOR_SAFE)
		return *(values + (i & static_cast<uint8_t>(1)));
		#endif
		// @note: If neither is true this *should* throw a compiler error.
	};

	inline float32& x()
	{
		return values[0];
	};
	inline float32 x() const
	{
		return values[0];
	};

	inline float32& y()
	{
		return values[1];
	};
	inline float32 y() const
	{
		return values[1];
	};

	// Size Methods
	inline constexpr size_t size() const
	{
		return static_cast<size_t>(2);
	};

	// Iterator Methods
	inline const float32* begin() const
	{
		return values;
	};
	inline const float32* end() const
	{
		return (values + 2);
	};

	// Magnitude Methods
	inline float32 length() const
	{
		return std::sqrt(
			(values[0] * values[0]) +
			(values[1] * values[1])
		);
	};
	inline float32 magnitude() const
	{
		return this->length();
	};
	inline float32 sum() const
	{
		return (std::abs(values[0]) + std::abs(values[1]));
	};

	// Normalization Methods
	inline float32 norm() const
	{
		// Keeping separate from `length` since they're semantically distinct.
		return std::sqrt(
			(values[0] * values[0]) +
			(values[1] * values[1])
		);
	};
	inline float32 pnorm(const float32 p) const
	{
		/*
			A Lebesgue p-Norm, where the exponent (2) in the norm formula is
			generalized into a variable p. Thanks again Henri!

			## Note
			-	Doesn't check that `p >= 1` which is required for the
				mathematical p-norm definition.
		*/
		return powf(
			powf(std::abs(values[0]), p) +
			powf(std::abs(values[1]), p),
			(1.0f / p)
		);
	};

	inline Vector2f unit() const
	{
		// Division by length.
		return ((*this) / this->norm());
	};
	inline Vector2f normal() const
	{
		// Division by absolute sum.
		return ((*this) / this->sum());
	};

	inline Vector2f direction() const
	{
		return this->unit();
	};

	// Binary Scalar Arithmetic Operators
	inline Vector2f operator+(const float32 b) const
	{
		return Vector2f(values[0] + b, values[1] + b);
	};
	inline Vector2f operator-(const float32 b) const
	{
		return Vector2f(values[0] - b, values[1] - b);
	};
	inline Vector2f operator*(const float32 b) const
	{
		return Vector2f(values[0] * b, values[1] * b);
	};
	inline Vector2f operator/(const float32 b) const
	{
		return Vector2f(values[0] / b, values[1] / b);
	};

	inline Vector2f& operator+=(const float32 b)
	{
		values[0] += b;
		values[1] += b;
		return (*this);
	};
	inline Vector2f& operator-=(const float32 b)
	{
		values[0] -= b;
		values[1] -= b;
		return (*this);
	};
	inline Vector2f& operator*=(const float32 b)
	{
		values[0] *= b;
		values[1] *= b;
		return (*this);
	};
	inline Vector2f& operator/=(const float32 b)
	{
		values[0] /= b;
		values[1] /= b;
		return (*this);
	};

	// Binary Vector Arithmetic Operators
	inline Vector2f operator+(const Vector2f& B) const
	{
		return Vector2f(values[0] + B[0], values[1] + B[1]);
	};
	inline Vector2f operator-(const Vector2f& B) const
	{
		return Vector2f(values[0] - B[0], values[1] - B[1]);
	};
	inline Vector2f operator*(const Vector2f& B) const
	{
		return Vector2f(values[0] * B[0], values[1] * B[1]);
	};
	inline Vector2f operator/(const Vector2f& B) const
	{
		return Vector2f(values[0] / B[0], values[1] / B[1]);
	};

	inline Vector2f& operator+=(const Vector2f& B)
	{
		values[0] += B[0];
		values[1] += B[1];
		return (*this);
	};
	inline Vector2f& operator-=(const Vector2f& B)
	{
		values[0] -= B[0];
		values[1] -= B[1];
		return (*this);
	};
	inline Vector2f& operator*=(const Vector2f& B)
	{
		values[0] *= B[0];
		values[1] *= B[1];
		return (*this);
	};
	inline Vector2f& operator/=(const Vector2f& B)
	{
		values[0] /= B[0];
		values[1] /= B[1];
		return (*this);
	};

	// Vector Product Methods
	inline float32 dot(const Vector2f& B) const
	{
		return ((values[0] * B[0]) + (values[1] * B[1]));
	};

	// Vector Projection Methods
	inline float32 scalarProjection(const Vector2f& B) const
	{
		/*
			Performs a scalar vector projection of this vector onto the given
			vector (B).
		*/
		return this->dot(B.unit());
	};

	// Angle Methods
	// @todo: it's too early in the morning to be fcking with tangents.
	// inline float32 getAngleRads() const
	// inline float32 getAngleDegs() const

	// Binary Vector Angle Methods
	// inline float32 getAngleBetweenDegs(const Vector2f& B) const
	// inline float32 getAngleBetweenRads(const Vector2f& B) const

	// Rotation Methods
	// inline Vector2f rotateDegs(float32 degs) const
	// inline Vector2f rotateRads(float32 rads) const

	inline Vector2f rot90() const
	{
		return Vector2f((-1.0f * values[1]), values[0]);
	};
	inline Vector2f rot180() const
	{
		return ((*this) * -1.0f);
	};
	inline Vector2f rot360() const
	{
		return (*this); // Why did you do this? Why did I write this? Humanity :^)
	};

	inline Vector2f mirror() const
	{
		return rot180();
	};

	// String Serialization Methods
	inline std::string toString() const
	{
		return ("(" + std::to_string(values[0]) + "," + std::to_string(values[1]) + ")");
	};
};

/* Structure Aliases */
typedef Vector2f vec2f;

/* Functions */

// Binary Scalar Arithmetic Functions
inline Vector2f operator+(const float32 a, const Vector2f& B)
{
	return Vector2f(a + B[0], a + B[1]);
};
inline Vector2f operator-(const float32 a, const Vector2f& B)
{
	return Vector2f(a - B[0], a - B[1]);
};
inline Vector2f operator*(const float32 a, const Vector2f& B)
{
	return Vector2f(a * B[0], a * B[1]);
};
inline Vector2f operator/(const float32 a, const Vector2f& B)
{
	return Vector2f(a / B[0], a / B[1]);
};

// Absolute Value Functions
inline Vector2f abs(const Vector2f& A)
{
	return Vector2f(std::abs(A[0]), std::abs(A[1]));
};

#endif