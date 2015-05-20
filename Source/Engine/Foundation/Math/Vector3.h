#pragma once
#include "MathFunctions.h"

namespace Vanguard
{
	class Vector3
	{
	public:
		float X;
		float Y;
		float Z;

		// Constructors

		Vector3(float aX, float aY, float aZ)
		{
			X = aX;
			Y = aY;
			Z = aZ;
		}

		Vector3() : Vector3(0.0f, 0.0f, 0.0f){}

		// Defaults

		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Right;
		static const Vector3 Left;
		static const Vector3 Forward;
		static const Vector3 Back;
		static const Vector3 Zero;

		// Operators

		inline Vector3 operator-() const { return Vector3(-X, -Y, -Z); }

		inline bool operator==(const Vector3& aV) { return X == aV.X && Y == aV.Y && Z == aV.Z; }

		inline Vector3 operator+(const Vector3& aV) const { return Vector3(X + aV.X, Y + aV.Y, Z + aV.Z); }
		inline Vector3 operator-(const Vector3& aV) const { return Vector3(X - aV.X, Y - aV.Y, Z - aV.Z); }
		inline Vector3 operator*(const Vector3& aV) const { return Vector3(X * aV.X, Y * aV.Y, Z * aV.Z); }
		inline Vector3 operator/(const Vector3& aV) const { return Vector3(X / aV.X, Y / aV.Y, Z / aV.Z); }

		inline Vector3 operator+(const float& aF) const	{ return Vector3(X + aF, Y + aF, Z + aF); }
		inline Vector3 operator-(const float& aF) const	{ return Vector3(X - aF, Y - aF, Z - aF); }
		inline Vector3 operator*(const float& aF) const { return Vector3(X * aF, Y * aF, Z * aF); }
		inline Vector3 operator/(const float& aF) const { return Vector3(X / aF, Y / aF, Z / aF); }

		// Functions

		inline float Dot(const Vector3& aV) const { return X*aV.X + Y*aV.Y + Z*aV.Z; }

		inline Vector3 Cross(const Vector3& aV) const { return Vector3(Y*aV.Z - Z*aV.Y, Z*aV.X - X*aV.Z, X*aV.Y - Y*aV.X); }

		inline float Length() const { return Math::Sqrt(SqrLength()); }

		inline float SqrLength() const { return X*X + Y*Y + Z*Z; }

	};
}
