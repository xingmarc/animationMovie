#pragma once
#include <cmath>
const double PI = 4 * atan(1);
const GLfloat  DegreesToRadians = PI / 180;
	
struct vec2			////////////////////////////////////////////////////////////////////////////////////////// vec2
{
    GLfloat  x, y;

    vec2( GLfloat s = GLfloat(0) ) :	x(s),	y(s)	{}
    vec2( GLfloat x, GLfloat y )	 :	x(x),	y(y)	{}
	vec2( const vec2& v )			 :	x(v.x), y(v.y)	{}

		  GLfloat& operator [] ( int i )	   { return *(&x + i); }
    const GLfloat  operator [] ( int i ) const { return *(&x + i); }

    vec2 operator - ()					const 	{ return vec2( -x, -y ); }
    vec2 operator + ( const vec2& v )	const	{ return vec2( x + v.x, y + v.y ); }
    vec2 operator - ( const vec2& v )	const	{ return vec2( x - v.x, y - v.y ); }
    vec2 operator * ( const GLfloat s ) const	{ return vec2( s*x, s*y ); }
    vec2 operator * ( const vec2& v )	const	{ return vec2( x*v.x, y*v.y ); }

    friend vec2 operator * ( const GLfloat s, const vec2& v )	{ return v * s; }

    vec2 operator / ( const GLfloat s ) const 	{ return *this * (GLfloat(1) / s);    }

    vec2& operator += ( const vec2& v )		{ x += v.x;  y += v.y;   return *this; }
    vec2& operator -= ( const vec2& v )		{ x -= v.x;  y -= v.y;  return *this; }
    vec2& operator *= ( const GLfloat s )	{ x *= s;  y *= s;   return *this; }
    vec2& operator *= ( const vec2& v )		{ x *= v.x;  y *= v.y; return *this; }
    vec2& operator /= ( const GLfloat s )   { return *this *= GLfloat(1) / s;  }
	
    friend std::ostream& operator << ( std::ostream& os, const vec2& v ) { return os << "( " << v.x << ", " << v.y <<  " )";    }
    friend std::istream& operator >> ( std::istream& is, vec2& v )		 { return is >> v.x >> v.y ; }

    operator const GLfloat* () const	{ return static_cast<const GLfloat*>( &x ); }
    operator GLfloat* () 				{ return static_cast<      GLfloat*>( &x ); }
};

inline GLfloat dot( const vec2& u, const vec2& v ) {    return u.x * v.x + u.y * v.y;	}
inline GLfloat length( const vec2& v ) {    return std::sqrt( dot(v,v) );	}
inline vec2 normalize( const vec2& v ) {    return v / length(v);	}


struct vec3			////////////////////////////////////////////////////////////////////////////////////////// vec3
{
	GLfloat  x, y, z;

    vec3( GLfloat s = GLfloat(0) )		:	x(s),   y(s),   z(s) {}
    vec3( GLfloat x, GLfloat y, GLfloat z ) :	x(x),   y(y),   z(z) {}
	vec3( const vec3& v )					:	x(v.x), y(v.y), z(v.z)	{}
	vec3( const vec2& v, const float f )	:	x(v.x), y(v.y), z(f)	{}

		  GLfloat& operator [] ( int i )	   { return *(&x + i); }
    const GLfloat  operator [] ( int i ) const { return *(&x + i); }

    vec3 operator - () const					{ return vec3( -x, -y, -z ); }
    vec3 operator + ( const vec3& v )	const	{ return vec3( x + v.x, y + v.y, z + v.z ); }
    vec3 operator - ( const vec3& v )	const	{ return vec3( x - v.x, y - v.y, z - v.z ); }
    vec3 operator * ( const GLfloat s ) const	{ return vec3( s*x, s*y, s*z ); }
    vec3 operator * ( const vec3& v )	const	{ return vec3( x*v.x, y*v.y, z*v.z ); }
    friend vec3 operator * ( const GLfloat s, const vec3& v )	{ return v * s; }
    vec3 operator / ( const GLfloat s ) const {	return *this * (GLfloat(1) / s);    }

    vec3& operator += ( const vec3& v )		{ x += v.x;  y += v.y;  z += v.z;  return *this; }
    vec3& operator -= ( const vec3& v )		{ x -= v.x;  y -= v.y;  z -= v.z;  return *this; }
    vec3& operator *= ( const GLfloat s )	{ x *= s;	 y *= s;  z *= s;  return *this; }
    vec3& operator *= ( const vec3& v )		{ x *= v.x;  y *= v.y;  z *= v.z;  return *this; }
    vec3& operator /= ( const GLfloat s )	{	return *this *= GLfloat(1) / s;    }
	
    friend std::ostream& operator << ( std::ostream& os, const vec3& v )	{	return os << "( " << v.x << ", " << v.y << ", " << v.z <<  " )";    }
    friend std::istream& operator >> ( std::istream& is, vec3& v )			{ return is >> v.x >> v.y >> v.z ; }

    operator const GLfloat* () const	{ return static_cast<const GLfloat*>( &x ); }
    operator GLfloat* ()				{ return static_cast<	   GLfloat*>( &x ); }
};

inline GLfloat dot( const vec3& u, const vec3& v )	{    return u.x*v.x + u.y*v.y + u.z*v.z ;	}
inline GLfloat length( const vec3& v )				{    return std::sqrt( dot(v,v) );	}
inline vec3 normalize( const vec3& v )				{    return v / length(v);	}
inline vec3 cross(const vec3& a, const vec3& b )	{    return vec3( a.y * b.z - a.z * b.y,		a.z * b.x - a.x * b.z,		a.x * b.y - a.y * b.x );	}


struct vec4			////////////////////////////////////////////////////////////////////////////////////////// vec4
{
    GLfloat  x, y, z, w;

    vec4( GLfloat s = GLfloat(0) )					:	x(s),   y(s),   z(s),   w(s)   {}
    vec4( GLfloat x, GLfloat y, GLfloat z, GLfloat w )	:	x(x),   y(y),   z(z),   w(w)   {}
    vec4( const vec4& v )								:	x(v.x), y(v.y), z(v.z), w(v.w) {}
    vec4( const vec3& v, const float w = 1 )			:	x(v.x), y(v.y), z(v.z), w(w)   {}
    vec4( const vec2& v, const float z, const float w ) :	x(v.x), y(v.y), z(z),   w(w)   {}

		  GLfloat& operator [] ( int i )	   { return *(&x + i); }
    const GLfloat  operator [] ( int i ) const { return *(&x + i); }

    vec4 operator - () const					{ return vec4( -x, -y, -z, -w ); }
    vec4 operator + ( const vec4& v ) const		{ return vec4( x + v.x, y + v.y, z + v.z, w + v.w ); }
    vec4 operator - ( const vec4& v ) const		{ return vec4( x - v.x, y - v.y, z - v.z, w - v.w ); }
    vec4 operator * ( const GLfloat s ) const	{ return vec4( s*x, s*y, s*z, s*w ); }
    vec4 operator * ( const vec4& v ) const		{ return vec4( x*v.x, y*v.y, z*v.z, w*v.z ); }
    friend vec4 operator * ( const GLfloat s, const vec4& v )	{ return v * s; }
    vec4 operator / ( const GLfloat s ) const	{ return *this * (GLfloat(1) / s);    }

    vec4& operator += ( const vec4& v )		{ x += v.x;  y += v.y;  z += v.z;  w += v.w;  return *this; }
    vec4& operator -= ( const vec4& v )		{ x -= v.x;  y -= v.y;  z -= v.z;  w -= v.w;  return *this; }
    vec4& operator *= ( const GLfloat s )	{ x *= s;  y *= s;  z *= s;  w *= s;  return *this; }
    vec4& operator *= ( const vec4& v )		{ x *= v.x, y *= v.y, z *= v.z, w *= v.w;  return *this; }
    vec4& operator /= ( const GLfloat s )	{ return *this *= GLfloat(1) / s; }

    friend std::ostream& operator << ( std::ostream& os, const vec4& v ) {	return os << "( " << v.x << ", " << v.y << ", " << v.z << ", " << v.w << " )";    }
    friend std::istream& operator >> ( std::istream& is, vec4& v )		 { return is >> v.x >> v.y >> v.z >> v.w; }

    operator const GLfloat* () const	{ return static_cast<const GLfloat*>( &x ); }
    operator	   GLfloat* ()			{ return static_cast<	   GLfloat*>( &x ); }
};

inline GLfloat dot( const vec4& u, const vec4& v)	{    return u.x*v.x + u.y*v.y + u.z*v.z + u.w+v.w; }
inline GLfloat length( const vec4& v )				{    return std::sqrt( dot(v,v) ); }
inline vec4 normalize( const vec4& v )				{    return v / length(v); }
inline vec3 cross(const vec4& a, const vec4& b )	{    return vec3( a.y * b.z - a.z * b.y,		 a.z * b.x - a.x * b.z,		 a.x * b.y - a.y * b.x );	}


class mat2 			////////////////////////////////////////////////////////////////////////////////////////// mat2
{
    vec2  _m[2];
public:
    mat2( const GLfloat d = GLfloat(1) )	{ _m[0].x = d;  _m[1].y = d;   }// Create a diagional matrix
    mat2( const vec2& a, const vec2& b )	{ _m[0] = a;  _m[1] = b;  }
    mat2( GLfloat m00, GLfloat m10, GLfloat m01, GLfloat m11 )	{ _m[0] = vec2( m00, m01 ); _m[1] = vec2( m10, m11 ); }
    mat2( const mat2& m ) {	if ( *this == m ) return;	_m[0] = m._m[0];  _m[1] = m._m[1]; }

		  vec2& operator [] ( int i )		{ return _m[i]; }
    const vec2& operator [] ( int i ) const { return _m[i]; }

    mat2 operator + ( const mat2& m )	const	{ return mat2( _m[0]+m[0], _m[1]+m[1] ); }
    mat2 operator - ( const mat2& m )	const	{ return mat2( _m[0]-m[0], _m[1]-m[1] ); }
    mat2 operator * ( const GLfloat s ) const 	{ return mat2( s*_m[0], s*_m[1] ); }
    mat2 operator / ( const GLfloat s ) const	{ return *this * (GLfloat(1) / s); }
    mat2 operator * ( const mat2& m )   const	{	mat2  a( 0 );	
													for ( int i = 0; i < 2; ++i ) 
														for ( int j = 0; j < 2; ++j )	
															for ( int k = 0; k < 2; ++k ) 
																a[i][j] += _m[i][k] * m[k][j];
													return a;	}

    friend mat2 operator * ( const GLfloat s, const mat2& m )	{ return m * s; }

    mat2& operator += ( const mat2& m )		{	_m[0] += m[0];  _m[1] += m[1];  return *this;    }
    mat2& operator -= ( const mat2& m )		{	_m[0] -= m[0];  _m[1] -= m[1];  return *this;    }
	mat2& operator *= ( const GLfloat s )	{	_m[0] *= s;		_m[1] *= s;		return *this;    }
    mat2& operator *= ( const mat2& m )		{	mat2  a( 0 );
												for ( int i = 0; i < 2; ++i )
													for ( int j = 0; j < 2; ++j )
														for ( int k = 0; k < 2; ++k )
															a[i][j] += _m[i][k] * m[k][j];
												return *this = a;	}
    
    mat2& operator /= ( const GLfloat s )		{	return *this *= (GLfloat(1) / s);    }
    vec2  operator *  ( const vec2& v ) const	{  	return vec2( _m[0][0]*v.x + _m[0][1]*v.y,		_m[1][0]*v.x + _m[1][1]*v.y );    }		// m * v
	
    friend std::ostream& operator << ( std::ostream& os, const mat2& m )	{ return os << '\n' << m[0] << '\n' << m[1] << '\n'; }
    friend std::istream& operator >> ( std::istream& is, mat2& m )			{ return is >> m._m[0] >> m._m[1] ; }


    operator const GLfloat* () const	{ return static_cast<const GLfloat*>( &_m[0].x ); }
    operator	   GLfloat* ()			{ return static_cast<	   GLfloat*>( &_m[0].x ); }
};

inline mat2 matrixCompMult( const mat2& A, const mat2& B ) {    return mat2( A[0][0]*B[0][0], A[0][1]*B[0][1],		 A[1][0]*B[1][0], A[1][1]*B[1][1] );}
inline mat2 transpose( const mat2& A ) {    return mat2( A[0][0], A[0][1],		 A[1][0], A[1][1] ); }


class mat3 			////////////////////////////////////////////////////////////////////////////////////////// mat3
{
    vec3  _m[3];
public:
    mat3( const GLfloat d = GLfloat(1) )				{ _m[0].x = d;  _m[1].y = d;  _m[2].z = d;   }	// Create a diagional matrix
    mat3( const vec3& a, const vec3& b, const vec3& c )	{ _m[0] = a;    _m[1] = b;    _m[2] = c;  }
    mat3( GLfloat m00, GLfloat m10, GLfloat m20,
		  GLfloat m01, GLfloat m11, GLfloat m21,
		  GLfloat m02, GLfloat m12, GLfloat m22 )	  {	 _m[0] = vec3( m00, m01, m02 );		_m[1] = vec3( m10, m11, m12 );	_m[2] = vec3( m20, m21, m22 );	}

    mat3( const mat3& m ) {	if ( *this == m ) return;	_m[0] = m._m[0];	_m[1] = m._m[1];	_m[2] = m._m[2];    } 
    vec3& operator [] ( int i ) { return _m[i]; }
    const vec3& operator [] ( int i )	const	{ return _m[i]; }
	mat3 operator + ( const mat3& m )	const	{ return mat3( _m[0]+m[0], _m[1]+m[1], _m[2]+m[2] ); }
    mat3 operator - ( const mat3& m )	const	{ return mat3( _m[0]-m[0], _m[1]-m[1], _m[2]-m[2] ); }
    mat3 operator * ( const GLfloat s ) const 	{ return mat3( s*_m[0], s*_m[1], s*_m[2] ); }
    mat3 operator / ( const GLfloat s ) const	{ return *this * (GLfloat(1) / s); }

    friend mat3 operator * ( const GLfloat s, const mat3& m )	{ return m * s; }
	
    mat3 operator * ( const mat3& m ) const		{	mat3  a( 0 );			
													for ( int i = 0; i < 3; ++i ) 
														for ( int j = 0; j < 3; ++j )	
															for ( int k = 0; k < 3; ++k ) 
																a[i][j] += _m[i][k] * m[k][j];
													return a;	}

    mat3& operator += ( const mat3& m )		{	_m[0] += m[0];  _m[1] += m[1];  _m[2] += m[2]; 	return *this;    }
    mat3& operator -= ( const mat3& m )		{	_m[0] -= m[0];  _m[1] -= m[1];  _m[2] -= m[2]; 	return *this;    }
    mat3& operator *= ( const GLfloat s )	{	_m[0] *= s;  _m[1] *= s;  _m[2] *= s; 	return *this;    }
    mat3& operator *= ( const mat3& m )		{	mat3  a( 0 );	
												for ( int i = 0; i < 3; ++i ) 
													for ( int j = 0; j < 3; ++j )	
														for ( int k = 0; k < 3; ++k ) 
															a[i][j] += _m[i][k] * m[k][j];
												return *this = a;	}										

    mat3& operator /= ( const GLfloat s ) {	return *this *= (GLfloat(1) / s);    }

    vec3 operator * ( const vec3& v ) const {  	return vec3( _m[0][0]*v.x + _m[0][1]*v.y + _m[0][2]*v.z, 
		_m[1][0]*v.x + _m[1][1]*v.y + _m[1][2]*v.z, _m[2][0]*v.x + _m[2][1]*v.y + _m[2][2]*v.z );    }	// m * v
	
	friend std::ostream& operator << ( std::ostream& os, const mat3& m ) {	return os << '\n' << m[0] << '\n'	<< m[1] << '\n' << m[2] << '\n';   }
    friend std::istream& operator >> ( std::istream& is, mat3& m )	 	 { return is >> m._m[0] >> m._m[1] >> m._m[2] ; }

    operator const GLfloat* () const	{ return static_cast<const GLfloat*>( &_m[0].x ); }
    operator	   GLfloat* ()			{ return static_cast<	   GLfloat*>( &_m[0].x ); }
};

inline mat3 matrixCompMult( const mat3& A, const mat3& B )					{
    return mat3(	A[0][0]*B[0][0], A[0][1]*B[0][1], A[0][2]*B[0][2],
					A[1][0]*B[1][0], A[1][1]*B[1][1], A[1][2]*B[1][2],
					A[2][0]*B[2][0], A[2][1]*B[2][1], A[2][2]*B[2][2] );	}

inline mat3 transpose( const mat3& A )				{
    return mat3(	A[0][0], A[0][1], A[0][2],
					A[1][0], A[1][1], A[1][2],
					A[2][0], A[2][1], A[2][2] );	}


class mat4		 	////////////////////////////////////////////////////////////////////////////////////////// mat4
{
    vec4  _m[4];
public:
    mat4( const GLfloat d = GLfloat(1) )  { _m[0].x = d;  _m[1].y = d;  _m[2].z = d;  _m[3].w = d; }	// Create a diagional matrix
    mat4( const vec4& a, const vec4& b, const vec4& c, const vec4& d )	{ _m[0] = a;  _m[1] = b;  _m[2] = c;  _m[3] = d; }
    mat4( GLfloat m00, GLfloat m10, GLfloat m20, GLfloat m30,
	  GLfloat m01, GLfloat m11, GLfloat m21, GLfloat m31,
	  GLfloat m02, GLfloat m12, GLfloat m22, GLfloat m32,
	  GLfloat m03, GLfloat m13, GLfloat m23, GLfloat m33 )
		{ _m[0] = vec4( m00, m01, m02, m03 );	    _m[1] = vec4( m10, m11, m12, m13 );	    _m[2] = vec4( m20, m21, m22, m23 );	    _m[3] = vec4( m30, m31, m32, m33 );		}

    mat4( const mat4& m )	{ if ( *this == m ) return;		_m[0] = m._m[0];	_m[1] = m._m[1];	_m[2] = m._m[2];	_m[3] = m._m[3];	}

		  vec4& operator [] ( int i )		{ return _m[i]; }
    const vec4& operator [] ( int i ) const { return _m[i]; }

    mat4 operator + ( const mat4& m )	const	{ return mat4( _m[0]+m[0], _m[1]+m[1], _m[2]+m[2], _m[3]+m[3] ); }
    mat4 operator - ( const mat4& m )	const	{ return mat4( _m[0]-m[0], _m[1]-m[1], _m[2]-m[2], _m[3]-m[3] ); }
    mat4 operator * ( const GLfloat s ) const 	{ return mat4( s*_m[0], s*_m[1], s*_m[2], s*_m[3] ); }
    mat4 operator / ( const GLfloat s ) const	{ return *this * (GLfloat(1) / s); }
    mat4 operator * ( const mat4& m ) const		{	mat4  a( 0 );
													for ( int i = 0; i < 4; ++i )
														for ( int j = 0; j < 4; ++j )
															for ( int k = 0; k < 4; ++k )
																a[i][j] += _m[i][k] * m[k][j];		
													return a;	}

    friend mat4 operator * ( const GLfloat s, const mat4& m )	{ return m * s; }
	

    mat4& operator += ( const mat4& m )		{	_m[0] += m[0];  _m[1] += m[1];  _m[2] += m[2];  _m[3] += m[3];	return *this;    }
    mat4& operator -= ( const mat4& m )		{	_m[0] -= m[0];  _m[1] -= m[1];  _m[2] -= m[2];  _m[3] -= m[3];	return *this;    }
    mat4& operator *= ( const GLfloat s )	{	_m[0] *= s;		_m[1] *= s;		_m[2] *= s;		_m[3] *= s;		return *this;    }
    mat4& operator *= ( const mat4& m )		{	mat4  a( 0 );
												for ( int i = 0; i < 4; ++i )
													for ( int j = 0; j < 4; ++j )
														for ( int k = 0; k < 4; ++k )
															a[i][j] += _m[i][k] * m[k][j];		
												return *this = a;	}

    mat4& operator /= ( const GLfloat s ) {	return *this *= (GLfloat(1.0) / s);    }

    vec4 operator * ( const vec4& v ) const {  
		return vec4(	_m[0][0]*v.x + _m[0][1]*v.y + _m[0][2]*v.z + _m[0][3]*v.w,
						_m[1][0]*v.x + _m[1][1]*v.y + _m[1][2]*v.z + _m[1][3]*v.w,
						_m[2][0]*v.x + _m[2][1]*v.y + _m[2][2]*v.z + _m[2][3]*v.w,
						_m[3][0]*v.x + _m[3][1]*v.y + _m[3][2]*v.z + _m[3][3]*v.w);	}	// m * v
	
    friend std::ostream& operator << ( std::ostream& os, const mat4& m ) {	return os << '\n' << m[0] << '\n'	<< m[1] << '\n' << m[2] << '\n' << m[3] << '\n';   }
    friend std::istream& operator >> ( std::istream& is, mat4& m )		 { return is >> m._m[0] >> m._m[1] >> m._m[2] >> m._m[3]; }

    operator const GLfloat* () const	{ return static_cast<const GLfloat*>( &_m[0].x ); }
    operator GLfloat* ()				{ return static_cast<GLfloat*>( &_m[0].x ); }
};

inline mat4 matrixCompMult( const mat4& A, const mat4& B )					
{	return mat4(
	A[0][0]*B[0][0], A[0][1]*B[0][1], A[0][2]*B[0][2], A[0][3]*B[0][3],
	A[1][0]*B[1][0], A[1][1]*B[1][1], A[1][2]*B[1][2], A[1][3]*B[1][3],
	A[2][0]*B[2][0], A[2][1]*B[2][1], A[2][2]*B[2][2], A[2][3]*B[2][3],
	A[3][0]*B[3][0], A[3][1]*B[3][1], A[3][2]*B[3][2], A[3][3]*B[3][3] );	
}

													///////////////////////////////////////////////  Transforms:  /////////////////////////////////////////// 

inline mat4 transpose( const mat4& A )										
{	return mat4( 
		A[0][0], A[0][1], A[0][2], A[0][3],
		A[1][0], A[1][1], A[1][2], A[1][3],
		A[2][0], A[2][1], A[2][2], A[2][3],
		A[3][0], A[3][1], A[3][2], A[3][3] );								
}

inline mat4 RotateX( const GLfloat theta )
{	mat4 c;    
    c[2][2] = c[1][1] = cos(DegreesToRadians * theta);
    c[2][1] = sin(DegreesToRadians * theta);
    c[1][2] = -c[2][1];
    return c;
}

inline mat4 RotateY( const GLfloat theta )
{	mat4 c;
    c[2][2] = c[0][0] = cos(DegreesToRadians * theta);
    c[0][2] = sin(DegreesToRadians * theta);
    c[2][0] = -c[0][2];
    return c;
}

inline mat4 RotateZ( const GLfloat theta )
{	mat4 c;
    c[0][0] = c[1][1] = cos(DegreesToRadians * theta);
    c[1][0] = sin(DegreesToRadians * theta);
    c[0][1] = -c[1][0];
    return c;
}

inline mat4 Translate( const GLfloat x, const GLfloat y, const GLfloat z )
{   mat4 c;
    c[0][3] = x;
    c[1][3] = y;
    c[2][3] = z;
    return c;
}

inline mat4 Translate( const vec3& v )	{    return Translate( v.x, v.y, v.z );	}
inline mat4 Translate( const vec4& v )	{    return Translate( v.x, v.y, v.z );	}

inline mat4 Scale( const GLfloat x, const GLfloat y, const GLfloat z )	{   mat4 c;
																			c[0][0] = x;    c[1][1] = y;    c[2][2] = z;
																			return c;										}
inline mat4 Scale( const vec3& v ) {    return Scale( v.x, v.y, v.z );	}

inline mat4 Ortho( const GLfloat left, const GLfloat right, const GLfloat bottom, const GLfloat top, const GLfloat zNear, const GLfloat zFar )
{	mat4 c;
    c[0][0] =				2	/	(right - left);
    c[1][1] =				2	/	(top - bottom);
    c[2][2] =				2	/	(zNear - zFar);
    c[0][3] = -(right + left)	/	(right - left);
    c[1][3] = -(top + bottom)	/	(top - bottom);
    c[2][3] = -(zFar + zNear)	/	(zFar - zNear);
    return c;
}

inline mat4 Ortho2D( const GLfloat left, const GLfloat right, const GLfloat bottom, const GLfloat top )	{	return Ortho( left, right, bottom, top, -1.0, 1.0 ); }

inline mat4 Frustum( const GLfloat left, const GLfloat right, const GLfloat bottom, const GLfloat top,  const GLfloat zNear, const GLfloat zFar )
{   mat4 c;
    c[0][0] =		2*zNear		/	(right - left);
    c[0][2] = (right + left)	/	(right - left);
    c[1][1] =		2*zNear		/	(top - bottom);
    c[1][2] = (top + bottom)	/	(top - bottom);
    c[2][2] = -(zFar + zNear)	/	(zFar - zNear);
    c[2][3] = -2*zFar*zNear		/	(zFar - zNear);
    c[3][2] = -1;
	c[3][3] = 0;
    return c;
}

inline mat4 Perspective( const GLfloat fovy, const GLfloat aspect,  const GLfloat zNear, const GLfloat zFar)
{	mat4 c;
    GLfloat top   = tan(fovy*DegreesToRadians/2) * zNear, right = top * aspect;
    c[0][0] =			zNear	/	right;
    c[1][1] =			zNear	/	top;
    c[2][2] = -(zFar + zNear)	/	(zFar - zNear);
    c[2][3] = -2*zFar*zNear		/	(zFar - zNear);
    c[3][2] = -1;
    return c;
}

inline mat4 LookAt( const vec4& eye, const vec4& at, const vec4& up )
{
    vec4 n = normalize(eye - at);
	vec3 uu = normalize(cross(up, n));
    vec4 u = vec4(uu.x, uu.y, uu.z, 0);
    vec3 vv = normalize(cross(n,u));
	vec4 v = vec4(vv.x, vv.y, vv.z, 0);
    vec4 t = vec4( 0, 0, 0, 1 );
    mat4 c = mat4(u, v, n, t);
    return c * Translate( -eye );
}