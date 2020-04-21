#include "Vector2.h"



Vector2 Vector2::operator + (const Vector2& other) const {
	return Vector2(X + other.X, Y + other.Y);
}
Vector2 Vector2::operator - (const Vector2& other) const {
	return Vector2(X - other.X, Y - other.Y);
}
Vector2 Vector2::operator * (const float other) const {
	return Vector2(X * other, Y * other);
}

Vector2 Vector2::operator / (const float other)  const {
	return Vector2(X / other, Y / other);
}
void Vector2::operator *= (const float other)  {
	X *= other;
	Y *= other;
}
void Vector2::operator /= (const float other)  {
	X /= other;
	Y /= other;
}

void Vector2::operator += (const Vector2& other) {
	X += other.X;
	Y += other.Y;
}
void Vector2::operator -= (const Vector2& other) {
	X -= other.X;
	Y -= other.Y;
}
bool Vector2::operator == (const Vector2& other) const {
	return X == other.X && Y == other.Y;
}
bool Vector2::operator != (const Vector2& other) const {
	return !(*this == other);
}
void Vector2::operator = (const Vector2& other)  {
	this->X = other.X;
	this->Y = other.Y;
}

const std::string Vector2::toString() {
	
	return  "(" + std::to_string(X) + ", " + std::to_string(Y) + ")";
}

Vector2::Vector2()
{
	X = 0;
	Y = 0;
}


Vector2::Vector2(float x, float y) {
	X = (double)x;
	Y = (double)y;
}

Vector2::Vector2(int x, int y) {
	X = (double)x;
	Y = (double)y;
}
Vector2::Vector2(double x, double y) {
	X = x;
	Y = y;
}
Vector2::~Vector2()
{
}

void Vector2::Set(float x, float y)
{
	X = x;
	Y = y;
}

void Vector2::Set(int x, int y)
{
	X = x;
	Y = y;
}

void Vector2::Set(double x, double y)
{
	X = x;
	Y = y;
}

Vector2 Vector2::Normalized() {
	if (X == 0 && Y == 0)
	{
		return Vector2(1, 0);
	}
	double l = sqrt((X * X) + (Y*Y));
	return Vector2(X / l,  Y / l);
}
Vector2 Vector2::Normalize() {
	float l = (float)sqrt((X*X) + (Y*Y));
	X /= l;
	Y /= l;
	return *this;
}


void Vector2::flipXY() {
	X = X + Y;
	Y = X - Y;
	X = X - Y;

}


