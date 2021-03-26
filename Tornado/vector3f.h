#pragma once
#include <cmath>

#define M_PI 3.14159265358979323846

class vector3f
{
public:
	//constructors
	vector3f(void) : x(0.0f), y(0.0f), z(0.0f)
	{
	}

	vector3f(float newX, float newY, float newZ)
		: x(newX), y(newY), z(newZ)
	{
	}

	vector3f(const float * rhs) : x(*rhs), y(*(rhs + 1)), z(*(rhs + 2))
	{
	}

	vector3f(const vector3f & rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
	{
	}

	~vector3f() {}	//empty

	void Set(float newX, float newY, float newZ)
	{
		x = newX;	y = newY;	z = newZ;
	}

	//setters and getters
	void SetX(float newX) { x = newX; }
	void SetY(float newY) { y = newY; }
	void SetZ(float newZ) { z = newZ; }

	float GetX() const { return x; }	
	float GetY() const { return y; }	
	float GetZ() const { return z; }		

	void LoadZero(void)
	{
		x = 0.0f; y = 0.0f; z = 0.0f;
	}

	void LoadOne(void)
	{
		x = 1.0f; y = 1.0f; z = 1.0f;
	}

	float GetLength() const
	{
		return (float)sqrt((x*x) + (y*y) + (z*z));
	}

	float getAngleY();

	//rotations
	void RotateX(double angle);
	vector3f GetRotatedX(double angle) const;
	void RotateY(double angle);
	vector3f GetRotatedY(double angle) const;
	void RotateZ(double angle);
	vector3f GetRotatedZ(double angle) const;

	//Add, subtract
	void Add(const vector3f & v2, vector3f & result)
	{
		result.x = x + v2.x;
		result.y = y + v2.y;
		result.z = z + v2.z;
	}

	void Subtract(const vector3f & v2, vector3f & result)
	{
		result.x = x - v2.x;
		result.y = y - v2.y;
		result.z = z - v2.z;
	}

	vector3f& operator=(const vector3f & right)
	{
		if (&right != this)
		{
			x = right.x;
			y = right.y;
			z = right.z;
		}
		return *this;
	}

	//self-add etc
	void operator+=(const vector3f & rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z;
	}

	void operator-=(const vector3f & rhs)
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
	}

	//multiply by a float, eg 3*v
	friend vector3f operator*(float scaleFactor, const vector3f & rhs)
	{
		return rhs*scaleFactor;
	}

	vector3f operator*(const float rhs) const
	{
		return vector3f(x*rhs, y*rhs, z*rhs);
	}

	//cast to pointer to float for glVertex4fv etc
	operator float* () const { return (float*) this; }
	operator const float* () const { return (const float*) this; }

	//member variables
	float x;
	float y;
	float z;
};

