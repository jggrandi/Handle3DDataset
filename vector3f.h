//-----------------------------------------------------------------------------
//           Name: vector3f.h
//         Author: Kevin Harris
//  Last Modified: 02/01/05
//    Description: OpenGL compatible utility class for a 3D vector of floats
//                 NOTE: This class has been left unoptimized for readability.
//-----------------------------------------------------------------------------

#ifndef _VECTOR3F_H_
#define _VECTOR3F_H_

#include <cmath>

class vector3f
{
public:

	float x;
	float y;
	float z;

	vector3f(void)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	vector3f(float x_, float y_, float z_);
	void set(float x_, float y_, float z_);
	float length(void);
	void normalize(void);

	// Static utility methods
	static float distance(const vector3f &v1, const vector3f &v2);
	static float dotProduct(const vector3f &v1,  const vector3f &v2 );
	static vector3f crossProduct(const vector3f &v1, const vector3f &v2);
	void projectW1(vector3f &v1);
	vector3f sub(vector3f a, vector3f b); 
	vector3f soma(vector3f a, vector3f b);
	void scale(float scalar, vector3f &a); 
	// Operators...
	vector3f operator + (const vector3f &other);
	vector3f operator - (const vector3f &other);
	vector3f operator * (const vector3f &other);
	vector3f operator / (const vector3f &other);
	vector3f operator / (const float 	scalar);
	vector3f operator * (const float 	scalar);
	friend vector3f operator * (const float scalar, const vector3f &other);

	vector3f& operator = (const vector3f &other);
	vector3f& operator += (const vector3f &other);
	vector3f& operator -= (const vector3f &other);

	vector3f operator + (void) const;
	vector3f operator - (void) const;
};

inline vector3f::vector3f( float x_, float y_, float z_ )
{
	x = x_;
	y = y_;
	z = z_;
}

inline void vector3f::set( float x_, float y_, float z_ )
{
	x = x_;
	y = y_;
	z = z_;
}

inline float vector3f::length( void )
{
	return( (float)sqrt( x * x + y * y + z * z ) );
}

inline void vector3f::normalize( void )
{
	float fLength = length();

	x = x / fLength;
	y = y / fLength;
	z = z / fLength;
}

// Static utility methods...

inline static float distance( const vector3f &v1,  const vector3f &v2  )
{
	float dx = v1.x - v2.x;
	float dy = v1.y - v2.y;
	float dz = v1.z - v2.z;

	return (float)sqrt( dx * dx + dy * dy + dz * dz );
}

inline static float dotProduct( const vector3f &v1,  const vector3f &v2 )
{
	return( v1.x * v2.x + v1.y * v2.y + v1.z * v2.z  );
}

inline static vector3f crossProduct( const vector3f &v1,  const vector3f &v2 )
{
	vector3f vCrossProduct;

	vCrossProduct.x =  v1.y * v2.z - v1.z * v2.y;
	vCrossProduct.y = -v1.x * v2.z + v1.z * v2.x;
	vCrossProduct.z =  v1.x * v2.y - v1.y * v2.x;

	return vCrossProduct;
}

inline void projectW1(vector3f &v1)
{
	v1.x = v1.x/v1.z;
	v1.y = v1.y/v1.z;
	v1.z = 1.0f;
}

inline vector3f sub(vector3f a, vector3f b) 
{
	vector3f vResult;

	vResult.x = a.x - b.x;
	vResult.y = a.y - b.y;
	vResult.z = a.z - b.z;

	return vResult;
}

inline vector3f soma(vector3f a, vector3f b) 
{
	vector3f vResult;

	vResult.x = a.x + b.x;
	vResult.y = a.y + b.y;
	vResult.z = a.z + b.z;

	return vResult;
}

inline void scale(float scalar, vector3f &a) 
{
	a.x = scalar*a.x;
	a.y = scalar*a.y;
	a.z = scalar*a.z;
}


// Operators...

inline vector3f vector3f::operator + ( const vector3f &other )
{
	vector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x + other.x;
	vResult.y = y + other.y;
	vResult.z = z + other.z;

	return vResult;
}

inline vector3f vector3f::operator + ( void ) const
{
	return *this;
}

inline vector3f vector3f::operator - ( const vector3f &other )
{
	vector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x - other.x;
	vResult.y = y - other.y;
	vResult.z = z - other.z;

	return vResult;
}

inline vector3f vector3f::operator - ( void ) const
{
	vector3f vResult(-x, -y, -z);

	return vResult;
}

inline vector3f vector3f::operator * ( const vector3f &other )
{
	vector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x * other.x;
	vResult.y = y * other.y;
	vResult.z = z * other.z;

	return vResult;
}

inline vector3f vector3f::operator * ( const float scalar )
{
	vector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x * scalar;
	vResult.y = y * scalar;
	vResult.z = z * scalar;

	return vResult;
}

inline vector3f operator * ( const float scalar, const vector3f &other )
{
	vector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = other.x * scalar;
	vResult.y = other.y * scalar;
	vResult.z = other.z * scalar;

	return vResult;
}

inline vector3f vector3f::operator / ( const vector3f &other )
{
	vector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x / other.x;
	vResult.y = y / other.y;
	vResult.z = z / other.z;

	return vResult;
}

inline vector3f vector3f::operator / ( const float scalar )
{
	vector3f vResult(0.0f, 0.0f, 0.0f);

	vResult.x = x / scalar;
	vResult.y = y / scalar;
	vResult.z = z / scalar;

	return vResult;
}


inline vector3f& vector3f::operator = ( const vector3f &other )
{
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
}

inline vector3f& vector3f::operator += ( const vector3f &other )
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

inline vector3f& vector3f::operator -= ( const vector3f &other )
{
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

#endif // _VECTOR3F_H_
