#pragma once
#include "Utilities.h"

struct ShapeData	{	GLuint vao;
						int numVertices;	};

void generateCube(GLuint program, ShapeData* cubeData);
void generateSphere(GLuint program, ShapeData* sphereData);
void generateCone(GLuint program, ShapeData* coneData);
void generateCylinder(GLuint program, ShapeData* cylData);
void generateHexagon(GLuint program, ShapeData* hexData);
//void generateShape(GLuint program, ShadeData* polyData);
void generateTriPrism(GLuint program, ShapeData* triPrismData);
//----------------------------------------------------------------------------
// Cube
const int numCubeVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 cubePoints [numCubeVertices];
point3 cubeNormals[numCubeVertices];
point2 cubeUV     [numCubeVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1 ),
    point4( -0.5,  0.5,  0.5, 1 ),
    point4(  0.5,  0.5,  0.5, 1 ),
    point4(  0.5, -0.5,  0.5, 1 ),
    point4( -0.5, -0.5, -0.5, 1 ),
    point4( -0.5,  0.5, -0.5, 1 ),
    point4(  0.5,  0.5, -0.5, 1 ),
    point4(  0.5, -0.5, -0.5, 1 )
};

// quad generates two triangles for each face and assigns normals and texture coordinates
//    to the vertices
int Index = 0;
void quad( int a, int b, int c, int d, const point3& normal )
{
    cubePoints[Index] = vertices[a]; cubeNormals[Index] = normal; 
    cubeUV[Index] = point2(0, 1); Index++;
    cubePoints[Index] = vertices[b]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(0, 0); Index++;
    cubePoints[Index] = vertices[c]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1, 0); Index++;
    cubePoints[Index] = vertices[a]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(0, 1); Index++;
    cubePoints[Index] = vertices[c]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1, 0); Index++;
    cubePoints[Index] = vertices[d]; cubeNormals[Index] = normal;
    cubeUV[Index] = point2(1, 1); Index++;
}

// generate 12 triangles: 36 vertices, 36 normals, 36 texture coordinates
void colorcube()
{
    quad( 1, 0, 3, 2, point3( 0,  0,  1) );
    quad( 2, 3, 7, 6, point3( 1,  0,  0) );
    quad( 3, 0, 4, 7, point3( 0, -1,  0) );
    quad( 6, 5, 1, 2, point3( 0,  1,  0) );
    quad( 4, 5, 6, 7, point3( 0,  0, -1) );
    quad( 5, 4, 0, 1, point3(-1,  0,  0) );
}

// initialization
void generateCube(GLuint program, ShapeData* cubeData)
{
    colorcube();
    cubeData->numVertices = numCubeVertices;

    // Create a vertex array object
    glGenVertexArrays( 1, &cubeData->vao );
    glBindVertexArray( cubeData->vao );

    // Set vertex attributes
    setVertexAttrib(program, 
        (float*)cubePoints,  sizeof(cubePoints), 
        (float*)cubeNormals, sizeof(cubeNormals),
        (float*)cubeUV,      sizeof(cubeUV));
}



/****************************/
//to generate another type of cube

/*point2 cubeUV_textured [numCubeVertices] = {
    point2 （.25,.375), point2(0.5,0.375),point2(0.5,0.625),

};
void generateTexutureCube(GLuint program, ShapeData* texturedCubeData)
{
    //colorcube();
    texturedCubeData->numVertices = numCubeVertices;
    
    // Create a vertex array object
    glGenVertexArrays( 1, &cubeData->vao );
    glBindVertexArray( cubeData->vao );
    
    // Set vertex attributes
    setVertexAttrib(program,
                    (float*)cubePoints,  sizeof(cubePoints),
                    (float*)cubeNormals, sizeof(cubeNormals),
                    (float*)cubeUV_textured,      sizeof(cubeUV_textured));
}*/
/****************************/



//----------------------------------------------------------------------------
// Sphere approximation by recursive subdivision of a tetrahedron

const int N = 5;  // number of subdivisions
const int numSphereVertices = 16*256*3;  // number of resulting points

point4 spherePoints [numSphereVertices];
point3 sphereNormals[numSphereVertices];
point2 sphereUVs[numSphereVertices];

// four equally spaced points on the unit circle

point4 v[4] = {
    vec4( 0, 0, 1, 1 ), 
    vec4(0.0, 0.942809, -0.333333, 1.0),
    vec4(-0.816497, -0.471405, -0.333333, 1.0),
    vec4(0.816497, -0.471405, -0.333333, 1.0)
};

static int k = 0;

// move a point to unit circle

point4 unit(const point4 &p)
{
    point4 c;
    double d=0;
    for(int i=0; i<3; i++) d+=p[i]*p[i];
    d=sqrt(d);
    if(d > 0) for(int i=0; i<3; i++) c[i] = p[i]/d;
    c[3] = 1;
    return c;
}

void triangle(point4 a, point4 b, point4 c)
{
    spherePoints[k] = a;
    k++;
    spherePoints[k] = b;
    k++;
    spherePoints[k] = c;
    k++;
}

void divide_triangle(point4 a, point4 b, point4 c, int n)
{
    point4 v1, v2, v3;
    if(n>0)
    {
        v1 = unit(a + b);
        v2 = unit(a + c);
        v3 = unit(b + c);   
        divide_triangle(a , v2, v1, n-1);
        divide_triangle(c , v3, v2, n-1);
        divide_triangle(b , v1, v3, n-1);
        divide_triangle(v1, v2, v3, n-1);
    }
    else triangle(a, b, c);
}

void tetrahedron(int n)
{
    divide_triangle(v[0], v[1], v[2], n);
    divide_triangle(v[3], v[2], v[1], n);
    divide_triangle(v[0], v[3], v[1], n);
    divide_triangle(v[0], v[3], v[2], n);
}

// initialization

void generateSphere(GLuint program, ShapeData* sphereData)
{
    tetrahedron(N);

    sphereData->numVertices = numSphereVertices;

    // Normals
    for (int i = 0; i < numSphereVertices; i++)
    {
        sphereNormals[i] = point3(spherePoints[i].x, spherePoints[i].y, spherePoints[i].z);
    }
    
    // TexCoords
    double u, v;
    for (int i = 0; i < numSphereVertices; i++)
    {
        v = 0.5 - asin(spherePoints[i].y)/PI ; //0~1
        u = 0.5*(atan2(spherePoints[i].z,spherePoints[i].x)/PI + 1); //0~1
        sphereUVs[i] = point2(u,v);
    }

    // Create a vertex array object
    glGenVertexArrays( 1, &sphereData->vao );
    glBindVertexArray( sphereData->vao );

    // Set vertex attributes
    setVertexAttrib(program,
        (float*)spherePoints,  sizeof(spherePoints),
        (float*)sphereNormals, sizeof(sphereNormals),
        (float*)sphereUVs, sizeof(sphereUVs));
}

//----------------------------------------------------------------------------
// Cone

const int numConeDivisions = 32;
const int numConeVertices = numConeDivisions * 6;

point4 conePoints [numConeVertices];
point3 coneNormals[numConeVertices];

point2 circlePoints[numConeDivisions];

void makeCircle(point2* dest, int numDivisions)
{
    for (int i = 0; i < numDivisions; i++)
    {
        float a = i * 2 * PI / numDivisions;
        dest[i] = point2(cosf(a), sinf(a));
    }
}

void makeConeWall(point4* destp, point3* destn, int numDivisions, float z1, float z2, int& Index, int dir)
{
    for (int i = 0; i < numDivisions; i++)
    {
        point3 p1(circlePoints[i].x, circlePoints[i].y, z1);
        point3 p2(0, 0, z2);
        point3 p3(circlePoints[(i+1)%numDivisions].x, circlePoints[(i+1)%numDivisions].y, z1);
        if (dir == -1)
        {
            point3 temp = p1;
            p1 = p3;
            p3 = temp;
        }
        point3 n = cross(p1-p2, p3-p2);
        destp[Index] = p1; destn[Index] = n; Index++;
        destp[Index] = p2; destn[Index] = n; Index++;
        destp[Index] = p3; destn[Index] = n; Index++;
    }    
}

void generateCone(GLuint program, ShapeData* coneData)
{
    makeCircle(circlePoints, numConeDivisions);
    int Index = 0;
    makeConeWall(conePoints, coneNormals, numConeDivisions, 1, 1, Index, 1);
    makeConeWall(conePoints, coneNormals, numConeDivisions, 1, -1, Index, -1);
    
    coneData->numVertices = numConeVertices;

    // Create a vertex array object
    glGenVertexArrays( 1, &coneData->vao );
    glBindVertexArray( coneData->vao );
    
    // Set vertex attributes
    setVertexAttrib(program,
                    (float*)conePoints,  sizeof(conePoints),
                    (float*)coneNormals, sizeof(coneNormals),
                    0, 0 );
}

//----------------------------------------------------------------------------
// Cylinder

const int numCylDivisions = 32;
const int numCylVertices = numCylDivisions * 12;

point4 cylPoints [numCylVertices];
point3 cylNormals[numCylVertices];

void generateCylinder(GLuint program, ShapeData* cylData)
{
    makeCircle(circlePoints, numCylDivisions);
    int Index = 0;
    makeConeWall(cylPoints, cylNormals, numCylDivisions, 1, 1, Index, 1);
    makeConeWall(cylPoints, cylNormals, numCylDivisions, -1, -1, Index, -1);
    
    for (int i = 0; i < numCylDivisions; i++)
    {
        int i2 = (i+1)%numCylDivisions;
        point3 p1(circlePoints[i2].x, circlePoints[i2].y, -1);
        point3 p2(circlePoints[i2].x, circlePoints[i2].y, 1);
        point3 p3(circlePoints[i].x,  circlePoints[i].y,  1);
        //point3 n = cross(p3-p2, p1-p2);
        cylPoints[Index] = p1; cylNormals[Index] = point3(p1.x, p1.y, 0); Index++;
        cylPoints[Index] = p2; cylNormals[Index] = point3(p2.x, p2.y, 0); Index++;
        cylPoints[Index] = p3; cylNormals[Index] = point3(p3.x, p3.y, 0); Index++;
        p1 = point3(circlePoints[i2].x, circlePoints[i2].y, -1);
        p2 = point3(circlePoints[i].x,  circlePoints[i].y,  1);
        p3 = point3(circlePoints[i].x,  circlePoints[i].y,  -1);
        //n = cross(p3-p2, p1-p2);
        cylPoints[Index] = p1; cylNormals[Index] = point3(p1.x, p1.y, 0); Index++;
        cylPoints[Index] = p2; cylNormals[Index] = point3(p2.x, p2.y, 0); Index++;
        cylPoints[Index] = p3; cylNormals[Index] = point3(p3.x, p3.y, 0); Index++;
    }
    
    cylData->numVertices = numCylVertices;
    
    // Create a vertex array object
    glGenVertexArrays( 1, &cylData->vao );
    glBindVertexArray( cylData->vao );
    
    // Set vertex attributes
    setVertexAttrib(program,
                    (float*)cylPoints,  sizeof(cylPoints),
                    (float*)cylNormals, sizeof(cylNormals),
                    0, 0 );
}

//-----------
//My Shape : Hexagonal
//
const int numHexVertices = 60;//36(6*3*2)+24(2*4*3)

point4 hexPoints [numHexVertices];
point3 hexNormals[numHexVertices];
point2 hexUV     [numHexVertices];
point4 hexVertices[12] = {
    point4 (1, 0, 0.5, 1),
    point4 (0.5, -0.8660254, 0.5, 1),
    point4 (-0.5, -0.8660254, 0.5, 1),
    point4 (-1, 0, 0.5, 1),
    point4 (-0.5, 0.8660254, 0.5, 1),
    point4 (0.5, 0.8660254, 0.5,1),
    
    point4 (1, 0, -0.5, 1),
    point4 (0.5, -0.8660254, -0.5, 1),
    point4 (-0.5, -0.8660254, -0.5, 1),
    point4 (-1, 0, -0.5, 1),
    point4 (-0.5, 0.8660254, -0.5, 1),
    point4 (0.5, 0.8660254,-0.5,1)
};
int  index1 = 0;

void topAndBottom(){
//This is the top and bottom face of this hexagonal.
hexPoints[index1] = hexVertices[0];hexNormals[index1] = point3(0 , 0, 1);
    hexUV[index1] = point2(0,0);
    index1++;
hexPoints[index1] = hexVertices[1];hexNormals[index1] = point3(0 , 0, 1);
    hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[2];hexNormals[index1] = point3(0 , 0, 1);
    hexUV[index1] = point2(0,0);index1++;

hexPoints[index1] = hexVertices[0];hexNormals[index1] = point3(0 , 0, 1);
    hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[2];hexNormals[index1] = point3(0 , 0, 1);
    hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[3];hexNormals[index1] = point3(0 , 0, 1);
    hexUV[index1] = point2(0,0);index1++;

hexPoints[index1] = hexVertices[0];hexNormals[index1] = point3(0 , 0, 1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[3];hexNormals[index1] = point3(0 , 0, 1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[4];hexNormals[index1] = point3(0 , 0, 1); hexUV[index1] = point2(0,0);index1++;

hexPoints[index1] = hexVertices[0];hexNormals[index1] = point3(0 , 0, 1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[4];hexNormals[index1] = point3(0 , 0, 1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[5];hexNormals[index1] = point3(0 , 0, 1); hexUV[index1] = point2(0,0);index1++;


hexPoints[index1] = hexVertices[6];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[7];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[8];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;

hexPoints[index1] = hexVertices[6];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[8];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[9];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;

hexPoints[index1] = hexVertices[6];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[9];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[10];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;

hexPoints[index1] = hexVertices[6];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[10];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;
hexPoints[index1] = hexVertices[11];hexNormals[index1] = point3(0 , 0, -1); hexUV[index1] = point2(0,0);index1++;

}

void quadForHex (int a, int b, int c, int d, const point3& normal)
{
    hexPoints[index1] = hexVertices[a]; hexNormals[index1] = normal;
    hexUV[index1] = point2(0 , 1);
    index1 ++;
    hexPoints[index1] = hexVertices[b]; hexNormals[index1] = normal;
    hexUV[index1] = point2(0 , 0);
    index1 ++;
    hexPoints[index1] = hexVertices[c]; hexNormals[index1] = normal;
    hexUV[index1] = point2(1 , 0);
    index1 ++;
    
    hexPoints[index1] = hexVertices[a]; hexNormals[index1] = normal;
    hexUV[index1] = point2(0 , 1);
    index1 ++;
    hexPoints[index1] = hexVertices[c]; hexNormals[index1] = normal;
    hexUV[index1] = point2(1 , 0);
    index1 ++;
    hexPoints[index1] = hexVertices[d]; hexNormals[index1] = normal;
    hexUV[index1] = point2(1 , 1);
    index1 ++;
}
void colorHex()
{
    //the normal vector of each face is very easy to calculate.
    //so I give these normal vectors directly.
    quadForHex (0, 5, 11, 6, point3( 0.8660254, 0.5, 0 ));
    quadForHex (5, 4, 10, 11, point3( 0, 1, 0 ));
    quadForHex (4, 3, 9, 10, point3(-0.8660254, 0.5, 0));
    quadForHex (3, 2, 8, 9, point3(-0.8660254, -0.5, 0));
    quadForHex (2, 1, 7, 8, point3(0, -1, 0));
    quadForHex (1, 0, 6, 7, point3(0.8660254, -0.5, 0));
}
void generateHexagon(GLuint program, ShapeData* hexData)
{
    topAndBottom();
    colorHex();
    hexData->numVertices = numHexVertices;
    // Create a vertex array object
    glGenVertexArrays( 1, &hexData->vao );
    glBindVertexArray( hexData->vao );
    
    // Set vertex attributes
    setVertexAttrib(program,
                    (float*)hexPoints,  sizeof(hexPoints),
                    (float*)hexNormals, sizeof(hexNormals),
                    (float*)hexUV, sizeof(hexUV) );
}


//
//My New Shape: Triangular Prism
//



//triangular prism
const int numTriangularPrismVertices = 24;//   (3*2*3)+(2*1*3)

point4 triPrismPoints [numTriangularPrismVertices];
point3 triPrismNormals[numTriangularPrismVertices];
point2 triPrismUV     [numTriangularPrismVertices];

point4 triPrismVertices[6] ={
    point4(1, 0.5, 0, 1),
    point4(0, 0.5, 0, 1),
    point4(0, 0.5, 1, 1),
    point4(1, -0.5, 0, 1),
    point4(0, -0.5, 0, 1),
    point4(0, -0.5, 1, 1)
};

int index_TP =0;

void topAndBottomForTP(){
    
    triPrismPoints[index_TP] = triPrismVertices[0];
    triPrismNormals[index_TP] = point3(0,1,0);
    triPrismUV[index_TP] = point2(0,0);index_TP++;
    
    triPrismPoints[index_TP] = triPrismVertices[1];
    triPrismNormals[index_TP] = point3(0,1,0);
    triPrismUV[index_TP] = point2(0,0);index_TP++;
    
    triPrismPoints[index_TP] = triPrismVertices[2];
    triPrismNormals[index_TP] = point3(0,1,0);
    triPrismUV[index_TP] = point2(0,0);index_TP++;
    
    triPrismPoints[index_TP] = triPrismVertices[3];
    triPrismNormals[index_TP] = point3(0,-1,0);
    triPrismUV[index_TP] = point2(0,0);index_TP++;
    
    triPrismPoints[index_TP] = triPrismVertices[4];
    triPrismNormals[index_TP] = point3(0,-1,0);
    triPrismUV[index_TP] = point2(0,0);index_TP++;
    
    triPrismPoints[index_TP] = triPrismVertices[5];
    triPrismNormals[index_TP] = point3(0,-1,0);
    triPrismUV[index_TP] = point2(0,0);index_TP++;
}
void quadForTriPrism (int a, int b, int c, int d, const point3& normal)
{
    triPrismPoints[index_TP] = triPrismVertices[a]; triPrismNormals[index_TP] = normal;
    triPrismUV[index_TP] = point2(0, 1);
    index_TP ++;
    triPrismPoints[index_TP] = triPrismVertices[b]; triPrismNormals[index_TP] = normal;
    triPrismUV[index_TP] = point2(0, 0);
    index_TP ++;
    triPrismPoints[index_TP] = triPrismVertices[c]; triPrismNormals[index_TP] = normal;
    triPrismUV[index_TP] = point2(1, 0);
    index_TP ++;
    
    triPrismPoints[index_TP] = triPrismVertices[a]; triPrismNormals[index_TP] = normal;
    triPrismUV[index_TP] = point2(0, 1);
    index_TP ++;
    triPrismPoints[index_TP] = triPrismVertices[c]; triPrismNormals[index_TP] = normal;
    triPrismUV[index_TP] = point2(1, 0);
    index_TP ++;
    triPrismPoints[index_TP] = triPrismVertices[d]; triPrismNormals[index_TP] = normal;
    triPrismUV[index_TP] = point2(1, 1);
    index_TP ++;
}

void colorTriPrism()
{
    quadForTriPrism(0,1,4,3,point3(0,0,-1));
    quadForTriPrism(1,2,5,4,point3(-1,0,0));
    quadForTriPrism(2,5,3,0,point3(0.707107,0,0.707107));
    
}

void generateTriPrism(GLuint program, ShapeData* triPrismData)
{
    
    topAndBottomForTP();
    colorTriPrism();
    
    
    triPrismData->numVertices = numTriangularPrismVertices;
    // Create a vertex array object
    glGenVertexArrays( 1, &triPrismData->vao );
    glBindVertexArray( triPrismData->vao );
    
    // Set vertex attributes
    setVertexAttrib(program,
                    (float*)triPrismPoints,  sizeof(triPrismPoints),
                    (float*)triPrismNormals, sizeof(triPrismNormals),
    
                    (float*)triPrismUV,      sizeof(triPrismUV) );
}