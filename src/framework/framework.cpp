#include "framework.h"

#include <cmath> //for sqrt (square root) function
#include <math.h> //atan2
#include <cstring>

#define M_PI_2 1.57079632679489661923


// Color
const Color Color::BLACK(0,0,0);
const Color Color::WHITE(255,255,255);
const Color Color::GRAY(64,64,64);
const Color Color::RED(255,0,0);
const Color Color::GREEN(0,255,0);
const Color Color::BLUE(0,0,255);
const Color Color::YELLOW(255,255,0);
const Color Color::CYAN(0,255,255);
const Color Color::PURPLE(255,0,255);

void Color::operator = (const Vector3& v)
{
	r = clamp( (unsigned char)v.x,0,255);
	g = clamp( (unsigned char)v.y,0,255);
	b = clamp( (unsigned char)v.z,0,255);
}

//**************************************
float Vector2::Distance(const Vector2& v)
{
	return (v - *this).length();
}

float Vector2::Dot( const Vector2& v )
{
	return x * v.x + y * v.y;
}

void Vector2::Clamp(float min, float max)
{
	x = clamp(x, min, max);
	y = clamp(y, min, max);
}

float Vector2::Perpdot( const Vector2& v )
{
	return y * v.x + -x * v.y;
}

void Vector2::Random(float range)
{
	//rand returns a value between 0 and RAND_MAX
	x = (rand() / (float)RAND_MAX) * 2 * range - range; //value between -range and range
	y = (rand() / (float)RAND_MAX) * 2 * range - range; //value between -range and range
}


Vector2 operator * (const Vector2& a, float v) { return Vector2(a.x * v, a.y * v); }
Vector2 operator / (const Vector2& a, float v) { return Vector2(a.x / v, a.y / v); }
Vector2 operator + (const Vector2& a, const Vector2& b) { return Vector2(a.x + b.x, a.y + b.y); }
Vector2 operator - (const Vector2& a, const Vector2& b) { return Vector2(a.x - b.x, a.y - b.y); }
Vector2 operator * (const Vector2& a, const Vector2& b) { return Vector2(a.x * b.x, a.y * b.y); }
Vector2 operator / (const Vector2& a, const Vector2& b) { return Vector2(a.x / b.x, a.y / b.y); }

// **************************************

// Vector3
const Vector3 Vector3::UP(0, 1, 0);
const Vector3 Vector3::DOWN(0, -1, 0);
const Vector3 Vector3::RIGHT(1, 0, 0);
const Vector3 Vector3::LEFT(-1, 0, 0);

float Vector3::Length()
{
	return sqrt(x*x + y*y + z*z);
}

float Vector3::Length() const
{
	return sqrt(x*x + y*y + z*z);
}

Vector3& Vector3::Normalize()
{
	float len = Length();
	x /= len;
	y /= len;
	z /= len;
	return *this;
}

float Vector3::Distance(const Vector3& v) const
{
	return (v - *this).Length();
}

Vector3 Vector3::Cross( const Vector3& b ) const
{
	return Vector3(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
}

float Vector3::Dot( const Vector3& v ) const
{
	return x*v.x + y*v.y + z*v.z;
}

void Vector3::Random(float range)
{
	//rand returns a value between 0 and RAND_MAX
	x = (rand() / (float)RAND_MAX) * 2 * range - range; //value between -range and range
	y = (rand() / (float)RAND_MAX) * 2 * range - range; //value between -range and range
	z = (rand() / (float)RAND_MAX) * 2 * range - range; //value between -range and range
}

void Vector3::Random(Vector3 range)
{
	//rand returns a value between 0 and RAND_MAX
	x = (rand() / (float)RAND_MAX) * 2 * range.x - range.x; //value between -range and range
	y = (rand() / (float)RAND_MAX) * 2 * range.y - range.y; //value between -range and range
	z = (rand() / (float)RAND_MAX) * 2 * range.z - range.z; //value between -range and range
}

void Vector3::Clamp(float min, float max)
{
	x = clamp(x, min, max);
	y = clamp(y, min, max);
	z = clamp(z, min, max);
}

//*********************************
Matrix44::Matrix44()
{
	SetIdentity();
}

Matrix44::Matrix44(const float* v)
{
	memcpy(m, v, sizeof(float) * 16);
}


void Matrix44::Set(
	float r1c1, float r1c2, float r1c3, float r1c4,
	float r2c1, float r2c2, float r2c3, float r2c4,
	float r3c1, float r3c2, float r3c3, float r3c4,
	float r4c1, float r4c2, float r4c3, float r4c4
) {
	m[0] = r1c1;	m[4] = r1c2;	m[8] = r1c3;	m[12] = r1c4;
	m[1] = r2c1;	m[5] = r2c2;	m[9] = r2c3;	m[13] = r2c4;
	m[2] = r3c1;	m[6] = r3c2;	m[10] = r3c3;	m[14] = r3c4;
	m[3] = r4c1;	m[7] = r4c2;	m[11] = r4c3;	m[15] = r4c4;
}

void Matrix44::Clear()
{
    std::memset(m, 0, 16*sizeof(float));
}

void Matrix44::SetIdentity()
{
	m[0]=1; m[4]=0; m[8]=0; m[12]=0;
	m[1]=0; m[5]=1; m[9]=0; m[13]=0;
	m[2]=0; m[6]=0; m[10]=1; m[14]=0;
	m[3]=0; m[7]=0; m[11]=0; m[15]=1;
}

void Matrix44::Transpose()
{
   std::swap(m[1],m[4]); std::swap(m[2],m[8]); std::swap(m[3],m[12]);
   std::swap(m[6],m[9]); std::swap(m[7],m[13]); std::swap(m[11],m[14]);
}

void Matrix44::Translate(float x, float y, float z)
{
	Matrix44 T;
	T.SetTranslation(x, y, z);
	*this = T * (*this);
}

void Matrix44::Rotate( float angle_in_rad, const Vector3& axis )
{
	Matrix44 R;
	R.SetRotation(angle_in_rad, axis);
	*this = R * (*this);
}

Vector3 Matrix44::RotateVector(const Vector3& v)
{
	Matrix44 temp = *this;
	temp.m[12] = 0.0;
	temp.m[13] = 0.0;
	temp.m[14] = 0.0;
	return temp * v;
}

void Matrix44::TranslateLocal(float x, float y, float z)
{
	Matrix44 T;
	T.SetTranslation(x, y, z);
	*this = *this * T;
}

void Matrix44::RotateLocal( float angle_in_rad, const Vector3& axis )
{
	Matrix44 R;
	R.SetRotation(angle_in_rad, axis);
	*this = *this * R;
}

//To create a traslation matrix
void Matrix44::SetTranslation(float x, float y, float z)
{
	SetIdentity();
	m[12] = x;
	m[13] = y;
	m[14] = z;
}

//To create a rotation matrix
void Matrix44::SetRotation( float angle_in_rad, const Vector3& axis )
{
	Vector3 axis_n = axis;
	axis_n.Normalize();

	float c = cos( angle_in_rad );
	float s = sin( angle_in_rad );
	float t = 1 - c;

	// first column
	m[0] = t * axis.x * axis.x + c;
	m[1] = t * axis.x * axis.y + s * axis.z;
	m[2] = t * axis.x * axis.z - s * axis.y;
	m[3] = 0;

	// second column
	m[4] = t * axis.x * axis.y - s * axis.z;
	m[5] = t * axis.y * axis.y + c;
	m[6] = t * axis.y * axis.z + s * axis.x;
	m[7] = 0;

	// third column
	m[8] = t * axis.x * axis.z + s * axis.y;
	m[9] = t * axis.y * axis.z - s * axis.x;
	m[10] = t * axis.z * axis.z + c;
	m[11] = 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}

Matrix44 Matrix44::GetRotationOnly()
{
	Vector3 v;
	Matrix44 result; // constructor already sets identity
	
	// right
	v.Set(m[0], m[1], m[2]);
	v = v.Normalize();
	result.m[0] = v.x;
	result.m[1] = v.y;
	result.m[2] = v.z;
	
	// up
	v.Set(m[4], m[5], m[6]);
	v = v.Normalize();
	result.m[4] = v.x;
	result.m[5] = v.y;
	result.m[6] = v.z;
	
	// front
	v.Set(m[8], m[9], m[10]);
	v = v.Normalize();
	result.m[8] = v.x;
	result.m[9] = v.y;
	result.m[10] = v.z;
	
	return result;
}

bool Matrix44::GetXYZ(float* euler) const
{
	// see https://en.wikipedia.org/wiki/Euler_angles Tait-Bryan -> angles table -> XYZ row

	// Code adapted from www.geometrictools.com  (math column major)
	//	Matrix3<Real>::EulerResult Matrix3<Real>::ToEulerAnglesXYZ 
    // +-           -+   +-                                        -+
    // | r00 r01 r02 |   |  cy*cz           -cy*sz            sy    |
    // | r10 r11 r12 | = |  cz*sx*sy+cx*sz   cx*cz-sx*sy*sz  -cy*sx |
    // | r20 r21 r22 |   | -cx*cz*sy+sx*sz   cz*sx+cx*sy*sz   cx*cy |
    // +-           -+   +-                                        -+

    if (_31 < 1.0f)
    {
        if (_31 > - 1.0f)
        {
            // y_angle = asin(r02)
            // x_angle = atan2(-r12,r22)
            // z_angle = atan2(-r01,r00)
			euler[1] = asinf(_31);
			euler[0] = atan2f(-_32, _33);
			euler[2] = atan2f(-_21, _11);
			return true;

        }
        else
        {
            // y_angle = -pi/2
            // z_angle - x_angle = atan2(r10,r11)
            // WARNING.  The solution is not unique.  Choosing z_angle = 0.
            euler[1] = (float)-M_PI_2;
            euler[0] = -atan2f(_12,_22);
            euler[2] = 0.0f;
            return false;
        }
    }
    else
    {
        // y_angle = +pi/2
        // z_angle + x_angle = atan2(r10,r11)
        // WARNING.  The solutions is not unique.  Choosing z_angle = 0.
        euler[1] = (float)M_PI_2;
        euler[0] = atan2f(_12,_22);
        euler[2] = 0.0f;
    }
	return false;
}

//Multiply a matrix by another and returns the result
Matrix44 Matrix44::operator*(const Matrix44& matrix) const
{
	Matrix44 ret;

	unsigned int i,j,k;
	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			ret.M[i][j]=0.0;
			for (k=0;k<4;k++) 
				ret.M[i][j] += M[k][j] * matrix.M[i][k];
		}
	}

	return ret;
}

//it allows to add two vectors
Vector3 operator + (const Vector3& a, const Vector3& b) 
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z );
}

//it allows to add two vectors
Vector3 operator - (const Vector3& a, const Vector3& b) 
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z );
}

Vector3 operator * (const Vector3& a, float v) 
{
	return Vector3(a.x * v, a.y * v, a.z * v);
}

Vector3 operator / (const Vector3& a, float v)
{
	return Vector3(a.x / v, a.y / v, a.z / v);
}

Vector3 operator * (const Vector3& a, const Vector3& b)
{
	return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector3 operator / (const Vector3& a, const Vector3& b)
{
	return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
}

Vector4 operator * (const Matrix44& matrix, const Vector4& v)
{
	float x = matrix.m[0] * v.x + matrix.m[4] * v.y + matrix.m[8] * v.z + matrix.m[12] * v.w;
	float y = matrix.m[1] * v.x + matrix.m[5] * v.y + matrix.m[9] * v.z + matrix.m[13] * v.w;
	float z = matrix.m[2] * v.x + matrix.m[6] * v.y + matrix.m[10] * v.z + matrix.m[14] * v.w;
	float w = matrix.m[3] * v.x + matrix.m[7] * v.y + matrix.m[11] * v.z + matrix.m[15] * v.w;
	return Vector4(x, y, z, w);
}

//Multiplies a vector by a matrix and returns the new vector ( assumes v4 = (v.x, v.y, v.z, 1) )
Vector3 operator * (const Matrix44& matrix, const Vector3& v)
{   
   float x = matrix.m[0] * v.x + matrix.m[4] * v.y + matrix.m[8] * v.z + matrix.m[12]; 
   float y = matrix.m[1] * v.x + matrix.m[5] * v.y + matrix.m[9] * v.z + matrix.m[13]; 
   float z = matrix.m[2] * v.x + matrix.m[6] * v.y + matrix.m[10] * v.z + matrix.m[14];
   return Vector3(x,y,z);
}

void Matrix44::SetUpAndOrthonormalize(Vector3 up)
{
	up.Normalize();

	//orthonormalize
	Vector3 right,front;
	right = RightVector().Normalize();

	if ( abs(right.Dot( up )) < 0.99998 )
	{
		front = right.Cross( up );
		right = up.Cross( front );
	}
	else
	{
		right = up.Cross( FrontVector().Normalize() );
		front = right.Cross( up );
	}

	right.Normalize();
	front.Normalize();

	m[0] = right.x;
	m[1] = right.y;
	m[2] = right.z;

	m[4] = up.x;
	m[5] = up.y;
	m[6] = up.z;

	m[8] = front.x;
	m[9] = front.y;
	m[10] = front.z;

}

void Matrix44::SetFrontAndOrthonormalize(Vector3 front)
{
	front.Normalize();

	//orthonormalize
	Vector3 right,up;
	right = RightVector().Normalize();

	if ( abs(right.Dot( front )) < 0.99998 )
	{
		up = front.Cross(right);
		right = up.Cross(front);
	}
	else
	{
		right = TopVector().Cross(front);
		up = front.Cross(right);
	}

	right.Normalize();
	up.Normalize();

	m[0] = right.x;
	m[1] = right.y;
	m[2] = right.z;

	m[4] = up.x;
	m[5] = up.y;
	m[6] = up.z;

	m[8] = front.x;
	m[9] = front.y;
	m[10] = front.z;
	
}

bool Matrix44::Inverse()
{
	// Guassian elimination
	// this code is meant for MemoryRowMajor
	// this works for both ( MemoryRowMajor ) and ( MemoryColMajor ) systems
	//							inv(A)			trans(inv(trans(A)) = inv(A)
	// note: inv(trans(A)) = trans(inv(A))

   unsigned int i, j, k, swap;
   float t;
   Matrix44 temp, final;
   final.SetIdentity();

   temp = (*this);

   unsigned int m,n;
   m = n = 4;
	
   for (i = 0; i < m; i++)
   {
      // Look for largest element in column

      swap = i;
      for (j = i + 1; j < m; j++)// m or n
	  {
		 if ( fabs(temp.M[j][i]) > fabs( temp.M[swap][i]) )
            swap = j;
	  }
   
      if (swap != i)
      {
         // Swap rows.
         for (k = 0; k < n; k++)
         {
			 std::swap( temp.M[i][k],temp.M[swap][k]);
			 std::swap( final.M[i][k], final.M[swap][k]);
         }
      }

      // No non-zero pivot.  The CMatrix is singular, which shouldn't
      // happen.  This means the user gave us a bad CMatrix.
	  
#define MATRIX_SINGULAR_THRESHOLD 0.00001 //change this if you experience problems with matrices
      if ( fabsf(temp.M[i][i]) <= MATRIX_SINGULAR_THRESHOLD)
	  {
		  final.SetIdentity();
         return false;
	  }
#undef MATRIX_SINGULAR_THRESHOLD

      t = 1.0f/temp.M[i][i];

      for (k = 0; k < n; k++)//m or n
      {
         temp.M[i][k] *= t;
         final.M[i][k] *= t;
      }

      for (j = 0; j < m; j++) // m or n
      {
         if (j != i)
         {
            t = temp.M[j][i];
            for (k = 0; k < n; k++)//m or n
            {
               temp.M[j][k] -= (temp.M[i][k] * t);
               final.M[j][k] -= (final.M[i][k] * t);
            }
         }
      }
   }

   *this = final;

   return true;
}

float ComputeSignedAngle( Vector2 a, Vector2 b)
{
	a.normalize();
	b.normalize();
	return atan2(a.Perpdot(b), a.Dot(b) );
}

Vector3 RayPlaneCollision( const Vector3& plane_pos, const Vector3& plane_normal, const Vector3& ray_origin, const Vector3& ray_dir )
{
    float D = plane_pos.Dot(plane_normal);
	float numer = plane_normal.Dot(ray_origin) + D;
	float denom = plane_normal.Dot(ray_dir);
	float t = -(numer / denom);
	return ray_origin + ray_dir * t;
}
