#pragma once

#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#endif

#define GLEW_STATIC
#define GL_GLEXT_PROTOTYPES
#define _CRT_SECURE_NO_DEPRECATE
#ifdef _WIN32
#include "..\GL\glew.h"

#ifdef _WIN64
#pragma comment(lib, "../GL/x64/freeglut.lib")
#pragma comment(lib, "../GL/x64/glew32s.lib")
#pragma comment(lib, "../GL/x64/glew32mxs.lib")
#else
#pragma comment(lib, "../GL/Win32/freeglut.lib")
#pragma comment(lib, "../GL/Win32/glew32s.lib")
#pragma comment(lib, "../GL/Win32/glew32mxs.lib")
#endif

#include <Windows.h>

#pragma comment(lib, "winmm.lib")
class Timer
{	
	LONGLONG cur_time, perf_cnt, last_time;

	DWORD time_count;
	bool perf_flag;
	float time_scale;
public:
	Timer() : last_time()
	{
		if(QueryPerformanceFrequency((LARGE_INTEGER*) &perf_cnt))
		{
			perf_flag=true;
			time_count=DWORD(perf_cnt); //perf_cnt counts per second
			QueryPerformanceCounter((LARGE_INTEGER*) &last_time);
			time_scale=1.0f/perf_cnt;
		}
		else
		{
			perf_flag=false;
			last_time=timeGetTime();
			time_scale=.001f;
			time_count=33;
		}
	}

	float Timer::GetElapsedTime()	//In seconds. Courtesy Alan Gasperini
	{
		if(perf_flag)
			QueryPerformanceCounter((LARGE_INTEGER*) &cur_time);
		else
			cur_time=timeGetTime();
				
		return (cur_time-last_time)*time_scale;
	}

	inline void Reset()	{	last_time = cur_time;	}
} TM;

#else
#include <sys/time.h>
class Timer
{
	timeval cur_time;
public:
	Timer() { Reset(); }
	inline void Reset() { gettimeofday(&cur_time, 0); }
	float GetElapsedTime()
	{
		float dif;
		timeval newtime;
		gettimeofday(&newtime, 0);
		return (newtime.tv_sec-cur_time.tv_sec) + (newtime.tv_usec-cur_time.tv_usec)/1000000.0;
	}
} TM;
#endif

#include "../GL/freeglut.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stack>
#include "../CS174a template/mat_and_vec.h"

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))
typedef vec4 color4;
typedef vec4 point4;
typedef vec3 point3;
typedef vec2 point2;

void setVertexAttrib(GLuint program, 
                     GLfloat* points,    GLsizeiptr psize, 
                     GLfloat* normals,   GLsizeiptr nsize,
                     GLfloat* texcoords, GLsizeiptr tsize)
{
    GLuint buffer[2];
    glGenBuffers( 2, buffer );

    glBindBuffer( GL_ARRAY_BUFFER, buffer[0] );
    glBufferData( GL_ARRAY_BUFFER, psize, points, GL_STATIC_DRAW );
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    glBindBuffer( GL_ARRAY_BUFFER, buffer[1] );
    glBufferData( GL_ARRAY_BUFFER, nsize, normals, GL_STATIC_DRAW );
    GLuint vNormal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    if (texcoords)
    {
        GLuint tbuffer;
        glGenBuffers( 1, &tbuffer );
        glBindBuffer( GL_ARRAY_BUFFER, tbuffer );
        glBufferData( GL_ARRAY_BUFFER, tsize, texcoords, GL_STATIC_DRAW );
        GLuint vTexCoords = glGetAttribLocation( program, "TexCoordIn" );
        glEnableVertexAttribArray( vTexCoords );
        glVertexAttribPointer( vTexCoords, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    }
    
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray(0);
}

struct TgaImage				// Textures
{   int width, height;
    unsigned char byteCount, *data;
	TgaImage(const char *filename) : data(0), width(), height(), byteCount()
	{
		unsigned char type[4], info[6];				//image type either 2 (color) or 3 (greyscale)
		FILE *file = fopen(filename, "rb");			assert(file);													
		fread (&type, sizeof (char), 3, file);		assert (type[1] == 0 && (type[2] == 2 || type[2] == 3));		fseek (file, 12, SEEK_SET);
		fread (&info, sizeof (char), 6, file);		

		width  = info[0] + info[1] * 256;
		height = info[2] + info[3] * 256;
		byteCount = info[4] / 8;						assert (byteCount == 3 || byteCount == 4);
		long imageSize = width * height * byteCount;
		
		data = new unsigned char[imageSize];
		fread(data, sizeof(unsigned char), imageSize, file);		fclose(file);
	}
    ~TgaImage()		{	delete[] data;		data = 0;	 }
};


static char* readShaderFile(const char* shaderFile)
{
	std::ifstream file(shaderFile, std::ios::in|std::ios::binary|std::ios::ate);
	assert(file.is_open());
	std::streampos size = file.tellg();
    char* memblock = new char [(long)size + 1];
    file.seekg ( 0, std::ios::beg );
    file.read ( memblock, size );
    file.close();
	memblock[ size ] = '\0';
    return memblock;
}						// Helper function to load vertex and fragment shader files; Create a NULL-terminated string by reading the provided file.

GLuint LoadShaders(const char* vShaderFile, const char* fShaderFile)			// Create a GLSL program object from vertex and fragment shader files.
{
    struct Shader {	const char*	filename;	GLenum	type;	GLchar*		source;    }  
		shaders[2] = {	{		vShaderFile,		GL_VERTEX_SHADER,	NULL },	
						{		fShaderFile,		GL_FRAGMENT_SHADER, NULL }    };

    GLuint program = glCreateProgram();
	GLint  compiled, linked, logSize;
    
    for ( int i = 0; i < 2; i++ ) 
	{
		Shader& s = shaders[i];

#ifdef EMSCRIPTEN
		char precision[] = "precision mediump float;", *file_contents = readShaderFile( s.filename );
		s.source = new char[ strlen( precision ) + strlen ( file_contents ) ];
		strcpy( s.source, strcat( precision, file_contents ) );
#else
		s.source = readShaderFile( s.filename );
#endif
		assert ( shaders[i].source && "FAILED TO LOCATE A SHADER'S FILE" );

		GLuint shader = glCreateShader( s.type );
		glShaderSource( shader, 1, (const GLchar**) &s.source, NULL );
		glCompileShader( shader );

		glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled && std::cerr << s.filename << " failed to compile:" << '\n' ) 
		{
			glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
			char* logMsg = new char[logSize];
			glGetShaderInfoLog( shader, logSize, NULL, logMsg );
			std::cerr << logMsg << '\n';		
			assert( false && "A SHADER FAILED TO COMPILE; SEE ABOVE DETAILS" );
		}
		delete [] s.source;

		glAttachShader( program, shader );
    }

    glLinkProgram(program);		// Link  and error check.

    glGetProgramiv( program, GL_LINK_STATUS, &linked );
    if ( !linked && std::cerr << "Shader program failed to link" << '\n' )  
	{
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog( program, logSize, NULL, logMsg );
		std::cerr << logMsg << '\n';			
		assert( false && "SHADER PROGRAMS FAILED TO LINK; SEE ABOVE FOR DETAILS");
    }

    glUseProgram(program);
    return program;
}