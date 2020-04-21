#pragma once
#include <math.h> 
#include <string>
class Vector2
{


public:
	double X;
	double Y;
	Vector2 operator + (const Vector2& other) const;
	Vector2 operator - (const Vector2& other) const;
	bool operator == (const Vector2& other) const;
	bool operator != (const Vector2& other) const;
	Vector2 operator * (const float other) const;
	const std::string toString();
	Vector2 operator / (const float other) const;
	void operator *= (const float other);
	void operator /= (const float other);
	void operator += (const Vector2& other);
	void operator -= (const Vector2& other);
	void operator = (const Vector2& other);
	Vector2 Normalized();
	void flipXY();
	Vector2 Normalize();
	Vector2();
	Vector2(float x, float y);
	Vector2(int x, int y);
	Vector2(double x, double y);
	~Vector2();
	void Set(float x, float y);
	void Set(int x, int y);
	void Set(double x, double y);
	static Vector2 Lerp(const Vector2& currentVector, const Vector2& targetVector, const float lerpAmount, float minDist)
	{
		Vector2 newVector = Vector2(0, 0);
		//float lerp(float a, float b, float f)
		{
			//newVector.X = (currentVector.X * (1.0 - lerpAmount)) + targetVector.X * lerpAmount;
			//newVector.Y = (currentVector.Y * (1.0 - lerpAmount)) + targetVector.Y * lerpAmount;
			newVector = (currentVector * (const float)(1.0 - lerpAmount)) + targetVector * lerpAmount;
			if (GetMagnitude(currentVector, targetVector) < minDist)
			{
				return targetVector;
			}
			//return (a * (1.0 - f)) + (b * f);
			return newVector;
		}
	}
	static float GetSquareDistance(const Vector2& a, const Vector2& b) {

		Vector2 v = (a - b);
		return (float)(v.X *  v.X + v.Y * v.Y);
	}
	static float GetMagnitude(const Vector2& a, const Vector2& b) {
		Vector2 v = (a - b);
		return (float)sqrt(v.X * v.X + v.Y * v.Y);
	}
	static Vector2 GetDirection(const Vector2& a, const Vector2& b) {
		return  (a - b) / GetMagnitude(a, b);
	}

	
	static float Dot(const Vector2& a, const Vector2& b) {

		return (float)((a.X * b.X) + (a.Y * b.Y));
	}

	static bool GetDistanceLessThan(const Vector2& val, const Vector2& val1, float dist) {
		return Vector2::GetSquareDistance(val, val1) <= dist * dist;
	}//gets if the distance between to vectors is smaller than some value
	static bool GetDistanceGreaterThan(const Vector2& val, const Vector2& val1, float dist)
	{
		return Vector2::GetSquareDistance(val, val1) >= dist * dist;
	}//gets if the distance between to vectors is greater than some value
	
};

