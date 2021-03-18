#include <cmath>
#include<iostream>

#ifndef __vectors_h__
#define __vectors_h__
#define pi 3.14159
#define DEGtoRED(x) (x * pi / 180)

class FlynnVector3
{
public:
	float x, y, z;
	FlynnVector3() : x(0), y(0), z(0){};
	FlynnVector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
	FlynnVector3(const FlynnVector3& rhs):x(rhs.x), y(rhs.y), z(rhs.z){}

	float getLength() const { return sqrt((x * x) + (y * y) + (z * z)); }
	void normalize() { *this /= getLength(); }
	FlynnVector3 normalized() { return *this /= getLength(); }
	float dot(FlynnVector3 rightVec) const { return (x * rightVec.x) + (y * rightVec.y) + (z * rightVec.z); }
	FlynnVector3 cross(const FlynnVector3) const;
	float angle(FlynnVector3);
	float greaterAngle(FlynnVector3 rightVec) { return float((2 * pi) - angle(rightVec)); }
	FlynnVector3 rotate(double A, const FlynnVector3 &r);
	FlynnVector3 operator+(FlynnVector3) const;
	FlynnVector3 operator-(FlynnVector3) const;
	FlynnVector3 operator*(float) const;
	FlynnVector3 operator/(float) const;
	FlynnVector3 &operator+=(FlynnVector3);
	FlynnVector3 &operator-=(FlynnVector3);
	FlynnVector3 &operator*=(float);
	FlynnVector3 &operator/=(float);
	FlynnVector3 &operator=(FlynnVector3);
	bool operator==(const FlynnVector3 &) const;
	bool operator!=(const FlynnVector3 &) const;
};

FlynnVector3 FlynnVector3::cross(const FlynnVector3 rightVec) const
{
	return FlynnVector3((y * rightVec.z) - (z * rightVec.y),
						(z * rightVec.x) - (x * rightVec.z),
						(x * rightVec.y) - (y * rightVec.x));
}

float FlynnVector3::angle(FlynnVector3 rightVec)
{
	FlynnVector3 tempVec = normalized();
	float temp = tempVec.dot(rightVec.normalized());
	return float(acos(temp));
}

FlynnVector3 FlynnVector3::operator+(FlynnVector3 rightVec) const
{
	return FlynnVector3(*this) += rightVec;
}

FlynnVector3 FlynnVector3::operator-(FlynnVector3 rightVec) const
{
	return FlynnVector3(*this) -= rightVec;
}

FlynnVector3 FlynnVector3::operator*(float scalar) const
{
	return FlynnVector3(*this) *= scalar;
}

FlynnVector3 FlynnVector3::operator/(float scalar) const
{
	return FlynnVector3(*this) /= scalar;
}

FlynnVector3 &FlynnVector3::operator+=(FlynnVector3 rightVec)
{
	x += rightVec.x;
	y += rightVec.y;
	z += rightVec.z;
	return *this;
}

FlynnVector3 &FlynnVector3::operator-=(FlynnVector3 rightVec)
{
	x -= rightVec.x;
	y -= rightVec.y;
	z -= rightVec.z;
	return *this;
}

FlynnVector3 &FlynnVector3::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

FlynnVector3 &FlynnVector3::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

FlynnVector3 &FlynnVector3::operator=(FlynnVector3 rightVec)
{
	x = rightVec.x;
	y = rightVec.y;
	z = rightVec.z;
	return *this;
}

bool FlynnVector3::operator==(const FlynnVector3 &rightVec) const
{
	return (x == rightVec.x &&
			y == rightVec.y &&
			z == rightVec.z);
}

bool FlynnVector3::operator!=(const FlynnVector3 &rightVec) const
{
	return !operator==(rightVec);
}

std::ostream &operator<<(std::ostream &sout, const FlynnVector3 &p)
{
	sout << "(" << p.x << "," << p.y << "," << p.z << ")";
	return sout;
}
std::istream &	operator>>(std::istream &sin, FlynnVector3 &p)
{
	sin >> p.x >> p.y >> p.z;
	return sin;
}
FlynnVector3 FlynnVector3::rotate(double A, const FlynnVector3 &r)
{
	A = DEGtoRED(A);
	FlynnVector3 l = *this;
	FlynnVector3 u = r.cross(l);
	return u * sin(A) + l * cos(A);
}

#endif
