/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	Here we define all the mathematical classes like Vector3, Matrix44 and some extra useful geometrical functions
*/

#pragma once

#include <vector>
#include <cmath>
#include <random>

#ifndef PI
	#define PI 3.14159265359
#endif
#define DEG2RAD 0.0174532925f

// Clamp a value 'x' between 'a' and 'b'
inline float clamp(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }
inline unsigned int clamp(unsigned int x, unsigned int a, unsigned int b) { return x < a ? a : (x > b ? b : x); }
inline unsigned char clamp(unsigned char x, unsigned char a, unsigned char b) { return x < a ? a : (x > b ? b : x); }

class Vector3;

// Color class to store colors in unsigned byte
class Color
{
public:
	union
	{
		struct { unsigned char r;
				 unsigned char g;
				 unsigned char b; };
		unsigned char v[3];
	};
	Color() { r = g = b = 0; }
	Color(float r, float g, float b) { this->r = (unsigned char)r; this->g = (unsigned char)g; this->b = (unsigned char)b; }
	void operator = (const Vector3& v);

	void Set(float r, float g, float b) { this->r = (unsigned char)clamp(r,0.0,255.0); this->g = (unsigned char)clamp(g,0.0,255.0); this->b = (unsigned char)clamp(b,0.0,255.0); }
	void Random() { r = rand() % 255; g = rand() % 255; b = rand() % 255; }

	Color operator * (float v) { return Color((unsigned char)(r*v), (unsigned char)(g*v), (unsigned char)(b*v)); }
	void operator *= (float v) { r = (unsigned char)(r * v); g = (unsigned char)(g * v); b = (unsigned char)(b * v); }
	Color operator / (float v) { return Color((unsigned char)(r/v), (unsigned char)(g/v), (unsigned char)(b/v)); }
	void operator /= (float v) { r = (unsigned char)(r / v); g = (unsigned char)(g / v); b = (unsigned char)(b / v); }
	Color operator + (const Color& v) { return Color((float)(r+v.r), (float)(g+v.g), (float)(b+v.b) ); }
	void operator += (const Color& v) { r += v.r; g += v.g; b += v.b; }
	Color operator - (const Color& v) { return Color((float)(r-v.r), (float)(g-v.g), (float)(b-v.b)); }
	void operator -= (const Color& v) { r -= v.r; g -= v.g; b -= v.b; }
	Color operator * (const Color& v) { return Color((float)(r * v.r), (float)(g * v.g), (float)(b * v.b)); }
	void operator *= (const Color& v) { r *= v.r; g *= v.g; b *= v.b; }

	//some colors to help
	static const Color WHITE;
	static const Color BLACK;
	static const Color GRAY;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color CYAN;
	static const Color YELLOW;
	static const Color PURPLE;
};

inline Color operator * (const Color& c, float v) { return Color((unsigned char)(c.r*v), (unsigned char)(c.g*v), (unsigned char)(c.b*v)); }
inline Color operator * (float v, const Color& c) { return Color((unsigned char)(c.r*v), (unsigned char)(c.g*v), (unsigned char)(c.b*v)); }
//*********************************

class Vector2
{
public:
	union
	{
		struct { float x, y; };
		float value[2];
	};

	Vector2() { x = y = 0.0f; }
	Vector2(float x, float y) { this->x = x; this->y = y; }

	float length() { return sqrt(x * x + y * y); }
	float length() const { return sqrt(x * x + y * y); }

	float Dot(const Vector2& v);
	float Perpdot(const Vector2& v);

	void set(float x, float y) { this->x = x; this->y = y; }

	Vector2& normalize() { *this *= 1/(float)length(); return *this; }

	float Distance(const Vector2& v);
	void Random(float range);
	void Clamp(float min, float max);

	void operator *= (float v) { x *= v; y *= v; }
	void operator *= (const Vector2& v) { x *= v.x; y *= v.y; }
	void operator += (const Vector2& v) { x += v.x; y += v.y; }
	void operator -= (const Vector2& v) { x -= v.x; y -= v.y; }
};

Vector2 operator * (const Vector2& a, float v);
Vector2 operator / (const Vector2& a, float v);
Vector2 operator + (const Vector2& a, const Vector2& b);
Vector2 operator - (const Vector2& a, const Vector2& b);
Vector2 operator * (const Vector2& a, const Vector2& b);
Vector2 operator / (const Vector2& a, const Vector2& b);

inline float distance(const Vector2& a, const Vector2& b) { return (float)(a - b).length(); }
inline float distance(float x, float y, float x2, float y2) { return sqrtf((x - x2) * (x - x2) + (y - y2) * (y - y2)); }

class Vector3
{
public:
	union
	{
		struct { float x,y,z; };
		float v[3];
	};

	Vector3() { x = y = z = 0.0f; }
	Vector3(float v) { this->x = v; this->y = v; this->z = v; }
	Vector3(float x, float y, float z) { this->x = x; this->y = y; this->z = z;	}

	float Length();
	float Length() const;

	void Set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

	Vector3& Normalize();
	Vector3 Cross( const Vector3& v ) const;
	Vector2 GetVector2() { return Vector2(x, y); }

	void Random(float range);
	void Random(Vector3 range);
	void Clamp(float min, float max);

	float Distance(const Vector3& v) const;
	float Dot( const Vector3& v ) const;

	static const Vector3 UP;
	static const Vector3 DOWN;
	static const Vector3 RIGHT;
	static const Vector3 LEFT;
};


class Vector4
{
public:
	union
	{
		struct { float x,y,z,w; };
		float v[4];
	};

	Vector4() { x = y = z = w = 0.0f; }
	Vector4(float x, float y, float z, float w ) { this->x = x; this->y = y; this->z = z; this->w = w; }
	void Set(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; }

	Vector3 GetVector3() { return Vector3(x,y,z); }
};

//****************************

class Matrix44
{
	public:

		// Memory column-major, math column-vectors. (M * p)
		union { // Allows to access the same var using different ways
			struct
			{
				float        _11, _12, _13, _14;	// column 1 (left)
				float        _21, _22, _23, _24;	// column 2
				float        _31, _32, _33, _34;	// column 3
				float        _41, _42, _43, _44;	// column 4 (right)
			};
			float M[4][4]; // [column][row]
			float m[16];
		};

		Matrix44();
		Matrix44(const float* v);

		void Set(
			float r1c1, float r1c2, float r1c3, float r1c4, 
			float r2c1, float r2c2, float r2c3, float r2c4, 
			float r3c1, float r3c2, float r3c3, float r3c4, 
			float r4c1, float r4c2, float r4c3, float r4c4 
		);
		void Clear();
		void SetIdentity();
		void Transpose();

		// Get base vectors
		Vector3 RightVector() { return Vector3(m[0],m[1],m[2]); }
		Vector3 TopVector() { return Vector3(m[4],m[5],m[6]); }
		Vector3 FrontVector() { return Vector3(m[8],m[9],m[10]); }

		bool Inverse();
		void SetUpAndOrthonormalize(Vector3 up);
		void SetFrontAndOrthonormalize(Vector3 front);

		// does not accept negative scale
		Matrix44 GetRotationOnly();

		// Rotate (and Scale) only
		Vector3 RotateVector(const Vector3& v);

		// Transform using world coordinates
		void Translate(float x, float y, float z);
		void Rotate(float angle_in_rad, const Vector3& axis);

		// Transform using local coordinates
		void TranslateLocal(float x, float y, float z);
		void RotateLocal(float angle_in_rad, const Vector3& axis);

		// Create a transformation matrix from scratch
		void SetTranslation(float x, float y, float z);
		void SetRotation(float angle_in_rad, const Vector3& axis);

		// returns Euler angles [ X,Y,Z ] from PURE ROTATION MATRIX (unscaled). To reconstruct the matrix M = XYZ (math column-vector M*p)
		bool GetXYZ(float* euler) const;

		Matrix44 operator * (const Matrix44& matrix) const;
};

// Operators, they are our friends
// Matrix44 operator * ( const Matrix44& a, const Matrix44& b );
Vector3 operator * (const Matrix44& matrix, const Vector3& v);
Vector3 operator + (const Vector3& a, const Vector3& b);
Vector3 operator - (const Vector3& a, const Vector3& b);
Vector3 operator * (const Vector3& a, float v);
Vector3 operator / (const Vector3& a, float v);
Vector3 operator * (const Vector3& a, const Vector3& b);
Vector3 operator / (const Vector3& a, const Vector3& b);
Vector4 operator * (const Matrix44& matrix, const Vector4& v);

class Vector3u
{
public:
	union
	{
		struct { unsigned int x;
				 unsigned int y;
				 unsigned int z; };
		struct { unsigned int r;
				 unsigned int g;
				 unsigned int b; };
		unsigned int v[3];
	};
	Vector3u() { x = y = z = 0; }
	Vector3u(unsigned int x, unsigned int y, unsigned int z) { this->x = x; this->y = y; this->z = z; }

	void operator *= (float v) { x = (unsigned int)(x * v); y = (unsigned int)(y * v); z = (unsigned int)(z * v); }
	Vector3u operator / (float v) { return Vector3u((unsigned int)(r/v), (unsigned int)(g/v), (unsigned int)(b/v)); }
	void operator /= (float v) { x = (unsigned int)(x / v); y = (unsigned int)(y / v); z = (unsigned int)(z / v); }
	Vector3u operator + (const Vector3u& v) { return Vector3u(x+v.x, y+v.y, z+v.z); }
	void operator += (const Vector3u& v) { x += v.x; y += v.y; z += v.z; }
	Vector3u operator - (const Vector3u& v) { return Vector3u(x-v.x, y-v.y, z-v.z); }
	void operator -= (const Vector3u& v) { x -= v.x; y -= v.y; z -= v.z; }
};

inline Vector3u operator * (float v, const Vector3u& c) { return Vector3u((unsigned int)(c.x*v), (unsigned int)(c.y*v), (unsigned int)(c.z*v)); }

float ComputeSignedAngle( Vector2 a, Vector2 b);
Vector3 RayPlaneCollision( const Vector3& plane_pos, const Vector3& plane_normal, const Vector3& ray_origin, const Vector3& ray_dir );
