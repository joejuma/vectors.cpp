#pragma once
/*
	# 3D Vector Structure
	By Joseph Juma

	## About
	A 3D Euclidean 32-bit floating-point vector structure implementation in
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
#ifndef VECTOR_3F_STRUCT__H
#define VECTOR_3F_STRUCT__H
/* Deps */
#include <cstdint>
#include <cmath>
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
#ifndef _VECTOR3F_TARGET_BEHAVIOR
#define _VECTOR3F_TARGET_BEHAVIOR_SAFE 0x01
#define _VECTOR3F_TARGET_BEHAVIOR_FAST 0x02
#define _VECTOR3F_TARGET_BEHAVIOR _VECTOR3F_TARGET_BEHAVIOR_FAST
#endif

/* Functions */
// @placeholder

struct Vector3f
{
	/*
		# 3D Vector  (struct)

		## About
		A 3D Euclidean vector structure comprised of 32-bit floating-point
		elements.

		## Notes
		@placeholder

	*/

	/* Elements */
	float32 values[3];

	/* Methods */

	// Constructors & Destructor
	Vector3f() : values { 0.0f, 0.0f, 0.0f }
	{};
	Vector3f(const float32 _x, const float32 _y, const float32 _z) : values{ _x, _y, _z }
	{};
	Vector3f(const int32_t _x, const int32_t _y, const int32_t _z) : values{ static_cast<float32>(_x), static_cast<float32>(_y), static_cast<float32>(_z) }
	{};
	Vector3f(const Vector3f& src) : values{ src.values[0], src.values[1], src.values[2] }
	{};
	~Vector3f()
	{
		values[0] = 0.0f;
		values[1] = 0.0f;
		values[2] = 0.0f;
	};

	// Assignment Operators
	inline Vector3f& operator=(const Vector3f& B)
	{
		values[0] = B.values[0];
		values[1] = B.values[1];
		values[2] = B.values[2];
		return (*this);
	};
	
	// Comparison Operators
	inline bool operator==(const Vector3f& B) const
	{
		return ((values[0] == B.values[0]) && (values[1] == B.values[1]) && (values[2] == B.values[2]));
	};
	inline bool operator!=(const Vector3f& B) const
	{
		return !((*this) == B);
	};

	// Access Operators
	inline float32& operator[](const uint8_t i)
	{
		#if (_VECTOR3F_TARGET_BEHAVIOR == _VECTOR3F_TARGET_BEHAVIOR_FAST)
		return (values[i]);
		#elif (_VECTOR3F_TARGET_BEHAVIOR == _VECTOR3F_TARGET_BEHAVIOR_SAFE)
		return *(values + (i % static_cast<uint8_t>(3)));
		#endif
		// @note: If neither is true this *should* throw a compiler error.
	};
	inline float32 operator[](const uint8_t i) const
	{
		#if (_VECTOR3F_TARGET_BEHAVIOR == _VECTOR3F_TARGET_BEHAVIOR_FAST)
		return (values[i]);
		#elif (_VECTOR3F_TARGET_BEHAVIOR == _VECTOR3F_TARGET_BEHAVIOR_SAFE)
		return *(values + (i % static_cast<uint8_t>(3)));
		#endif
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

	inline float32& z()
	{
		return values[2];
	};
	inline float32 z() const
	{
		return values[2];
	};

	// Size Methods
	inline constexpr size_t size() const
	{
		return static_cast<size_t>(3);
	};

	// Iterator Methods
	inline float32* begin()
	{
		return values;
	};
	inline float32* end()
	{
		return (values + 3);
	};

	inline const float32* begin() const
	{
		return values;
	};
	inline const float32* end() const
	{
		return (values + 3);
	};

	// Magnitude Methods
	inline float32 length() const
	{
		return std::sqrt(
			(values[0] * values[0]) +
			(values[1] * values[1]) +
			(values[2] * values[2])
		);
	};
	inline float32 magnitude() const
	{
		return this->length();
	};
	inline float32 sum() const
	{
		return (std::abs(values[0]) + std::abs(values[1]) + std::abs(values[2]));
	};

	// Normalization Methods
	inline float32 norm() const
	{
		// Keeping separate from `length` since they're semantically distinct.
		return std::sqrt(
			(values[0] * values[0]) +
			(values[1] * values[1]) +
			(values[2] * values[2])
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
			powf(std::abs(values[1]), p) +
			powf(std::abs(values[2]), p),
			(1.0f / p)
		);
	};

	inline Vector3f unit() const
	{
		// Division by length.
		return ((*this) / this->norm());
	};
	inline Vector3f normal() const
	{
		// Division by absolute sum.
		return ((*this) / this->sum());
	};

	inline Vector3f direction() const
	{
		return this->unit();
	};

	// Binary Scalar Arithmetic Operators
	inline Vector3f operator+(const float32 b) const
	{
		return Vector3f(values[0] + b, values[1] + b, values[2] + b);
	};
	inline Vector3f operator-(const float32 b) const
	{
		return Vector3f(values[0] - b, values[1] - b, values[2] - b);
	};
	inline Vector3f operator*(const float32 b) const
	{
		return Vector3f(values[0] * b, values[1] * b, values[2] * b);
	};
	inline Vector3f operator/(const float32 b) const
	{
		return Vector3f(values[0] / b, values[1] / b, values[2] / b);
	};

	// Compound Scalar Assignment Operators
	inline Vector3f& operator+=(const float32 b)
	{
		values[0] += b;
		values[1] += b;
		values[2] += b;
		return (*this);
	};
	inline Vector3f& operator-=(const float32 b)
	{
		values[0] -= b;
		values[1] -= b;
		values[2] -= b;
		return (*this);
	};
	inline Vector3f& operator*=(const float32 b)
	{
		values[0] *= b;
		values[1] *= b;
		values[2] *= b;
		return (*this);
	};
	inline Vector3f& operator/=(const float32 b)
	{
		values[0] /= b;
		values[1] /= b;
		values[2] /= b;
		return (*this);
	};

	// Binary Vector Arithmetic Operators
	inline Vector3f operator+(const Vector3f& B) const
	{
		return Vector3f(values[0] + B[0], values[1] + B[1], values[2] + B[2]);
	};
	inline Vector3f operator-(const Vector3f& B) const
	{
		return Vector3f(values[0] - B[0], values[1] - B[1], values[2] - B[2]);
	};
	inline Vector3f operator*(const Vector3f& B) const
	{
		return Vector3f(values[0] * B[0], values[1] * B[1], values[2] * B[2]);
	};
	inline Vector3f operator/(const Vector3f& B) const
	{
		return Vector3f(values[0] / B[0], values[1] / B[1], values[2] / B[2]);
	};

	// Compound Vector Assignment Operators
	inline Vector3f& operator+=(const Vector3f& B)
	{
		values[0] += B[0];
		values[1] += B[1];
		values[2] += B[2];
		return (*this);
	};
	inline Vector3f& operator-=(const Vector3f& B)
	{
		values[0] -= B[0];
		values[1] -= B[1];
		values[2] -= B[2];
		return (*this);
	};
	inline Vector3f& operator*=(const Vector3f& B)
	{
		values[0] *= B[0];
		values[1] *= B[1];
		values[2] *= B[2];
		return (*this);
	};
	inline Vector3f& operator/=(const Vector3f& B)
	{
		values[0] /= B[0];
		values[1] /= B[1];
		values[2] /= B[2];
		return (*this);
	};

	// Vector Product Methods
	inline float32 dot(const Vector3f& B) const
	{
		return (values[0] * B[0]) + (values[1] * B[1]) + (values[2] * B[2]);
	};
	inline Vector3f cross(const Vector3f& B) const
	{
		return Vector3f(
			((values[1] * B[2]) - (values[2] * B[1])),
			((values[2] * B[0]) - (values[0] * B[2])),
			((values[0] * B[1]) - (values[1] * B[0]))
		);
	};

	// Vector Projection Methods
	inline float32 scalarProjection(const Vector3f& B) const
	{
		/*
			Performs a scalar vector projection of this vector onto the given
			vector (B).
		*/
		return this->dot(B.unit());
	};

	// Angle Methods
	// @todo

	// Rotation Methods
	// @todo

	// String Serialization Methods
	inline std::string toString() const
	{
		return "(" + std::to_string(values[0]) + "," + std::to_string(values[1]) + "," + std::to_string(values[2]) + ")";
	};
};

/* Structure Aliases */
typedef Vector3f vec3f;

/* Functions */

// Binary Scalar Arithmetic Functions
inline Vector3f operator+(const float32 a, const Vector3f& B)
{
	return Vector3f(a + B[0], a + B[1], a + B[2]);
};
inline Vector3f operator-(const float32 a, const Vector3f& B)
{
	return Vector3f(a - B[0], a - B[1], a - B[2]);
};
inline Vector3f operator*(const float32 a, const Vector3f& B)
{
	return Vector3f(a * B[0], a * B[1], a * B[2]);
};
inline Vector3f operator/(const float32 a, const Vector3f& B)
{
	return Vector3f(a / B[0], a / B[1], a / B[2]);
};

// Absolute Value Functions
inline Vector3f abs(const Vector3f& A)
{
	return Vector3f(std::abs(A[0]), std::abs(A[1]), std::abs(A[2]));
};
#endif