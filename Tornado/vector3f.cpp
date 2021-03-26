#include "vector3f.h"

void vector3f::RotateX(double angle)
{
	(*this) = GetRotatedX(angle);
}

void vector3f::RotateY(double angle)
{
	(*this) = GetRotatedY(angle);
}

void vector3f::RotateZ(double angle)
{
	(*this) = GetRotatedZ(angle);
}

vector3f vector3f::GetRotatedX(double angle) const
{
	if (angle == 0.0)
		return (*this);

	float sinAngle = (float)sin(M_PI*angle / 180);
	float cosAngle = (float)cos(M_PI*angle / 180);

	return vector3f(x,
					y*cosAngle - z*sinAngle,
					y*sinAngle + z*cosAngle);
}

vector3f vector3f::GetRotatedY(double angle) const
{
	if (angle == 0.0)
		return (*this);

	float r = this->GetLength();

	float sinAngle = (float)sin(M_PI*angle / 180);
	float cosAngle = (float)cos(M_PI*angle / 180);

	float x = r * cosAngle;
	float z = r * sinAngle;
	
	return vector3f(x,
					y,
					z);
}

float vector3f::getAngleY()
{
	float r = this->GetLength();

	float cosAngle = this->x / r;

	float angle = acosf(cosAngle);

	angle *= 180 / M_PI;

	return angle;
}

vector3f vector3f::GetRotatedZ(double angle) const
{
	if (angle == 0.0)
		return (*this);

	float sinAngle = (float)sin(M_PI*angle / 180);
	float cosAngle = (float)cos(M_PI*angle / 180);

	return vector3f(x*cosAngle - y*sinAngle,
					x*sinAngle + y*cosAngle,
					z);
}