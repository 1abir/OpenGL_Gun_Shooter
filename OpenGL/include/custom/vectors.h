#include <cmath>
#include<iostream>

#ifndef __vectors_h__
#define __vectors_h__
#define pi 3.14159
#define DEGtoRED(x) (x * pi / 180)

class Point3D
{
public:
	float x, y, z;
	Point3D() : x(0), y(0), z(0){};
	Point3D(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
	Point3D(const Point3D& rhs):x(rhs.x), y(rhs.y), z(rhs.z){}

	float getLength() const { return sqrt((x * x) + (y * y) + (z * z)); }
	void normalize() { *this /= getLength(); }
	Point3D normalized() { return *this /= getLength(); }
	float dot(Point3D rightVec) const { return (x * rightVec.x) + (y * rightVec.y) + (z * rightVec.z); }
	Point3D cross(const Point3D) const;
	float angle(Point3D);
	float greaterAngle(Point3D rightVec) { return float((2 * pi) - angle(rightVec)); }
	Point3D rotate(double A, const Point3D &r);
	Point3D operator+(Point3D) const;
	Point3D operator-(Point3D) const;
	Point3D operator*(float) const;
	Point3D operator/(float) const;
	Point3D &operator+=(Point3D);
	Point3D &operator-=(Point3D);
	Point3D &operator*=(float);
	Point3D &operator/=(float);
	Point3D &operator=(Point3D);
	bool operator==(const Point3D &) const;
	bool operator!=(const Point3D &) const;
};

Point3D Point3D::cross(const Point3D rightVec) const
{
	return Point3D((y * rightVec.z) - (z * rightVec.y),
						(z * rightVec.x) - (x * rightVec.z),
						(x * rightVec.y) - (y * rightVec.x));
}

float Point3D::angle(Point3D rightVec)
{
	Point3D tempVec = normalized();
	float temp = tempVec.dot(rightVec.normalized());
	return float(acos(temp));
}

Point3D Point3D::operator+(Point3D rightVec) const
{
	return Point3D(*this) += rightVec;
}

Point3D Point3D::operator-(Point3D rightVec) const
{
	return Point3D(*this) -= rightVec;
}

Point3D Point3D::operator*(float scalar) const
{
	return Point3D(*this) *= scalar;
}

Point3D Point3D::operator/(float scalar) const
{
	return Point3D(*this) /= scalar;
}

Point3D &Point3D::operator+=(Point3D rightVec)
{
	x += rightVec.x;
	y += rightVec.y;
	z += rightVec.z;
	return *this;
}

Point3D &Point3D::operator-=(Point3D rightVec)
{
	x -= rightVec.x;
	y -= rightVec.y;
	z -= rightVec.z;
	return *this;
}

Point3D &Point3D::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Point3D &Point3D::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Point3D &Point3D::operator=(Point3D rightVec)
{
	x = rightVec.x;
	y = rightVec.y;
	z = rightVec.z;
	return *this;
}

bool Point3D::operator==(const Point3D &rightVec) const
{
	return (x == rightVec.x &&
			y == rightVec.y &&
			z == rightVec.z);
}

bool Point3D::operator!=(const Point3D &rightVec) const
{
	return !operator==(rightVec);
}

std::ostream &operator<<(std::ostream &sout, const Point3D &p)
{
	sout << "(" << p.x << "," << p.y << "," << p.z << ")";
	return sout;
}
std::istream &	operator>>(std::istream &sin, Point3D &p)
{
	sin >> p.x >> p.y >> p.z;
	return sin;
}
Point3D Point3D::rotate(double A, const Point3D &r)
{
	A = DEGtoRED(A);
	Point3D l = *this;
	Point3D u = r.cross(l);
	return u * sin(A) + l * cos(A);
}

Point3D intersectPoint(Point3D rayVector, Point3D rayPoint, Point3D planeNormal, Point3D planePoint) {
	Point3D diff = rayPoint - planePoint;
	double prod1 = diff.dot(planeNormal);
	double prod2 = rayVector.dot(planeNormal);
	double prod3 = prod1 / prod2;
	return rayPoint - rayVector * prod3;
}

#endif
